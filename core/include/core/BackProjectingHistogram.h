#ifndef HGE_BACKPROJECTINGHISTOGRAM_H_
#define HGE_BACKPROJECTINGHISTOGRAM_H_

#include <mvg/Size.h>
#include <mvg/Rectangle.h>
#include <mvg/matrix.h>


namespace core
{

class BackProjectingHistogram
{
    typedef float HistDataType;

public:
    BackProjectingHistogram();
    ~BackProjectingHistogram();

public:
    bool    isTraining() const;
    bool    isNeededTraining() const;
    bool    trainSuccessful() const;

    void    setTrainDuration(int framesNum);
    void    setTrainWndSize(int wndSize);
    void    setHistThreshold(float thr);

    bool    trainWithNext(const unsigned char* yuv420sp, int width, int height);
    bool    process(const unsigned char* yuv420sp, int width, int height);

    const mvg::matrix<unsigned char>& getMask() const;

private:
    void    update(const unsigned char* yuv420sp, int width, int height);
    void    calcHistogram(mvg::matrix<HistDataType>& hist,
                            bool relative = true,
                            const mvg::Rect& roi = mvg::Rect());
    void    calcCurrentHistogram();
    void    calcRatioHistogram();
    void    makeMask();

private:
    BackProjectingHistogram(const BackProjectingHistogram&);
    BackProjectingHistogram& operator=(const BackProjectingHistogram&);

private:
    mvg::Size                   mImgSize;
    int                         mTrainDuration;     // in frames
    int                         mTrainHalfWndSize;  // half of train window square side
    mvg::Rect                   mTrainWnd;
    bool                        mTrainStarted;
    bool                        mProcessingStarted;
    float                       mHistThreshold;
    const unsigned char*        mUVData;
    mvg::matrix<HistDataType>   mModelHistogram;
    mvg::matrix<HistDataType>   mCurrentHistogram;
    mvg::matrix<float>          mRatioHistogram;
    mvg::matrix<unsigned char>  mMask;
};

} // namespace hge

#endif // HGE_BACKPROJECTINGHISTOGRAM_H_