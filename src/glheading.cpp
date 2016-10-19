#include "glheading.h"
#include <math.h>

GLHeading::GLHeading(QWidget* parent):QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    timer = new QTimer(this);
    timer->start(400);
    radius = 0.6;
    angle = 0.0;
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
}

void GLHeading::initializeGL()
{
    glClearColor(0.0, 0.0, 102.0/255.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

void GLHeading::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void GLHeading::paintGL()
{
    x1 = 0.0, y1 = 0.0;
    int triangles = 100; //number used to draw
    float dPi = 2.0f * M_PI; //double Pi
    glColor3f(1.0, 1.0, 0.6);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1, y1); //center object at these coord
    for(int i =0; i <= triangles; i++)
    {
        x2 = x1 + (radius * cos(i * dPi / triangles));
        y2 = y1 + (radius * sin(i * dPi/triangles));
        glVertex2f(x2, y2);
    }
    glEnd();
    //radius += 0.001;
}
void GLHeading::repaint()
{
    paintGL();
}
