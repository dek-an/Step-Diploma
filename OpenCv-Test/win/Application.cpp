#include "Application.h"

#include <highgui.h>
#include <cv.h>

#include <core/YUVImage.h>
#include <core/gestures/GestureRecognitor.h>
#include <core/ContourDetector.h>

using namespace core;

Application::Application(void)
{
    
}

Application::~Application(void)
{
    
}

int Application::run(void)
{
    //showHelloWorld();
    captureCamera();

    return 0;
}

void Application::showHelloWorld(void)
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

    waitForEsc();

    cvReleaseImage(&hw);
    cvDestroyWindow("Hello World");
    cvDestroyWindow("One more");
}

void Application::captureCamera(void)
{
    CvCapture* cam(cvCreateCameraCapture(CV_CAP_ANY));
    assert(cam);
    printf("FORMAT: %f\n", cvGetCaptureProperty(cam, CV_CAP_PROP_BRIGHTNESS));

    cvNamedWindow("CamWnd", CV_WINDOW_AUTOSIZE);

    IplImage* frameRGB = 0;
    IplImage* frameYUV = 0;
    IplImage* frameGRAY = 0;
    unsigned char* data = 0;

    ContourDetector cd(GestureRecognitor::skinBinarizationFunction);

    while (true)
    {
        static float sum = 0.f;
        static int frameNum = 0;
        float start = clock();

        frameRGB = cvQueryFrame(cam);

        float ellapsedTime = clock() - start;
        sum += ellapsedTime;
        //printf("ellapsed time: %3.3f   average: %3.3f\n", ellapsedTime, sum / (++frameNum));

        const int width = frameRGB->width;
        const int height = frameRGB->height;
        const int size = (width * height * 3);
        if (!data)
        {
            data = new unsigned char[size];
            frameYUV = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
            frameGRAY = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
        }

        cvCvtColor(frameRGB, frameYUV, CV_BGR2YUV);
        cvCvtColor(frameRGB, frameGRAY, CV_BGR2GRAY);

        for (int i = 0; i < size; ++i)
            data[i] = (unsigned char)frameYUV->imageData[i];

        YUVImage img(YUVImage::YUV, frameYUV->width, frameYUV->height, data);
        
        cd.detect(img);
        //img.doBinaryMask(GestureRecognitor::skinBinarizationFunction);
        //img.smoothBinaryMask();
        for (int i = 0; i < height; ++i)
            //frameGRAY->imageData[i] = grayImg->data()[i];
            for (int j = 0; j < width; ++j)
                frameGRAY->imageData[i * width + j] = 0;//img.m_binaryMask(i, j);
//        delete grayImg;

        typedef ContourDetector::ContourContainer Contour;
        Contour cont = cd.getContour();
        Contour::const_iterator begin = cont.begin();
        Contour::const_iterator end = cont.end();
        int kk = 0;
        for (Contour::const_iterator it = begin; it != end; ++it, ++kk)
            frameGRAY->imageData[it->x() * width + it->y()] = 255;

        if (frameRGB)
            cvShowImage("CamWnd", frameGRAY);

        char bttn = cvWaitKey(33);
        if (bttn == 27 || frameNum == 300)
            break;
    }

    delete[] data;

    cvReleaseCapture(&cam);
    cvDestroyWindow("CamWnd");
}

void Application::waitForEsc(int delay)
{
    char c = 0;
    while (c != 27)
        c = cvWaitKey(delay);
}