#ifndef APPLICATION_MAINWINDOW_H_
#define APPLICATION_MAINWINDOW_H_

#include <Windows.h>

namespace application
{

class MainProcessor;
class GLDrawer;

// The main window class
class MainWindow
{
public:
    MainWindow(INT iWidth, INT iHeight);
    ~MainWindow();

    // Called by the application class to update the logic
    void update(DWORD dwCurrentTime);
    // Called by the application class when the window need to be redrawn.
    void draw();

private:
    // Initialization
    void init();
    // Register the window class with the correct window procedure (OnEvent)
    void registerWindowClass();
    // Create the rendering context used by OpenGL
    void createContext();

    // Called when a WM_SIZE message is received
    void onSize(INT width, INT height);

    // Static function which will be the window procedure callback
    static LRESULT CALLBACK onEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);
    // Processes the messages that were received in OnEvent.
    void processEvent(UINT Message, WPARAM wParam, LPARAM lParam);

private:
    MainWindow(const MainWindow&);
    MainWindow& operator=(const MainWindow&);

private:
    HWND            m_hWindow;          // The window handle
    INT             m_iWndWidth;        // Window width
    INT             m_iWndHeight;       // Window height
    HDC             m_hDeviceContext;   // The window device context
    MainProcessor*  m_pProcessor;       // Main engine
    GLDrawer*       m_pDrawer;
};

} // namespace application

#endif // APPLICATION_MAINWINDOW_H_