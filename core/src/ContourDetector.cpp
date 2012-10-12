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
    Pixel();

public:
    bool moveTo(int i, int j);
    bool moveToCurrentAround();
    Vector2D<int> currentAround() const;

    void operator++();
    Pixel operator++(int);
    Pixel operator+=(const Vector2D<int>& v);
    bool operator==(const Pixel& px) const;
    bool operator!=(const Pixel& px) const;
    operator Vector2D<int>() const;
    Pixel& operator=(const Pixel& p);

    inline int iCurrent() const { return m_i; }
    inline int jCurrent() const { return m_j; }
    inline int iPrev() const { return m_iPrev; }
    inline int jPrev() const { return m_jPrev; }
    inline int iShift() const { return m_iShift; }
    inline int jShift() const { return m_jShift; }
    inline int iAround() const { return m_iA; }
    inline int jAround() const { return m_jA; }

private:
    void determineAroundStart(int i, int j);

    void swap(Pixel& p);

private:
    // current indexes
    int m_i;
    int m_j;
    // around indexes
    int m_iA;
    int m_jA;
    // previous indexes
    int m_iPrev;
    int m_jPrev;
    // shift relatively previous position
    int m_iShift;
    int m_jShift;
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
    , m_iPrev(i)
    , m_jPrev(j - 1)
    , m_iShift(0)
    , m_jShift(1)
    , m_normal(0)
{}

