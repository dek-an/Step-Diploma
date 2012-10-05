#ifndef CORE_YUVIMAGE_H_
#define CORE_YUVIMAGE_H_

#include "geometry/Matrix.h"

namespace core
{

class YUVImage
{
public:
    typedef bool (*BinarizationFunction)(unsigned char y, unsigned char u, unsigned char v);

public:
    class iterator
    {
        friend class YUVImage;
        typedef const unsigned char DataType;

    private:
        iterator(const unsigned char* data, int width, int height, int divisor, int step);

    public:
        unsigned char operator*();
        iterator operator+=(int shift);
        void operator++();
        iterator operator++(int);
        bool operator==(const iterator& it);
        bool operator!=(const iterator& it);

        friend iterator operator+(const iterator& it, int shift);

    private:
        void movePtr();

    private:
        DataType* const     m_ptr;
        DataType*           m_currPtr;
        int                 m_width;
        int                 m_height;
        int                 m_divisor;
        int                 m_step;
        int                 m_i;
        int                 m_j;
    };

public:
    enum ImageFormat
    {
        GRAY,
        YUV,
        YUV420SP
    };

public:
    YUVImage(ImageFormat format, int width, int height, const unsigned char* data);
    ~YUVImage();

public:
    int width() const;
    int height() const;

    unsigned char operator()(char comp, int row, int col) const;

    const unsigned char* data() const;

public:
    YUVImage* doBinarizeImage(BinarizationFunction binFunc);
    void smoothBinarizeImage(int radius);

public: // iterators
    iterator beginY() const;
    iterator endY() const;
    iterator beginU() const;
    iterator endU() const;
    iterator beginV() const;
    iterator endV() const;

private:
    void applyFormat();

private:
    YUVImage(const YUVImage&);
    YUVImage& operator=(const YUVImage&);

private:
    ImageFormat             m_format;
    int                     m_width;
    int                     m_height;
    int                     m_dataSize;
    const unsigned char*    m_yData;
    const unsigned char*    m_uData;
    const unsigned char*    m_vData;
    const unsigned char*    m_yEnd;
    const unsigned char*    m_uEnd;
    const unsigned char*    m_vEnd;
    int                     m_yDivisor; // for iterator
    int                     m_uDivisor; // for iterator
    int                     m_vDivisor; // for iterator
    int                     m_yStep;    // for iterator
    int                     m_uStep;    // for iterator
    int                     m_vStep;    // for iterator
    bool                    m_dataOwner;

    // for binarization
    Matrix<unsigned char>   m_binarize;
    Matrix<int>             m_integral;
};

typedef YUVImage::iterator YUVImageIterator;

} // namespace core

#endif // CORE_YUVIMAGE_H_