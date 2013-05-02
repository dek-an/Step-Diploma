#ifndef GESTURES_GESTURERECOGNITOR_H_
#define GESTURES_GESTURERECOGNITOR_H_

#include <map>


namespace core
{

class YUVImage;
class Gesture;
class GestureDetector;
class ContourDetector;

class GestureRecognitor
{
    typedef std::map<long, GestureDetector*> DetectorsContainer;
    typedef DetectorsContainer::const_iterator DetectorsIterator;

    typedef std::map<long, Gesture*> GesturesContainer;
    typedef GesturesContainer::const_iterator GesturesIterator;

public:
    static bool skinBinarizationFunction(unsigned char y, unsigned char u, unsigned char v);

public:
    GestureRecognitor();
    ~GestureRecognitor();

public:
    void recognize(const std::string& what);
    void detect(YUVImage* frame);
    Gesture* retrieve(const std::string& what) const;

private:
    bool addGestureDetector(GestureDetector* detector);
    long determineGestureType(const std::string& strGesture) const;

private:
    GestureRecognitor(const GestureRecognitor&);
    GestureRecognitor& operator=(const GestureRecognitor&);

private:
    DetectorsContainer  m_detectors;
    GesturesContainer   m_gestures;
    ContourDetector*    m_contourDetector;
};

} // namespace core

#endif // GESTURES_GESTURERECOGNITOR_H_