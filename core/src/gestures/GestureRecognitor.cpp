#include "../../include/core/gestures/GestureRecognitor.h"

#include "../../include/core/gestures/GestureDetector.h"
#include "../../include/core/gestures/Gesture.h"

#include "../../include/core/ContourDetector.h"

#include <string>

namespace core
{

bool GestureRecognitor::skinBinarizationFunction(unsigned char/* y*/, unsigned char u, unsigned char v)
{
    // isSkinEllipse
    static const float cx = 109.38f;
    static const float cy = 152.02f;
    static const float theta = 2.53f;
    static const float cs = cosf(theta);
    static const float sn = sinf(theta);
    static const float ecx = 1.6f;
    static const float ecy = 2.41f;
    static const float a = 25.39f;
    static const float b = 14.03f;
    static const float a2 = a * a;
    static const float b2 = b * b;

    const float cb = (float)u;
    const float cr = (float)v;

    float x = cs * (cb - cx) + sn * (cr - cy);
    float y = -sn * (cb - cx) + cs * (cr - cy);

    float ellipseVal = (x - ecx) * (x - ecx) / a2 + (y - ecy) * (y - ecy) / b2;

    return (ellipseVal <= 1.f);
}


GestureRecognitor::GestureRecognitor()
    : m_contourDetector(new ContourDetector(skinBinarizationFunction))
{
}

GestureRecognitor::~GestureRecognitor()
{
}


void GestureRecognitor::recognize(const char* what)
{
    if ( !strcmp("Paper/Scissors/Stone", what) )
        addGestureDetector(new PaperScissorsStoneDetector());
}

void GestureRecognitor::detect(YUVImage* frame)
{

}

Gesture* GestureRecognitor::retrieve(const char* what) const
{
    const long gestureType = determineGestureType(what);
    if (!gestureType)
        return 0;

    GesturesIterator begin(m_gestures.begin());
    GesturesIterator end(m_gestures.end());
    for (GesturesIterator gstIt = begin; gstIt != end; ++gstIt)
        if ( gestureType == gstIt->first )
            return gstIt->second;

    return 0;
}


bool GestureRecognitor::addGestureDetector(GestureDetector* detector)
{
    if ( !(m_detectors.insert(std::pair<long, GestureDetector*>(detector->type(), detector))).second )
    {
        delete detector;
        return false;
    }

    return true;
}

long GestureRecognitor::determineGestureType(const char* strGesture) const
{
    if ( !strcmp("Paper", strGesture) )
        return PaperGesture::GestureType;
    if ( !strcmp("Scissors", strGesture) )
        return ScissorsGesture::GestureType;
    if ( !strcmp("Stone", strGesture) )
        return StoneGesture::GestureType;

    return 0;
}

} // namespace core