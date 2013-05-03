#ifndef APPLICATION_GLDRAWER_H_
#define APPLICATION_GLDRAWER_H_

#include <Windows.h>


namespace phys
{
class Mass;
} // namespace phys

namespace application
{

class GLDrawer
{
public:
    GLDrawer();
    GLDrawer(const GLDrawer&);
    ~GLDrawer();

public:
    void init();
    BOOLEAN createContext(const HDC& hDeviceContext);

    void onSize(INT width, INT height);
    void onStartDraw();
    void onEndDraw();

    void operator()(const phys::Mass* mass1, const phys::Mass* mass2);
    void operator()(const phys::Mass* mass);

private:
    GLDrawer& operator=(const GLDrawer& other);

private:
    HGLRC   m_hGLContext;   // The openGL context.
};

} // namespace application

#endif // APPLICATION_GLDRAWER_H_