#ifndef CORE_CONTOURDETECTOR_H_
#define CORE_CONTOURDETECTOR_H_

#include "geometry/Vector2D.h"

#include <vector>


namespace core
{

template <typename T = float> class Matrix;
class YUVImage;

class ContourDetector
{
public:
    typedef bool (*BinarizationFunction)(unsigned char y, unsigned char u, unsigned char v);
    typedef std::vector< Vector2D<int> > ContourContainer;

public:
    ContourDetector(BinarizationFunction binFunc);
    ~ContourDetector(void);

public:
    void detect(YUVImage& image);

private:
    void setSize(int width, int height);
    void reset(void);
    void findContour(const Matrix<unsigned char>& mask, const Matrix<int>& integral, int maskBorder);
    bool findStartPoint(const Matrix<unsigned char>& mask, const Matrix<int>& integral, int maskBorder);
    void detourContour(const Matrix<unsigned char>& mask, int maskBorder);

private:
    ContourDetector(const ContourDetector&);
    ContourDetector& operator=(const ContourDetector&);

private:
    BinarizationFunction    m_binFunc;
    int                     m_width;
    int                     m_height;
    int                     m_imageArea;
    int                     m_iStart;
    int                     m_jStart;
    bool                    m_contourFounded;
    int                     m_maskBorder;
    ContourContainer        m_contour;

    class Pixel;
};

} // namespace core

#endif // CORE_CONTOURDETECTOR_H_