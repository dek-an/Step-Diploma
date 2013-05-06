#include "MainApplication.h"

#include <highgui.h>
#include <cv.h>

#include <core/YUVImage.h>
#include <core/gestures/GestureRecognitor.h>
#include <core/ContourDetector.h>

namespace application
{

MainApplication::MainApplication(void)
    : mYuvFrame(0)
    , mGrayResult(0)
{}

MainApplication::~MainApplication(void)
{
    if (mYuvFrame)
        cvReleaseImage(&mYuvFrame);

    if (mGrayResult)
        cvReleaseImage(&mGrayResult);
}


int MainApplication::run(void)
{
    //showHelloWorld();
    captureCamera();
    //processImage("logo-1-640x480.jpg");
    //processImage("kote-1-640x480.jpg");

    return 0;
}


void MainApplication::captureCamera(void)
{
    CvCapture* cam(cvCreateCameraCapture(CV_CAP_ANY));
    if (!cam)
    {
        printf("Error in camera initialization\n");
        return;
    }

    printf("CAMERA FORMAT: %f\n", cvGetCaptureProperty(cam, CV_CAP_PROP_BRIGHTNESS));

    // Creating of windows
    cvNamedWindow("CamWnd", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);

    IplImage* frameRGB = 0;

    for (;;)
    {
        frameRGB = cvQueryFrame(cam);

        cvShowImage("Original", frameRGB);
        frameCaptured(*frameRGB);

        if (mGrayResult)
            cvShowImage("CamWnd", mGrayResult);

        // waiting for esc
        if (cvWaitKey(33) == 27)
            break;
    }

    cvReleaseCapture(&cam);
    cvDestroyAllWindows();
}


void MainApplication::frameCaptured(const IplImage& frameRGB)
{
    //backProjectingMask(frameRGB);
    skinContourExtraction(frameRGB);
}


void MainApplication::backProjectingMask(const IplImage& frameRGB)
{
    
}

void MainApplication::skinContourExtraction(const IplImage& frameRGB)
{
    const int width = frameRGB.width;
    const int height = frameRGB.height;

    if (!mYuvFrame)
        mYuvFrame = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

    cvCvtColor(&frameRGB, mYuvFrame, CV_BGR2YUV);

    static core::ContourDetector cd(core::GestureRecognitor::skinBinarizationFunction);

    core::YUVImage img(core::YUVImage::YUV, width, height, (unsigned char*)mYuvFrame->imageData);

    cd.detect(img);

    if (!mGrayResult)
        mGrayResult = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);

    memset(mGrayResult->imageData, 0, width * height * sizeof(char));

    typedef core::Contour::ContourIterator ContourIter;
    core::Contour cont = cd.getContour();
    ContourIter begin = cont.begin();
    ContourIter end = cont.end();
    for (ContourIter it = begin; it != end; ++it)
        mGrayResult->imageData[it->x() * width + it->y()] = 255;
}

// -------------------- Debug ------------------------

void MainApplication::showHelloWorld(void)
{
    int height = 600;
    int width = 800;

    CvPoint pt = cvPoint(height/4, width/2);
    IplImage* hw = cvCreateImage(cvSize(height, width), 8, 3);

    cvSet(hw, cvScalar(0, 0, 0));

    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 1.0, 1.0, 0, 1, CV_AA);
    cvPutText(hw, "Hello World!!!!", pt, &font, CV_RGB(150, 0, 150) );

    cvNamedWindow("Hello World", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("One more", 0);
    cvShowImage("Hello World", hw);
    cvShowImage("One more", hw);

    // waiting for esc 
    char c = 0;
    while (c != 27)
        c = cvWaitKey(0);

    cvReleaseImage(&hw);
    cvDestroyWindow("Hello World");
    cvDestroyWindow("One more");
}

void MainApplication::processImage(const char* imgFile)
{
    IplImage* img(cvLoadImage(imgFile));
    if (!img)
    {
        printf("Error in image (%s) loading\n", imgFile);
        return;
    }

    cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
    cvShowImage("original", img);

    //IplImage* result = scaleImageGRAY(*img, 4);
    IplImage* result = scaleImageYUV(*img, 2);

    if (!result)
    {
        printf("Unexpected result in processImage()\n");
        cvReleaseImage(&img);
        cvDestroyWindow("original");
        return;
    }

    cvNamedWindow("result", CV_WINDOW_AUTOSIZE);
    cvShowImage("result", result);

    cvWaitKey(0);

    cvReleaseImage(&img);
    cvReleaseImage(&result);

    cvDestroyAllWindows();
}


