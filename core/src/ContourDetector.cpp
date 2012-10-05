#include "../include/core/ContourDetector.h"

#include "../include/core/YUVImage.h"
#include "../include/core/geometry/Matrix.h"

namespace core
{

ContourDetector::ContourDetector(BinarizationFunction binFunc)
    : m_binFunc(binFunc)
    , m_width(0)
    , m_height(0)
    , m_scale(1)
{
}

ContourDetector::~ContourDetector()
{
}


void ContourDetector::detect(YUVImage* image)
{
    
}


void ContourDetector::setSize(int width, int height)
{
    if ( width != m_width || height != m_height )
    {
        m_width = width;
        m_height = height;
    }
}

void ContourDetector::reset()
{
    
}

} // namespace core