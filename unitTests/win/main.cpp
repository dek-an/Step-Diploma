#include <core/YUVImage.h>
#include <core/ContourDetector.h>
#include <iostream>

using namespace std;
using namespace core;

template <typename T> void print(const Matrix<T>& m);
void print(const unsigned char* data, int width, int height);

static int width = 0;
static int height = 0;
static YUVImage::ImageFormat format = YUVImage::GRAY;
static int dataSize = 0;
unsigned char* data = 0;

void YUVImageTest();
void YUVImageBinaryIntegralTest();
void ContourDetectorTest();

void main()
{
    //YUVImageTest();
    //YUVImageBinaryIntegralTest();
    ContourDetectorTest();
}

bool simpleBin(unsigned char y, unsigned char, unsigned char)
{
    return y;
}

template <typename T> void print(const Matrix<T>& m)
{
    int rows = m.rows();
    int cols = m.cols();
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            printf("%3i ", m(i, j));
        printf("\n");
    }
    printf("\n");
}

void print(const unsigned char* data, int width, int height)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
            printf("%3i ", *(data + i * width + j));
        printf("\n");
    }
    printf("\n");
}

void YUVImageTest()
{
    format = YUVImage::YUV;
    width = 6;
    height = 4;
    dataSize = 3 * width * height;
    switch (format)
    {
    case YUVImage::YUV:
        //dataSize = 3 * width * height;
        break;
    case YUVImage::YUV420SP:
        dataSize >>= 1;
        break;
    }
    data = new unsigned char[dataSize];

    for (int i = 0; i < dataSize; ++i)
    {
        cout << i << ' ';
        data[i] = i;
    }

    cout << endl << endl;

    YUVImage* img = new YUVImage(format, width, height, data);
    YUVImage::iterator beginY = img->beginY();
    YUVImage::iterator beginU = img->beginU();
    YUVImage::iterator beginV = img->beginV();
    YUVImage::iterator endY = img->endY();
    YUVImage::iterator endU = img->endU();
    YUVImage::iterator endV = img->endV();
    for (YUVImage::iterator it = beginV; it != endV; ++it)
        cout << (int)*it << ' ';
}

void YUVImageBinaryIntegralTest()
{
    width = 10;
    height = 10;
    format = YUVImage::GRAY;
    switch (format)
    {
    case YUVImage::YUV:
        dataSize = 3 * width * height;
        break;
    case YUVImage::YUV420SP:
        dataSize = (3 * width * height) >> 1;
        break;
    case YUVImage::GRAY:
        dataSize = width * height;
        break;
    }
    data = new unsigned char[dataSize];
    for (int i = 0; i < dataSize; ++i)
        data[i] = i;
    print(data, width, height);

    YUVImage img(format, width, height, data);
    img.setMaskBorder(2);
    img.doBinaryMask(&simpleBin);
    print<unsigned char>(img.getBinaryMask());
    print<int>(img.getIntegralMask());

    delete[] data;
}

void ContourDetectorTest()
{
    int width = 10;
    int height = 10;
    YUVImage::ImageFormat format = YUVImage::GRAY;
    int dataSize = 3 * width * height;
    switch (format)
    {
    case YUVImage::YUV:
        //dataSize = 3 * width * height;
        break;
    case YUVImage::YUV420SP:
        dataSize >>= 1;
        break;
    case YUVImage::GRAY:
        dataSize /= 3;
        break;
    }
    unsigned char* data = new unsigned char[dataSize];
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            data[i * width + j] = i + j;

    YUVImage img(format, width, height, data);
    img.setMaskBorder(2);

    ContourDetector* cd = new ContourDetector(&simpleBin);
    cd->detect(img);

    typedef ContourDetector::ContourContainer Contour;
    const Contour& cont = cd->getContour();
    Contour::const_iterator begin = cont.begin();
    Contour::const_iterator end = cont.end();
    for (Contour::const_iterator it = begin; it != end; ++it)
        *(data + it->x() * width + it->y()) = 33;

    print(data, width, height);

    delete[] data;
}