#ifndef APPLICATION_APPLICATION_H_
#define APPLICATION_APPLICATION_H_

typedef struct _IplImage IplImage;

namespace application
{

class MainApplication
{
public:
    MainApplication(void);
    ~MainApplication(void);

public:
    int run(void);

private:
    void captureCamera(void);
    void frameCaptured(const IplImage& frameRGB);

    void backProjectingMask(const IplImage& frameRGB);
    void skinContourExtraction(const IplImage& frameRGB);

private: // Debug functions
    void showHelloWorld(void);
    void processImage(const char* imgFile);

    IplImage* scaleImageGRAY(const IplImage& frameRGB, int scale = 2);
    IplImage* scaleImageYUV(const IplImage& frameRGB, int scale = 2);

private:
    MainApplication(const MainApplication&);
    MainApplication& operator=(const MainApplication&);

private:
    IplImage*   mYuvFrame;
    IplImage*   mGrayResult;
};

} // nmespace app

#endif // APPLICATION_APPLICATION_H_