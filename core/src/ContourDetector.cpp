#include "../include/core/ContourDetector.h"

#include "../include/core/YUVImage.h"
#include "../include/core/geometry/Matrix.h"
#include "../include/core/geometry/Vector2D.h"

#include <memory>

namespace core
{

const float kSmallAreaKoef = 10.0f;

ContourDetector::ContourDetector(BinarizationFunction binFunc)
    : m_binFunc(binFunc)
    , m_width(0)
    , m_height(0)
    , m_imageArea(0)
    , m_iStart(0)
    , m_jStart(0)
    , m_contourFounded(false)
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

    const int detectedArea = integral(m_width - 1, m_height - 1);
    if (detectedArea * kSmallAreaKoef < m_imageArea)
        return;

    findContour(mask, integral, image.getMaskBorder());
}


void ContourDetector::setSize(int width, int height)
{
    if ( width != m_width || height != m_height )
    {
        m_width = width;
        m_height = height;
        m_imageArea = width * height;
    }
}

void ContourDetector::reset(void)
{
    m_iStart = 0;
    m_jStart = 0;
    m_contourFounded = false;
}

void ContourDetector::findContour(const Matrix<unsigned char>& mask, const Matrix<int>& integral, int maskBorder)
{
    if ( findStartPoint(mask, integral, maskBorder) )
        detourContour(mask, maskBorder);
}

bool ContourDetector::findStartPoint(const Matrix<unsigned char>& mask, const Matrix<int>& integral, int maskBorder)
{
    // i - rows; j - cols
    const int iBegin = maskBorder;
    const int jBegin = maskBorder;
    const int iEnd = mask.rows() - maskBorder;
    const int jEnd = mask.cols() - maskBorder;

    // let's find start point on the borders of the image
    // consider the presence of mask borders (at least 1)
    static const int kTreshold = 6;

    // top, bottom
    for (int j = jBegin + 1; j < jEnd - 1; ++j)
    {
        // check if it is not noise
        // top
        if (mask(iBegin, j))
        {
            int val = integral(iBegin + 1, j + 1) - integral(iBegin - 1, j + 1) -
                integral(iBegin + 1, j - 2) + integral(iBegin - 1, j - 2);
            if ( val >= kTreshold)
            {
                m_iStart = iBegin;
                m_jStart = j - 1;
                return true;
            }
        }
        // bottom
        if (mask(iEnd - 1, j))
        {
            int val = integral(iEnd - 1, j + 1) - integral(iEnd - 3, j + 1) -
                integral(iEnd - 1, j - 2) + integral(iEnd - 3, j - 2);
            if ( val >= kTreshold )
            {
                m_iStart = iEnd - 1;
                m_jStart = j - 1;
                return true;
            }
        }
    }
    // left, right
    for (int i = iBegin + 1; i < iEnd - 1; ++i)
    {
        // left
        if (mask(i, jBegin))
        {
            int val = integral(i + 1, jBegin + 1) - integral(i - 2, jBegin + 1) -
                integral(i + 1, jBegin - 1) + integral(i - 2, jBegin - 1);
            if ( val >= kTreshold)
            {
                m_iStart = i - 1;
                m_jStart = jBegin;
                return true;
            }
        }
        // right
        if (mask(i, jEnd - 1))
        {
            int val = integral(i + 1, jEnd - 1) - integral(i + 1, jEnd - 3) -
                integral(i - 2, jEnd - 1) + integral(i - 2, jEnd - 3);
            if ( val >= kTreshold )
            {
                m_iStart = i - 1;
                m_jStart = jEnd - 1;
                return true;
            }
        }
    }

    // if there is no start point on the borders, find it in other part of image
    for (int i = iBegin + 1; i < iEnd - 1; ++i)
        for (int j = jBegin + 1; j < jEnd - 1; ++j)
        {
            if (mask(i, j))
            {
                // this pixel is not noise
                int val = integral(i + 1, j + 1) - integral(i + 1, j - 2) -
                    integral(i - 2, j + 1) + integral(i - 2, j - 2);

                static const int kThreshold1 = 9;
                if (val >= kThreshold1)
                {
                    m_iStart = i - 1;
                    m_jStart = j - 1;
                    return true;
                }
            }
        }

    return false;
}

void ContourDetector::detourContour(const Matrix<unsigned char>& mask, int maskBorder)
{
    // i - rows; j - cols
    const int iBegin = maskBorder;
    const int jBegin = maskBorder;
    const int iEnd = mask.rows() - maskBorder;
    const int jEnd = mask.cols() - maskBorder;

    int ii = m_iStart;
    int jj = m_jStart;

    do
    {
        m_contour.push_back(Vector2D<int>(ii, jj));


    } while (ii != m_iStart && jj != m_jStart);
}

} // namespace core