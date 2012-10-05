#include <core/YUVImage.h>
#include <iostream>

using namespace std;

using core::YUVImage;

void YUVImageTest();

void main()
{
    YUVImageTest();
}

void YUVImageTest()
{
    YUVImage::ImageFormat format = YUVImage::YUV;
    int width(6);
    int height(4);
    int dataSize(3 * width * height);
    switch (format)
    {
    case YUVImage::YUV:
        //dataSize = 3 * width * height;
        break;
    case YUVImage::YUV420SP:
        dataSize >>= 1;
        break;
    }
    unsigned char* data = new unsigned char[dataSize];

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