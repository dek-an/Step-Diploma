#include "MainApplication.h"

#include "MainWindow.h"

#include <common/CommonException.h>

#include <string>

namespace application
{

using common::CommonException;

MainApplication::MainApplication(HINSTANCE hInstance, LPSTR lpCmdLine)
  : m_hInstance(hInstance)
  , m_lpCmdLine(lpCmdLine)
  , m_iWndWidth(1000)
  , m_iWndHeight(600)
{}

MainApplication::~MainApplication()
{}


int MainApplication::run()
{
    try
    {
        parseCmdLine();
        process();
    }
    catch(CommonException& e)
    {
        MessageBox(NULL, (LPCSTR)e.what(), (LPCSTR)"Error", MB_OK|MB_ICONEXCLAMATION);
    }

    return 0;
}


void MainApplication::parseCmdLine()
{
    // parse cmd line code here
}

void MainApplication::process()
{
    // Create the main window
    MainWindow mainWindow(m_iWndWidth, m_iWndHeight);

    MSG Message;
    Message.message = (UINT)~WM_QUIT;
    DWORD dwNextDeadLine = GetTickCount() + FRAME_TIME;
    DWORD dwSleep = FRAME_TIME;
    BOOLEAN bUpdate = FALSE;

    // Loop until a WM_QUIT message is received
    while (Message.message != WM_QUIT)
    {
        // Wait a message (every dwSleep or timeout)
        DWORD dwResult = MsgWaitForMultipleObjectsEx(0, NULL, dwSleep, QS_ALLEVENTS, 0);

        if (dwResult != WAIT_TIMEOUT) // No timeout (process all of messages)
        {
            while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }

            // Process if current time is close (or past) to the deadline
            bUpdate = (GetTickCount() >= dwNextDeadLine) ? TRUE : FALSE;
        }
        else // Timeout (application should be processed)
            bUpdate = TRUE;

        // Check if the application should be processed
        if (bUpdate)
        {
            DWORD dwCurrentTime = GetTickCount();
            // Update the main window
            mainWindow.update(dwCurrentTime);
            // Draw the main window
            mainWindow.draw();

            dwNextDeadLine += FRAME_TIME;
        }

        // Process the sleep time, which is the difference
        // between the current time and the next deadline.
        dwSleep = dwNextDeadLine - GetCurrentTime();

        // If the sleep time is larger than the frame time,
        // it probably means that the processing was stopped 
        // (e.g. the window was being moved,...), so recalculate
        // the next deadline.
        if (dwSleep > FRAME_TIME)
        {
            dwSleep = FRAME_TIME;
            dwNextDeadLine = GetCurrentTime() + FRAME_TIME;
        }
    }
}

} // namespace application