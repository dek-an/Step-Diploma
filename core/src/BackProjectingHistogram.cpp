#include "../include/core/BackProjectingHistogram.h"

namespace core
{

static const int    kDefTrainDuration       = 300;  // in frames
static const int    kDefTrainHalfWndSize    = 100;
static const float  kDefHistThreshold       = 0.8f;
static const int    kHistSize               = 256;
static const int    kHistDataSize           = kHistSize * kHistSize;


BackProjectingHistogram::BackProjectingHistogram()
    : mImgSize(0, 0)
    , mTrainDuration(kDefTrainDuration)
    , mTrainHalfWndSize(kDefTrainHalfWndSize)
    , mTrainWnd(0, 0, 0, 0)
    , mTrainStarted(false)
    , mProcessingStarted(false)
    , mHistThreshold(kDefHistThreshold)
    , mUVData(0)
{
    mModelHistogram.resize(kHistSize, kHistSize);
    mCurrentHistogram.resize(kHistSize, kHistSize);
    mRatioHistogram.resize(kHistSize, kHistSize);
}

BackProjectingHistogram::~BackProjectingHistogram()
{}


bool BackProjectingHistogram::isTraining() const
{
    return mTrainStarted;
}

bool BackProjectingHistogram::isNeededTraining() const
{
    return !mTrainStarted && !mProcessingStarted;
}

bool BackProjectingHistogram::trainSuccessful() const
{
    return !mTrainStarted && mProcessingStarted;
}


void BackProjectingHistogram::setTrainDuration(int framesNum)
{
    mTrainDuration = framesNum;
}

void BackProjectingHistogram::setTrainWndSize(int wndSize)
{
    mTrainHalfWndSize = wndSize >> 1;
}

void BackProjectingHistogram::setHistThreshold(float thr)
{
    mHistThreshold = thr;
}


bool BackProjectingHistogram::trainWithNext(const unsigned char* yuv420sp, int width, int height)
{
    if (trainSuccessful())
        return false;

    update(yuv420sp, width, height);

    mTrainStarted = true;
    calcHistogram(mModelHistogram, false, mTrainWnd);

    static int trainFrameCnt = 0;
    if (++trainFrameCnt >= mTrainDuration)
    {
        mTrainStarted = false;
        mProcessingStarted = true;

        const float inv = 1.f / (mTrainDuration * mTrainWnd.area());
        for (HistDataType *data = mModelHistogram.raw(), *dataEnd = data + kHistDataSize;
            data != dataEnd; ++data)
            *data *= inv;
    }

    return true;
}

bool BackProjectingHistogram::process(const unsigned char* yuv420sp, int width, int height)
{
    if (!trainSuccessful())
        return false;

    update(yuv420sp, width, height);
    calcCurrentHistogram();
    calcRatioHistogram();
    makeMask();

    return true;
}


const mvg::matrix<unsigned char>& BackProjectingHistogram::getMask() const
{
    return mMask;
}


void BackProjectingHistogram::update(const unsigned char* yuv420sp, int width, int height)
{
    ASSERT_CONVERTIBLE(width, uint);  ASSERT_CONVERTIBLE(height, uint);
    const uint uWidth = (uint)width;
    const uint uHeight = (uint)height;

    mUVData = yuv420sp + width * height;

    if (mImgSize.width() == uWidth && mImgSize.height() == uHeight)
        return;

    mImgSize.setSize(uWidth, uHeight);
    mMask.resize(width, height);

    const mvg::Point center(short(width >> 1), short(height >> 1));
    const mvg::Point shift((short)mTrainHalfWndSize, (short)mTrainHalfWndSize);
    mTrainWnd.set(center - shift, center + shift);
}

void BackProjectingHistogram::calcHistogram(mvg::matrix<HistDataType>& hist,
                                            bool relative /*= true*/,
                                            const mvg::Rect& roi /*= mvg::Rect()*/)
{
    int uvStartCol = roi.left() >> 1;
    int uvStartRow = roi.top() >> 1;
    int uvEndCol = roi.right() >> 1;
    int uvEndRow = roi.bottom() >> 1;

    const int width = mImgSize.width();

    if (!roi.area())
    {
        uvStartCol = 0;
        uvStartRow = 0;
        uvEndCol = width >> 1;
        uvEndRow = mImgSize.height() >> 1;
    }

    for (int i = uvStartRow; i < uvEndRow; ++i)
        for (int j = uvStartCol; j < uvEndCol; ++j)
        {
            const int currentShift = i * width + (j * 2);
            const unsigned char u = *(mUVData + currentShift);
            const unsigned char v = *(mUVData + currentShift + 1);
            ++hist[u][v];
        }

    if (relative)
    {
        const int area = (uvEndCol - uvStartCol) * (uvEndRow - uvStartRow);
        const float inv = 1.f / area;
        for (HistDataType *data = hist.raw(), *dataEnd = data + kHistDataSize;
            data != dataEnd; ++data)
            *data *= inv;
    }
}

void BackProjectingHistogram::calcCurrentHistogram()
{
    memset(mCurrentHistogram.raw(), 0, kHistDataSize * sizeof(HistDataType));
    calcHistogram(mCurrentHistogram);
}

void BackProjectingHistogram::calcRatioHistogram()
{
    const HistDataType* model = mModelHistogram.raw();
    const HistDataType* cur = mCurrentHistogram.raw();

    for (float *ratio = mRatioHistogram.raw(), *end = ratio + kHistDataSize;
        ratio != end; ++ratio, ++model, ++cur)
    {
        const HistDataType curVal = *cur;
        // this correct comparison for float
        *ratio = mvg::isZero(curVal) ? 1.f : std::min(1.f, float(*model) / curVal);
    }
}

void BackProjectingHistogram::makeMask()
{
    const int width = mImgSize.width();

    const int uvWidth = width >> 1;
    const int uvHeight = mImgSize.height() >> 1;

    const unsigned char* uvData = mUVData;
    unsigned char* maskData = mMask.raw();

    for (int uvRow = 0; uvRow < uvHeight; ++uvRow)
        for (int uvCol = 0; uvCol < uvWidth; ++uvCol, uvData += 2)
        {
            const unsigned char u = *uvData;
            const unsigned char v = *(uvData + 1);

            // upper left pixel of quarto square
            const int imgRow = 2 * uvRow;
            const int imgCol = 2 * uvCol;
            const int imgShift = imgRow * width + imgCol;

            const unsigned char maskPixel = (mRatioHistogram[u][v] > mHistThreshold) ? 255 : 0;

            unsigned char* mP = maskData + imgShift; // mask pixel pointer
            memset(mP, maskPixel, 2 * sizeof(unsigned char));
            memset(mP + width, maskPixel, 2 * sizeof(unsigned char));
        }
}

} // namespace core
