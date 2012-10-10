#include "../include/core/ContourDetector.h"

#include "../include/core/YUVImage.h"
#include "../include/core/geometry/Matrix.h"
#include "../include/core/geometry/Vector2D.h"

#include <memory>

namespace core
{

class ContourDetector::Pixel
{

////////////////////////////////////////////////////////////////
//
// Direction && Normal:
//     7  0  1
//     6  X  2
//     5  4  3
//
////////////////////////////////////////////////////////////////

static const int indI[8];
static const int indJ[8];

public:
    Pixel(int i, int j);

public:
    bool moveTo(int i, int j);
    bool moveToCurrentAround();
    void operator++();
    Pixel operator++(int);
    bool operator==(const Pixel& px) const;
    bool operator!=(const Pixel& px) const;
    operator Vector2D<int>() const;
    Vector2D<int> currentAround() const;

    inline int nextI() const { return m_iA; }
    inline int nextJ() const { return m_jA; }

private:
    void determineAroundStart(int i, int j);

private:
    // current indexes
    int m_i;
    int m_j;
    // around indexes
    int m_iA;
    int m_jA;
    int m_normal;
};

// ---------------------------------------------------------------------

const int ContourDetector::Pixel::indI[8] = { -1, -1, 0, 1, 1, 1, 0, -1};
const int ContourDetector::Pixel::indJ[8] = { 0, 1, 1, 1, 0, -1, -1, -1};

ContourDetector::Pixel::Pixel(int i, int j)
    : m_i(i)
    , m_j(j)
    // by default direction is right and normal is up
    , m_iA(i - 1)
    , m_jA(j)
    , m_normal(0)
{}

bool ContourDetector::Pixel::moveTo(int i, int j)
{
    if (i == m_i && j == m_j)
        return false;

    determineAroundStart(i, j);
    m_i = i;
    m_j = j;

    return true;
}

bool ContourDetector::Pixel::moveToCurrentAround()
{
    return moveTo(m_iA, m_jA);
}

void ContourDetector::Pixel::operator++()
{
    ++m_normal %= 8;
    m_iA = m_i + indI[m_normal];
    m_jA = m_j + indJ[m_normal];
}

ContourDetector::Pixel ContourDetector::Pixel::operator++(int)
{
    Pixel p(*this);
    ++(*this);
    return p;
}

bool ContourDetector::Pixel::operator==(const Pixel& px) const
{
    return (px.m_i == m_i && px.m_j == m_j);
}

bool ContourDetector::Pixel::operator!=(const Pixel& px) const
{
    return !(px == *this);
}

ContourDetector::Pixel::operator Vector2D<int>() const
{
    return Vector2D<int>(m_i, m_j);
}

Vector2D<int> ContourDetector::Pixel::currentAround() const
{
    return Vector2D<int>(m_iA, m_jA);
}

void ContourDetector::Pixel::determineAroundStart(int i, int j)
{
    int direction = 0;

    int iR = i - m_i;
    int jR = j - m_j;

    if (iR > 0)
        direction = 4 - jR;
    else if (iR < 0)
        direction = (8 + jR) % 8;
    else
        direction = 4 - 2 * jR;

    m_normal = direction - 2;

    // for start around point
    m_iA = i + indI[m_normal];
    m_jA = j + indJ[m_normal];
}

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

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

    const Pixel start(m_iStart, m_jStart);
    Pixel px(m_iStart, m_jStart);

    do
    {
        m_contour.push_back(px);
        for (; !mask(px.currentAround()); ++px);
        px.moveToCurrentAround();
    } while ( px != start );
}

} // namespace core