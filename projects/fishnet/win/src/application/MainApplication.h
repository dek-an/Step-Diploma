#ifndef COMMON_MAINAPPLICATION_H_
#define COMMON_MAINAPPLICATION_H_

#include <windows.h>

namespace application
{

// The application class, which simply wraps the message queue and process
// the command line.
class MainApplication
{
public:
    MainApplication(HINSTANCE hInstance, LPSTR lpCmdLine);
    ~MainApplication();

    // Creates the main window and start the message loop.
    int run();

private:
    void parseCmdLine();
    void process();

private:
    MainApplication(const MainApplication&);
    MainApplication& operator=(const MainApplication&);

private:
    static const INT FRAME_TIME = 33;

    HINSTANCE   m_hInstance;
    LPSTR       m_lpCmdLine;
    INT         m_iWndWidth;
    INT         m_iWndHeight;
};

} // namespace application

#endif // COMMON_MAINAPPLICATION_H_