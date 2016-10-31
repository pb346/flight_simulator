#include "glwidget.h"
#include <math.h>

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    timer = new QTimer(this);
    timer->start(400);
    radius = 0.5;
    angle = 0.0;
}

void GLWidget::initializeGL()
{
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}

void GLWidget::paintGL()
{
    x1 = 0.0, y1 = 0.0;
    int triangles = 100; //number used to draw
    float dPi = 2.0f * M_PI; //double Pi
    glColor3f(0.5,0.5,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1, y1); //center object at these coord
    for(int i =0; i <= triangles; i++)
    {
        x2 = x1 + (radius * cos(i * dPi / triangles))*2;
        y2 = y1 + (radius * sin(i * dPi / triangles))*2;
        glVertex2f(x2, y2);
        if(y2<0){
            glColor3f(0.5f, 0.35f, 0.05f);
        }
    }
    glEnd();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
}
