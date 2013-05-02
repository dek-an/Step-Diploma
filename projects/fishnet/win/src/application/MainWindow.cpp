#include "MainWindow.h"
#include "MainProcessor.h"

#include <common/CommonException.h>

#include <gl/GL.h>

namespace application
{

using common::CommonException;

#define WINDOW_CLASSNAME "Fishnet Simulation Window" // Window class name

MainWindow::MainWindow(INT iWidth, INT iHeight)
    : m_hWindow(NULL)
    , m_iWndWidth(iWidth)
    , m_iWndHeight(iHeight)
    , m_hDeviceContext(NULL)
    , m_hGLContext(NULL)
    , m_pProcessor(new MainProcessor())
{
    init();
}

void MainWindow::init()
{
    registerWindowClass();

    RECT WindowRect = { 0, 0, m_iWndWidth, m_iWndHeight};

    const DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    const DWORD dwStyle = WS_OVERLAPPEDWINDOW;

    // Adjust the window to the true requested size
    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);
    // And create the main window
    m_hWindow = CreateWindowEx( dwExStyle, TEXT(WINDOW_CLASSNAME),
                                TEXT(WINDOW_CLASSNAME),
                                WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
                                0, 0, WindowRect.right - WindowRect.left,
                                WindowRect.bottom - WindowRect.top,
                                NULL, NULL,
                                GetModuleHandle(NULL),
                                this);

    if (m_hWindow == NULL)
        throw CommonException("Cannot create the main window");

    createContext();
    initGL();
    ShowWindow(m_hWindow,SW_SHOW);
}

MainWindow::~MainWindow()
{
    delete m_pProcessor;

    if (m_hGLContext)
    {
        // Make the rendering context not current
        wglMakeCurrent(NULL,NULL);
        // Delete the OpenGL rendering context
        wglDeleteContext(m_hGLContext);
        m_hGLContext = NULL;
    }
    if (m_hDeviceContext)
    {
        // Release the device context
        ReleaseDC(m_hWindow,m_hDeviceContext);
        m_hDeviceContext = NULL;
    }

    // Finally, destroy our main window and unregister the
    // window class.
    DestroyWindow(m_hWindow);
    UnregisterClass(TEXT(WINDOW_CLASSNAME), GetModuleHandle(NULL));
}

LRESULT MainWindow::onEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam)
{
    if (Message == WM_NCCREATE)
    {
        // Get the creation parameters.
        CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);

        // Set as the "user data" parameter of the window
        SetWindowLongPtr(Handle, GWLP_USERDATA, 
                         reinterpret_cast<long>(pCreateStruct->lpCreateParams));
    }

    // Get the MainWindow instance corresponding to the window handle
    MainWindow* pWindow = reinterpret_cast<MainWindow*>
        (GetWindowLongPtr(Handle, GWLP_USERDATA));
    if (pWindow)
        pWindow->processEvent(Message,wParam,lParam);

    return DefWindowProc(Handle, Message, wParam, lParam);
}

void MainWindow::processEvent(UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message)
    {
        // Quit when we close the main window
        case WM_CLOSE :
            PostQuitMessage(0);
            break;
        case WM_SIZE:
            onSize(LOWORD(lParam), HIWORD(lParam));
            break;
        case WM_KEYDOWN:
            break;
        case WM_KEYUP:
            break;
        case WM_LBUTTONDOWN:
            m_pProcessor->onLeftButtonDown(LOWORD(lParam), HIWORD(lParam));
            break;
        case WM_LBUTTONUP:
            m_pProcessor->onLeftButtonUp(LOWORD(lParam), HIWORD(lParam));
            break;
        case WM_MOUSEMOVE:
            if (wParam == MK_LBUTTON)
                m_pProcessor->onMouseMove(LOWORD(lParam), HIWORD(lParam));
            break;
    }
}

void MainWindow::registerWindowClass()
{
    WNDCLASS WindowClass;
    WindowClass.style         = 0;
    WindowClass.lpfnWndProc   = &MainWindow::onEvent;
    WindowClass.cbClsExtra    = 0;
    WindowClass.cbWndExtra    = 0;
    WindowClass.hInstance     = GetModuleHandle(NULL);
    WindowClass.hIcon         = NULL;
    WindowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    WindowClass.hbrBackground = 0;
    WindowClass.lpszMenuName  = NULL;
    WindowClass.lpszClassName = (LPCSTR)WINDOW_CLASSNAME;

    RegisterClass(&WindowClass);
}

#pragma warning (push)
#pragma warning (disable: 4706)
void MainWindow::createContext()
{
    // Describes the pixel format of the drawing surface
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;   // Version Number
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |  // Draws to a window
                  PFD_SUPPORT_OPENGL |  // The format must support OpenGL
                  PFD_DOUBLEBUFFER;     // Support for double buffering
    pfd.iPixelType = PFD_TYPE_RGBA;     // Uses an RGBA pixel format
    pfd.cColorBits = 32;                // 32 bits colors

    INT PixelFormat;
    if (!(m_hDeviceContext = GetDC(m_hWindow))
        || !(PixelFormat = ChoosePixelFormat(m_hDeviceContext, &pfd)) // Do Windows find a matching pixel format ?
        || !SetPixelFormat(m_hDeviceContext, PixelFormat, &pfd) // Set the new pixel format
        || !(m_hGLContext = wglCreateContext(m_hDeviceContext)) // Create the OpenGL rendering context
        || !wglMakeCurrent(m_hDeviceContext, m_hGLContext)) // Activate the rendering context
            throw CommonException("Unable to create rendering context");
}
#pragma warning (pop)

void MainWindow::initGL()
{
    // Enable 2D texturing
    glEnable(GL_TEXTURE_2D);
    // Choose a smooth shading model
    glShadeModel(GL_SMOOTH);
    // Set the clear color
    glClearColor((GLclampf)0.9, (GLclampf)1.0, (GLclampf)1.0, (GLclampf)0.0);

    // Enable the alpha test. This is needed 
    // to be able to have images with transparent 
    // parts.
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
}

void MainWindow::onSize(GLsizei width, GLsizei height)
{
    // Sets the size of the OpenGL viewport
    glViewport(0,0,width,height);

    // Select the projection stack and apply 
    // an orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,width,height,0.0,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
}

void MainWindow::update(DWORD/* dwCurrentTime*/)
{
    m_pProcessor->update();
}

void MainWindow::draw()
{
    // Clear the buffer
    glClear(GL_COLOR_BUFFER_BIT);

    m_pProcessor->draw();

    SwapBuffers(m_hDeviceContext);
}

} // namespace application