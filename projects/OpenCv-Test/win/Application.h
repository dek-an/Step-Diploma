#ifndef APPLICATION_H_
#define APPLICATION_H_

typedef struct _IplImage IplImage;

class Application
{
public:
    Application(void);
    ~Application(void);

    int run(void);

private:
    Application(const Application&);
    Application& operator=(const Application&);

private:
    void showHelloWorld(void);
    void captureCamera(void);
    void processImage(const char* imgFile);

    void firstTestings(const IplImage& frameRGB);
    IplImage* scaleImageGRAY(const IplImage& frameRGB, int scale = 2);
    IplImage* scaleImageYUV(const IplImage& frameRGB, int scale = 2);

    static void waitForEsc(int delay = 0);
};

#endif // APPLICATION_H_