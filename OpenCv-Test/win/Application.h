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

    void firstTestings(const IplImage& frameRGB);

    static void waitForEsc(int delay = 0);
};

#endif // APPLICATION_H_