IplImage* MainApplication::scaleImageGRAY(const IplImage& frameRGB, int scale/* = 2*/)
{
    const int width = frameRGB.width;
    const int height = frameRGB.height;
    const int imgSize = width * height;
    const int dataSize = (imgSize * 3) >> 1;

    IplImage* gray = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
    cvConvertImage(&frameRGB, gray, CV_BGR2GRAY);

    unsigned char* data = new unsigned char[dataSize];
    memset(data, 0, dataSize);
    for (int i = 0; i < imgSize; ++i)
        data[i] = (unsigned char)gray->imageData[i];
    cvReleaseImage(&gray);

    core::YUVImage img(core::YUVImage::YUV420SP, width, height, data);
    core::YUVImage* scaled = img.scaled(scale);

    const int scWidth = scaled->width();
    const int scHeight = scaled->height();
    const int scImgSize = scWidth * scHeight;
    const int scDataSize = ( scImgSize * 3) >> 1;
    delete data;
    data = const_cast<unsigned char*>(scaled->data());
    gray = cvCreateImage(cvSize(scWidth, scHeight), IPL_DEPTH_8U, 1);
    for (int i = 0; i < scImgSize; ++i)
        gray->imageData[i] = data[i];

    delete scaled;
    return gray;
}

IplImage* MainApplication::scaleImageYUV(const IplImage& frameRGB, int scale/* = 2*/)
{
    const int width = frameRGB.width;
    const int width3 = 3 * width;
    const int height = frameRGB.height;
    const int imgSize = width * height;
    const int imgSize3 = 3 * imgSize;
    const int dataSize = imgSize3 >> 1;

    IplImage* yuv = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
    cvConvertImage(&frameRGB, yuv, CV_BGR2YUV);

    unsigned char* data = new unsigned char[dataSize];
    memset(data, 0, dataSize * sizeof(char));
    // y
    for (int i = 0; i < imgSize; ++i)
        data[i] = (unsigned char)yuv->imageData[3 * i];

    unsigned char* uvData = data + imgSize;
    const int uvDataSize = imgSize >> 1;
    char* srcData = yuv->imageData;
    // u
    for (int j = 0, s = 1; j < uvDataSize && s < imgSize3; j += 2, s += 6)
    {
        int val = (unsigned char)srcData[s] + (unsigned char)srcData[s + 3] +
            (unsigned char)srcData[s + width3] + (unsigned char)srcData[s + width3 + 3];
        val /= 4;
        uvData[j] = val;

        if ( s % width3 == width3 - 5)
            s += width3;
    }
    // v
    for (int j = 1, s = 2; j < uvDataSize && s < imgSize3; j += 2, s += 6)
    {
        int val = (unsigned char)srcData[s] + (unsigned char)srcData[s + 3] +
            (unsigned char)srcData[s + width3] + (unsigned char)srcData[s + width3 + 3];
        val /= 4;
        uvData[j] = val;

        if ( s % width3 == width3 - 4)
            s += width3;
    }
    cvReleaseImage(&yuv);

    core::YUVImage img(core::YUVImage::YUV420SP, width, height, data);
    core::YUVImage* scaled = img.scaled(scale);
    delete data;
    if (!scaled)
    {
        printf("Error in YUVImage::scaled() method\n");
        return 0;
    }
    data = const_cast<unsigned char*>(scaled->data());

    const int scWidth = scaled->width();
    const int scHeight = scaled->height();
    const int scImgSize = scWidth * scHeight;
    const int scDataSize = ( scImgSize * 3) >> 1;

    yuv = cvCreateImage(cvSize(scWidth, scHeight), IPL_DEPTH_8U, 3);
    int i = 0;
    for (i; i < scImgSize; ++i)
        yuv->imageData[3 * i] = data[i];
    i = 1;
    for (core::YUVImage::iterator it = scaled->beginU(), e = scaled->endU(); it != e; ++it, i += 3)
        yuv->imageData[i] = *it;
    i = 2;
    for (core::YUVImage::iterator it = scaled->beginV(), e = scaled->endV(); it != e; ++it, i += 3)
        yuv->imageData[i] = *it;
    delete scaled;

    return yuv;
}

} // namespace app