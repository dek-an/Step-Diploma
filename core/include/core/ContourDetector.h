#ifndef CORE_CONTOURDETECTOR_H_
#define CORE_CONTOURDETECTOR_H_

namespace core
{

class YUVImage;

class ContourDetector
{
public:
    typedef bool (*BinarizationFunction)(unsigned char y, unsigned char u, unsigned char v);

public:
    ContourDetector(BinarizationFunction binFunc);
    ~ContourDetector();

public:
    void detect(YUVImage* image);

private:
    void setSize(int width, int height);
    void reset();

private:
    ContourDetector(const ContourDetector&);
    ContourDetector& operator=(const ContourDetector&);

private:
    BinarizationFunction    m_binFunc;
    int                     m_width;
    int                     m_height;
    int                     m_scale;
};

} // namespace core

#endif // CORE_CONTOURDETECTOR_H_