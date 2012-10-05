#include "../include/core/ContourDetector.h"

#include "../include/core/YUVImage.h"
#include "../include/core/geometry/Matrix.h"
#include "../include/core/geometry/Vector2D.h"

namespace core
{

const float ContourDetector::kSmallAreaKoef = 10.0f;

ContourDetector::ContourDetector(BinarizationFunction binFunc)
    : m_binFunc(binFunc)
    , m_width(0)
    , m_height(0)
    , m_scale(1)
{
}

ContourDetector::~ContourDetector(void)
{
}


void ContourDetector::detect(YUVImage& image)
{
    reset();
    setSize(image.width(), image.height());

    image.doBinaryMask(m_binFunc);
    const Matrix<unsigned char>& mask = image.getBinaryMask();
    const Matrix<int>& integral = image.getIntegralMask();

    const int imgArea = m_width * m_height;
    const int detectedArea = integral(m_width - 1, m_height - 1);
    if (detectedArea * kSmallAreaKoef < imgArea)
        return;

    detourContour(mask);
}


void ContourDetector::setSize(int width, int height)
{
    if ( width != m_width || height != m_height )
    {
        m_width = width;
        m_height = height;
    }
}

void ContourDetector::reset(void)
{
    
}

void ContourDetector::detourContour(const Matrix<unsigned char>& mask)
{


    // let's find start point on the borders of the image

}

} // namespace core