#include <core/YUVImage.h>
#include <core/ContourDetector.h>
#include "core/../../src/ContourDetector.cpp"
#include <core/Contour.h>
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
void SimpleContourDetectorTest();
void BordersContourDetectorTest();
void PixelTest();

void main()
{
    //YUVImageTest();
    //YUVImageBinaryIntegralTest();
    //SimpleContourDetectorTest();
    BordersContourDetectorTest();
    //PixelTest();
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
    print<int>(img.getIntegralMask().getRaw());

    delete[] data;
}

void SimpleContourDetectorTest()
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

    const Contour& cont = cd->getContour();
    Contour::ContourIterator begin = cont.begin();
    Contour::ContourIterator end = cont.end();
    for (Contour::ContourIterator it = begin; it != end; ++it)
        *(data + it->x() * width + it->y()) = 33;

    print(data, width, height);

    delete[] data;
}

void BordersContourDetectorTest()
{
    int width = 15;
    int height = 15;
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

    // dataSize = 225 (15x15)
    unsigned char data1[225] =
    {
        2, 2,   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,   2, 2,
        2, 2,   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,   2, 2,

        2, 2,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,   2, 2,
        2, 2,   1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0,   2, 2,
        2, 2,   1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0,   2, 2,
        2, 2,   1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0,   2, 2,
        2, 2,   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,   2, 2,
        2, 2,   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   2, 2,
        2, 2,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   2, 2,
        2, 2,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   2, 2,
        2, 2,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   2, 2,
        2, 2,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   2, 2,
        2, 2,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   2, 2,

        2, 2,   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,   2, 2,
        2, 2,   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,   2, 2
    };

    unsigned char data2[225] =
    {
        2, 0,   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,   2, 2,
        0, 1,   2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,

        2, 2,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   2, 2,
        2, 3,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   2, 2,
        2, 4,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   2, 2,
        2, 5,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   2, 2,
        2, 6,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   2, 2,
        2, 7,   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   2, 2,
        2, 8,   1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,   2, 2,
        2, 9,   1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,   2, 2,
        2, 10,  1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,   2, 2,
        2, 11,  0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,   2, 2,
        2, 12,  0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,   2, 2,

        2, 13,  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,   2, 2,
        2, 14,  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,   2, 2
    };

    print(data2, width, height);

    YUVImage img(format, width, height, data2);
    img.setMaskBorder(2);

    ContourDetector cd(simpleBin);
    cd.detect(img);

    unsigned char outData[255];
    for (int i = 0; i < 255; ++i)
        outData[i] = 0;

    Contour cont = cd.getContour();
    Contour::ContourIterator begin = cont.begin();
    Contour::ContourIterator end = cont.end();
    for (Contour::ContourIterator it = begin; it != end; ++it)
        *(outData + it->x() * width + it->y()) = 2;

    print(outData, 15, 15);
}

void PixelTest()
{
    int i = 5;
    int j = 5;

    typedef ContourDetector::Pixel Pixel;
    Pixel p(i, j);
    printf("i = %i   j = %i\n\n", i, j);

    printf("i = %i   j = %i\n", p.iCurrent(), p.jCurrent());
    for (int i = 0; i < 8; ++i, ++p)
        printf("   current around i = %i   j = %i\n", p.iAround(), p.jAround());
    printf("prev i = %i   j = %i\nshift i = %i   j = %i\n", p.iPrev(), p.jPrev(), p.iShift(), p.jShift());

    printf("\nMOVING to i = %i, j = %i (right)\n\n", 5, 6);
    p.moveTo(5, 6);
    printf("i = %i   j = %i\n", p.iCurrent(), p.jCurrent());
    for (int i = 0; i < 8; ++i, ++p)
        printf("   current around i = %i   j = %i\n", p.iAround(), p.jAround());
    printf("prev i = %i   j = %i\nshift i = %i   j = %i\n", p.iPrev(), p.jPrev(), p.iShift(), p.jShift());

    printf("\nMOVING to i = %i, j = %i (left)\n\n", 5, 5);
    p.moveTo(5, 5);
    printf("i = %i   j = %i\n", p.iCurrent(), p.jCurrent());
    for (int i = 0; i < 8; ++i, ++p)
        printf("   current around i = %i   j = %i\n", p.iAround(), p.jAround());
    printf("prev i = %i   j = %i\nshift i = %i   j = %i\n", p.iPrev(), p.jPrev(), p.iShift(), p.jShift());

    printf("\nMOVING to i = %i, j = %i (down)\n\n", 6, 5);
    p.moveTo(6, 5);
    printf("i = %i   j = %i\n", p.iCurrent(), p.jCurrent());
    for (int i = 0; i < 8; ++i, ++p)
        printf("   current around i = %i   j = %i\n", p.iAround(), p.jAround());
    printf("prev i = %i   j = %i\nshift i = %i   j = %i\n", p.iPrev(), p.jPrev(), p.iShift(), p.jShift());

    printf("\nMOVING to i = %i, j = %i (up)\n\n", 5, 5);
    p.moveTo(5, 5);
    printf("i = %i   j = %i\n", p.iCurrent(), p.jCurrent());
    for (int i = 0; i < 8; ++i, ++p)
        printf("   current around i = %i   j = %i\n", p.iAround(), p.jAround());
    printf("prev i = %i   j = %i\nshift i = %i   j = %i\n", p.iPrev(), p.jPrev(), p.iShift(), p.jShift());

    printf("\nMOVING to i = %i, j = %i (u-l)\n\n", 4, 4);
    p.moveTo(4, 4);
    printf("i = %i   j = %i\n", p.iCurrent(), p.jCurrent());
    for (int i = 0; i < 8; ++i, ++p)
        printf("   current around i = %i   j = %i\n", p.iAround(), p.jAround());
    printf("prev i = %i   j = %i\nshift i = %i   j = %i\n", p.iPrev(), p.jPrev(), p.iShift(), p.jShift());

    printf("\nMOVING to i = %i, j = %i (d-r)\n\n", 5, 5);
    p.moveTo(5, 5);
    printf("i = %i   j = %i\n", p.iCurrent(), p.jCurrent());
    for (int i = 0; i < 8; ++i, ++p)
        printf("   current around i = %i   j = %i\n", p.iAround(), p.jAround());
    printf("prev i = %i   j = %i\nshift i = %i   j = %i\n", p.iPrev(), p.jPrev(), p.iShift(), p.jShift());

    printf("\nMOVING to i = %i, j = %i (u-r)\n\n", 4, 6);
    p.moveTo(4, 6);
    printf("i = %i   j = %i\n", p.iCurrent(), p.jCurrent());
    for (int i = 0; i < 8; ++i, ++p)
        printf("   current around i = %i   j = %i\n", p.iAround(), p.jAround());
    printf("prev i = %i   j = %i\nshift i = %i   j = %i\n", p.iPrev(), p.jPrev(), p.iShift(), p.jShift());

    printf("\nMOVING to i = %i, j = %i (d-l)\n\n", 5, 5);
    p.moveTo(5, 5);
    printf("i = %i   j = %i\n", p.iCurrent(), p.jCurrent());
    for (int i = 0; i < 8; ++i, ++p)
        printf("   current around i = %i   j = %i\n", p.iAround(), p.jAround());
    printf("prev i = %i   j = %i\nshift i = %i   j = %i\n", p.iPrev(), p.jPrev(), p.iShift(), p.jShift());
}