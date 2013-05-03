#include "GLDrawer.h"

#include <Windows.h>
#include <gl/GL.h>

#include <physics/Mass.h>
#include <physics/Constants.h>


namespace application
{

// ------------------------------------------------
// ----------- class TrigonometricTable -----------
// ------------------------------------------------

// sin and cos table of all angles (0 ~ 360);
// uses for nodes drawing
class TrigonometricTable
{
public:
    static void init();

    static const int TABLE_SIZE = 360;

    static float SIN_TABLE[TABLE_SIZE];
    static float COS_TABLE[TABLE_SIZE];
};

float TrigonometricTable::SIN_TABLE[TABLE_SIZE];
float TrigonometricTable::COS_TABLE[TABLE_SIZE];

void TrigonometricTable::init()
{
    static bool inited = false;
    if (inited)
        return;

    static const float kDegToRad = phys::PI_VALUE / 180.f;

    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        SIN_TABLE[i] = sin(kDegToRad * i);
        COS_TABLE[i] = cos(kDegToRad * i);
    }

    inited = true;
}

// ------------------------------------------------
// ------------------------------------------------
// ------------------------------------------------

GLDrawer::GLDrawer()
    : m_hGLContext(NULL)
{}

GLDrawer::GLDrawer(const GLDrawer&)
    : m_hGLContext(NULL)
{}

GLDrawer::~GLDrawer()
{
    if (m_hGLContext)
    {
        // Make the rendering context not current
        wglMakeCurrent(NULL,NULL);
        // Delete the OpenGL rendering context
        wglDeleteContext(m_hGLContext);
        m_hGLContext = NULL;
    }
}


void GLDrawer::init()
{
    TrigonometricTable::init();

    // Enable 2D texturing
    glEnable(GL_TEXTURE_2D);
    // Choose a smooth shading model
    glShadeModel(GL_SMOOTH);
    // Set the clear color
    glClearColor((GLclampf)0.96, (GLclampf)0.96, (GLclampf)0.86, (GLclampf)0.0);

    // Enable the alpha test. This is needed 
    // to be able to have images with transparent 
    // parts.
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    glColor3f(0.0, 0.0, 0.0);
}

#pragma warning (push)
#pragma warning (disable: 4706)
BOOLEAN GLDrawer::createContext(const HDC& hDeviceContext)
{
        return  (m_hGLContext = wglCreateContext(hDeviceContext)) // Create the OpenGL rendering context
                && wglMakeCurrent(hDeviceContext, m_hGLContext); // Activate the rendering context
}
#pragma warning (pop)


void GLDrawer::onSize(INT width, INT height)
{
    // Sets the size of the OpenGL viewport
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // Select the projection stack and apply
    // an orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void GLDrawer::onStartDraw()
{
    // Clear the buffer
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLDrawer::onEndDraw()
{}


void GLDrawer::operator()(const phys::Mass* mass1, const phys::Mass* mass2)
{
    glBegin(GL_LINE_STRIP);

    glVertex2f(mass1->position().x(), mass1->position().y());
    glVertex2f(mass2->position().x(), mass2->position().y());

    glEnd();
}

void GLDrawer::operator()(const phys::Mass* mass)
{
    glBegin(GL_TRIANGLE_FAN);

    for (int i = 0; i < 360; i += 10)
        glVertex2f( mass->position().x() + phys::POINT_RADIUS * TrigonometricTable::COS_TABLE[i],
                    mass->position().y() + phys::POINT_RADIUS * TrigonometricTable::SIN_TABLE[i]);

    glEnd();
}

} // namespace application