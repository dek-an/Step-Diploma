#include "../include/core/ImageUtilites.h"

#include "../include/core/typedefs.h"

namespace core
{

void ImageUtilites::Yuv2Yuv420sp(const uchar* const yuvSrc, uchar* yuv420spDst,
    int width, int height)
{
    if (!yuvSrc)
        return;

    if (!(width * height))
        return;

    const int imgSize = width * height;
    const int srcDataSize = imgSize * 3;
    const int dstDataSize = srcDataSize >> 1;
    const int width3 = width * 3;
    const int widthHalf = width >> 1;

    yuv420spDst = new uchar[dstDataSize];

    // y data
    const uchar* srcIt = yuvSrc;
    const uchar* srcEnd = srcIt + srcDataSize;
    for (uchar* dstIt = yuv420spDst; srcIt != srcEnd; srcIt += 3, ++dstIt)
        *dstIt = *srcIt;

    // uv data
    srcIt = yuvSrc + 1;
    srcEnd = srcIt + srcDataSize;
    int cnt = 1;
    for (uchar* dstIt = yuv420spDst + imgSize; srcIt != srcEnd; srcIt += 6, dstIt += 2, ++cnt)
    {
        int uVal = *srcIt + *(srcIt + 3) + *(srcIt + width3) + *(srcIt + width3 + 3);
        uVal /= 4;
        int vVal = *(srcIt + 1) + *(srcIt + 1 + 3) + *(srcIt + 1 + width3) + *(srcIt + 1 + width3 + 3);
        vVal /= 4;

        *dstIt = uVal;
        *(dstIt + 1) = vVal;

        if (cnt % widthHalf == 0)
            srcIt += width3;
    }
}

} // namespace core