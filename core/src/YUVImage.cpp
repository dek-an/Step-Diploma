#include "../include/core/YUVImage.h"

#include <cassert>
#include <memory>

namespace core
{

// ----------------------------------------------------------
// -------------- YUVImage::iterator ------------------------
// ----------------------------------------------------------

YUVImage::iterator::iterator(const unsigned char* data, int width, int height, int divisor, int step)
    : m_ptr(data)
    , m_currPtr(data)
    , m_width(width)
    , m_height(height)
    , m_divisor(divisor)
    , m_step(step)
    , m_i(0)
    , m_j(0)
{
}

unsigned char YUVImage::iterator::operator*()
{
    return *m_currPtr;
}

YUVImage::iterator YUVImage::iterator::operator+=(int nShift)
{
    m_j = (m_j + nShift) % m_width;
    m_i += nShift / m_width;
    assert(m_i < m_height);
    movePtr();
    return *this;
}

void YUVImage::iterator::operator++()
{
    ++m_j %= m_width;
    if (!m_j)
        ++m_i;
    movePtr();
}

YUVImage::iterator YUVImage::iterator::operator++(int)
{
    iterator it(*this);
    ++(*this);
    return it;
}

bool YUVImage::iterator::operator==(const YUVImage::iterator& it)
{
    return (it.m_currPtr == m_currPtr);
}

bool YUVImage::iterator::operator!=(const YUVImage::iterator& it)
{
    return (it.m_currPtr != m_currPtr);
}


YUVImage::iterator operator+(const YUVImage::iterator& it, int shift)
{
    YUVImage::iterator res(it);
    res += shift;
    return res;
}


void YUVImage::iterator::movePtr()
{
    int shift = ((m_i / m_divisor) * (m_width / m_divisor) + m_j / m_divisor) * m_step;
    m_currPtr = m_ptr + shift;
}

// ----------------------------------------------------------
// ------------------------ YUVImage ------------------------
// ----------------------------------------------------------

YUVImage::YUVImage(ImageFormat format, int width, int height, const unsigned char* data/* = 0*/)
    : m_format(format)
    , m_width(width)
    , m_height(height)
    , m_dataSize(3 * width * height)
    , m_yData(data)
    , m_uData(data)
    , m_vData(data)
    , m_yEnd(data)
    , m_uEnd(data)
    , m_vEnd(data)
    , m_yDivisor(1)
    , m_uDivisor(1)
    , m_vDivisor(1)
    , m_yStep(1)
    , m_uStep(1)
    , m_vStep(1)
    , m_dataOwner(false)

    , m_binaryMask(height, width)
    , m_integral(height, width)
    , m_maskBorder(5)
{
    applyFormat();
}

YUVImage::~YUVImage()
{
    if (m_dataOwner)
        delete m_yData;
}


int YUVImage::width() const
{
    return m_width;
}

int YUVImage::height() const
{
    return m_height;
}


unsigned char YUVImage::operator()(char comp, int row, int col) const
{
    switch(comp)
    {
    case 'y':
    case 'Y':
        return *(beginY() + row * m_width + col);
        break;
    case 'u':
    case 'U':
        return *(beginU() + row * m_width + col);
        break;
    case 'v':
    case 'V':
        return *(beginV() + row * m_width + col);
        break;
    default:
        return 0;
    }
}


const unsigned char* YUVImage::data() const
{
    return m_yData;
}


void YUVImage::doBinaryMask(BinarizationFunction binFunc)
{
    //const int binDataSize = m_width *m_height;
    //unsigned char* binData = new unsigned char[binDataSize];

    const YUVImageIterator yBegin = beginY();
    const YUVImageIterator yEnd = endY();
    const YUVImageIterator uBegin = beginU();
    const YUVImageIterator uEnd = endU();
    const YUVImageIterator vBegin = beginV();
    const YUVImageIterator vEnd = endV();
    //const unsigned char* maskBegin = binData;
    //const unsigned char* maskEnd = maskBegin + binDataSize;

    YUVImageIterator yIt = yBegin;
    YUVImageIterator uIt = uBegin;
    YUVImageIterator vIt = vBegin;
    //unsigned char* maskIt = binData;

    int i = 0;
    int j = 0;
    for (; yIt != yEnd && uIt != uEnd && vIt != vEnd /*&& maskIt != maskEnd*/; ++yIt, ++uIt, ++vIt/*, ++maskIt*/)
    {
        int val = 0;

        if (i - m_maskBorder >= 0 && j - m_maskBorder >= 0 &&
            i + m_maskBorder < m_height && j + m_maskBorder < m_width) // if not borders (if borders, val = 0; clean borders)
        {
            if ( binFunc(*yIt, *uIt, *vIt) )
                val = 255;
            //*maskIt = val;
        }

        m_binaryMask.set(i, j, val);

        val %= 254;

        if (!i && j)
            val += m_integral(i, j - 1);
        else if (i && !j)
            val += m_integral(i - 1, j);
        else if (i && j)
            val += m_integral(i, j - 1) + m_integral(i - 1, j) - m_integral(i - 1, j - 1);
        m_integral.set(i, j, val);

        ++j %= m_width;
        if (!j)
            ++i;
    }

    //YUVImage* res = new YUVImage(GRAY, m_width, m_height, binData);
    //res->m_dataOwner = true;

    //return res;
}

void YUVImage::smoothBinaryMask(int radius/* = 1*/)
{
    int q = 1;
    const int b1 = 2;  // error on the first layer; edit
    for (int i = 0; i < radius; ++i)
        q *= 2;
    const int error = b1 * (q - 1); // sum of the geometric progression
    const int kThreshold = (radius + 2) * (radius + 2) - 1 - error;

    const int rows = m_binaryMask.rows();
    const int cols = m_binaryMask.cols();

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            // top left - (i1, j1); bottom right - (i2, j2).
            // i - row; j - column
            const int i1 = i - radius < 0 ? 0 : i - radius;
            const int i2 = i + radius >= rows ? rows - 1 : i + radius;
            const int j1 = j - radius < 0 ? 0 : j - radius;
            const int j2 = j + radius >= cols ? cols - 1 : j + radius;
            const int integralVal = m_integral(i2, j2) - m_integral(i1, j2)
                - m_integral(i2, j1) + m_integral(i1, j1);

            const int currentVal = m_binaryMask(i, j);
            if (currentVal && integralVal < error)
                m_binaryMask.set(i, j, 0);
            else if (!currentVal && integralVal >= kThreshold)
                m_binaryMask.set(i, j, 255);
        }
}