bool ContourDetector::Pixel::moveTo(int i, int j)
{
    if (i == m_i && j == m_j)
        return false;

    determineAroundStart(i, j);
    m_iPrev = m_i;
    m_jPrev = m_j;
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

ContourDetector::Pixel ContourDetector::Pixel::operator+=(const Vector2D<int>& v)
{
    moveTo(v.x(), v.y());
    return *this;
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

ContourDetector::Pixel& ContourDetector::Pixel::operator=(const Pixel& p)
{
    if (&p != this)
        Pixel(p).swap(*this);
    return *this;
}

Vector2D<int> ContourDetector::Pixel::currentAround() const
{
    return Vector2D<int>(m_iA, m_jA);
}

void ContourDetector::Pixel::determineAroundStart(int i, int j)
{
    int direction = 0;

    int iR = i - m_i;
    if (iR)
        iR = iR < 0 ? -1 : 1;
    int jR = j - m_j;
    if (jR)
        jR = jR < 0 ? -1 : 1;

    if (iR > 0)
        direction = 4 - jR;
    else if (iR < 0)
        direction = (8 + jR) % 8;
    else
        direction = 4 - 2 * jR;

    m_normal = (8 + direction - 2) % 8;

    // for start around point
    m_iA = i + indI[m_normal];
    m_jA = j + indJ[m_normal];

    // shift
    m_iShift = iR;
    m_jShift = jR;
}

//ContourDetector::Pixel ContourDetector::Pixel::operator+(const Pixel& p)
//{
//    Pixel res(*this);
//    res += p;
//    return res;
//}

void ContourDetector::Pixel::swap(Pixel& p)
{
    int i = m_i; int j = m_j;
    int iA = m_iA; int jA = m_jA;
    int iP = m_iPrev; int jP = m_jPrev;
    int iS = m_iShift; int jS = m_jShift;
    int n = m_normal;

    this->m_i = p.m_i; this->m_j = p.m_j;
    this->m_iA = p.m_iA; this->m_jA = p.m_jA;
    this->m_iPrev = p.m_iPrev; this->m_jPrev = p.m_jPrev;
    this->m_iShift = p.m_iShift; this->m_jShift = p.m_jShift;
    this->m_normal;

    p.m_i = i; p.m_j = j;
    p.m_iA = iA; p.m_jA = jA;
    p.m_iPrev = iP; p.m_jPrev = jP;
    p.m_iShift = iS; p.m_jShift = jS;
    p.m_normal = n;
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
    , m_activeArea(0)
    , m_iStart(0)
    , m_jStart(0)
    , m_contourFounded(false)
    , m_maskBorder(0)
    , m_iBegin(0)
    , m_jBegin(0)
    , m_iEnd(0)
    , m_jEnd(0)
{
}

ContourDetector::~ContourDetector(void)
{
}


void ContourDetector::detect(YUVImage& image)
{
    reset();
    setValues(image);

    image.doBinaryMask(m_binFunc);
    const Matrix<unsigned char>& mask = image.getBinaryMask();
    const Matrix<int>& integral = image.getIntegralMask();

    int detectedArea = integral(m_height - 1, m_width - 1);
    int border = image.getMaskBorder();
    if (detectedArea * kSmallAreaKoef < m_activeArea)
        return;

    findContour(mask, integral, image.getMaskBorder());
}


void ContourDetector::setValues(const YUVImage& image)
{
    int maskBorder = image.getMaskBorder();
    int width = image.width();
    int height = image.height();

    if ( width != m_width || height != m_height || m_maskBorder != maskBorder)
    {
        m_width = width;
        m_height = height;
        m_imageArea = width * height;
        int borderArea = 2 * maskBorder * (width + height - 2 * maskBorder);
        m_activeArea = m_imageArea - borderArea;
    }
}

void ContourDetector::reset(void)
{
    m_iStart = 0;
    m_jStart = 0;
    m_contourFounded = false;
    m_contour.clear();
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
    const Pixel start(m_iStart, m_jStart);
    Pixel px(m_iStart, m_jStart);

    const int iBegin = maskBorder;
    const int jBegin = maskBorder;
    const int iEnd = mask.rows() - maskBorder;
    const int jEnd = mask.cols() - maskBorder;

    // borders on the previous step
    bool tPrevBorder = false;    // top previous border
    bool bPrevBorder = false;    // bottom previous border
    bool lPrevBorder = false;    // left previous border
    bool rPrevBorder = false;    // right previous border

    do
    {
        m_contour.push_back(px);

        // in case if the set of pixels is on the border
        // we have to skip it all while such situation
        int iCurrent = px.iCurrent();
        int jCurrent = px.jCurrent();

        // current borders
        bool tCurrBorder = (iBegin == iCurrent);    // current top border
        bool bCurrBorder = (iEnd - 1 == iCurrent);  // current bottom border
        bool lCurrBorder = (jBegin == jCurrent);    // current left border
        bool rCurrBorder = (jEnd - 1 == jCurrent);  // current right border

        // borders (if previous and current are true)
        bool tBorder = (tCurrBorder && tPrevBorder);  // current top border
        bool bBorder = (bCurrBorder && bPrevBorder);  // current bottom border
        bool lBorder = (lCurrBorder && lPrevBorder);  // current left border
        bool rBorder = (rCurrBorder && rPrevBorder);  // current right border

        bool border = (tBorder || bBorder || lBorder || rBorder);   // any border

        // check corners
        bool ltCorner = lBorder && tBorder; // left top corner
        bool rtCorner = rBorder && tBorder; // right top corner
        bool lbCorner = lBorder && bBorder; // left bottom corner
        bool rbCorner = rBorder && bBorder; // right bottom corner

        bool corner = ( border && (ltCorner || rtCorner || lbCorner || rbCorner) );   // any corner

        if (!corner && border)
        {
            int iInd = iCurrent;
            int jInd = jCurrent;
            // because of two pixels means that we moving on the border:
            int iShift = px.iShift();
            int jShift = px.jShift();

            while ( mask(px + Vector2D<int>(2 * iShift, 2 * jShift)) )
            {
                iInd += iShift;
                jInd += jShift;
                px.moveTo(iInd, jInd);
            }

            tCurrBorder = false;
            bCurrBorder = false;
            lCurrBorder = false;
            rCurrBorder = false;
        }
        else
        {
            while ( !mask(px.currentAround()) )
                ++px;
            px.moveToCurrentAround();
        }

        tPrevBorder = tCurrBorder;
        bPrevBorder = bCurrBorder;
        lPrevBorder = lCurrBorder;
        rPrevBorder = rCurrBorder;
    } while ( px != start );
    m_contourFounded = true;
}

} // namespace core