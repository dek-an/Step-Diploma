#include "../include/core/IntegralImage.h"

namespace core
{

IntegralImage::IntegralImage(int width, int height)
    : m_width(width)
    , m_height(height)
    , m_integral(height, width)
    , m_maxVal(0)
    , m_iMax(0)
    , m_jMax(0)
{}

IntegralImage::~IntegralImage()
{}


int IntegralImage::operator()(int i, int j) const
{
    return m_integral(i, j);
}

void IntegralImage::set(int i, int j, int val)
{

    m_integral.set(i, j, val);

    if (m_maxVal < val)
    {
        m_maxVal = val;
        m_iMax = i;
        m_jMax = j;
    }
}

void IntegralImage::addVal(int i, int j, int val)
{
    val = val ? 1 : 0;

    if (!i && j)
        val += m_integral(i, j - 1);
    else if (i && !j)
        val += m_integral(i - 1, j);
    else if (i && j)
        val += m_integral(i, j - 1) + m_integral(i - 1, j) - m_integral(i - 1, j - 1);

    set(i, j, val);
}

const MatrixInt& IntegralImage::getRaw() const
{
    return m_integral;
}

int IntegralImage::sumInSquare(int iTL, int jTL, int iBR, int jBR) const
{
    iTL = iTL ? iTL - 1 : 0;
    jTL = jTL ? jTL - 1 : 0;

    return m_integral(iBR, jBR) - m_integral(iBR, jTL) - m_integral(iTL, jBR) + m_integral(iTL, jTL);
}

} // namespace core