YUVImage::iterator YUVImage::beginY() const
{
    return iterator(m_yData, m_width, m_height, m_yDivisor, m_yStep);
}

YUVImage::iterator YUVImage::endY() const
{
    return iterator(m_yEnd, m_width, m_height, m_yDivisor, m_yStep);
}

YUVImage::iterator YUVImage::beginU() const
{
    return iterator(m_uData, m_width, m_height, m_uDivisor, m_uStep);
}

YUVImage::iterator YUVImage::endU() const
{
    return iterator(m_uEnd, m_width, m_height, m_uDivisor, m_uStep);
}

YUVImage::iterator YUVImage::beginV() const
{
    return iterator(m_vData, m_width, m_height, m_vDivisor, m_vStep);
}

YUVImage::iterator YUVImage::endV() const
{
    return iterator(m_vEnd, m_width, m_height, m_vDivisor, m_vStep);
}


void YUVImage::applyFormat()
{
    switch (m_format)
    {
    case GRAY:
        m_dataSize /= 3; // = m_width * m_height;
        if (!m_uData)
            m_uData = new unsigned char[m_dataSize];

        m_uData = m_yData;
        m_vData = m_yData;

        m_yEnd = m_yData + m_dataSize;
        m_uEnd = m_uData + m_dataSize;
        m_vEnd = m_vData + m_dataSize;

        break;
    case YUV:
        //m_dataSize = 3 * width * height;

        if (!m_uData)
            m_uData = new unsigned char[m_dataSize];

        m_uData = m_yData + 1;
        m_vData = m_uData + 1;

        m_yEnd = m_yData + m_dataSize;
        m_uEnd = m_yEnd + 1;
        m_vEnd = m_uEnd + 1;

        // m_yDivisor = 1;
        // m_uDivisor = 1;
        // m_vDivisor = 1;

        m_yStep = 3;
        m_uStep = 3;
        m_vStep = 3;

        break;
    case YUV420SP:
        m_dataSize >>= 1;   // = (3 * m_width * m_height) >> 1;

        if (!m_uData)
            m_uData = new unsigned char[m_dataSize];

        const int imgSize = m_width * m_height;
        const int imgSizeDiv4 = imgSize >> 2;
        m_uData = m_yData + imgSize;
        m_vData = m_uData + imgSizeDiv4;

        m_yEnd = m_uData;
        m_uEnd = m_vData;
        m_vEnd = m_vData + imgSizeDiv4;

        //m_yDivisor = 1;
        m_uDivisor = 2;
        m_vDivisor = 2;

        //m_yStep = 1;
        //m_uStep = 1;
        //m_vStep = 1;

        break;
    }
}

} // namespace core