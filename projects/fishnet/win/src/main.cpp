#include <Windows.h>

#include <application/MainApplication.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, INT)
{
    application::MainApplication app(hInstance, lpCmdLine);
    return app.run();
}