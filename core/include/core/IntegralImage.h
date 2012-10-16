#ifndef CORE_INTEGRALIMAGE_H_
#define CORE_INTEGRALIMAGE_H_

#include "geometry/Matrix.h"

namespace core
{

class IntegralImage
{
public:
    IntegralImage(int width, int height);
    ~IntegralImage();

public:
    int operator()(int i, int j) const;
    void set(int i, int j, int val);    // TODO: private?
    void addVal(int i, int j, int val);
    const MatrixInt& getRaw() const;
    int sumInSquare(int iTL, int jTL, int iBR, int jBR) const;

    inline int iMax() const { return m_iMax; }
    inline int jMax() const { return m_jMax; }

private:
    int         m_height;
    int         m_width;
    MatrixInt   m_integral;
    int         m_maxVal;
    // min pos of max val
    int         m_iMax;
    int         m_jMax;
};

} // namespace core

#endif // CORE_INTEGRALIMAGE_H_