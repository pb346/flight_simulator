#include "glheading.h"
#include <QFileInfo>
#include <math.h>
#include <stdio.h>
#include <QGLWidget>

GLHeading::GLHeading(QWidget* parent):QOpenGLWidget(parent)
{
    timer = new QTimer(this);
    timer->start(400);
    radius = 1.0;
    angle = 0.0;
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
}

void GLHeading::glRotated()
{

}

void GLHeading::initializeGL()
{
    //QFileInfo checkpath(":/new/prefix1/heading.bmp");
    //QOpenGLTexture* texture = new QOpenGLTexture(QImage(":/new/prefix1/heading.bmp"));
    //QImage fileName;
    //fileName.load(":/new/prefix/heading.bmp");
    //QImage texture = QGLWidget::convertToGLFormat(fileName);
    glClearColor(0.0, 0.0, 102.0/255.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
   // glEnable(GL_DEPTH_TEST);
   // glEnable(GL_LIGHT0);
  //  glEnable(GL_LIGHTING);
 //   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
 //   glEnable(GL_COLOR_MATERIAL);
}

void GLHeading::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void GLHeading::paintGL()
{
    x1 = 0.0, y1 = -1.0;
    int triangles = 50; //number used to draw
    float dPi = 2.0f * M_PI; //double Pi
    glColor3f(1.0, 1.0, 1.0);
   // glClearColor(1,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //setAutoBufferSwap(false);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1, y1); //center object at these coord
    for(int i =0; i <= triangles; i++)
    {
        x2 = x1 + (radius * cos(i * dPi / triangles));
        y2 = y1 + (radius * sin(i * dPi / triangles))*2;
        glVertex2f(x2, y2);
    }
    glEnd();
    //radius += 0.001;
}
void GLHeading::repaint()
{
    paintGL();
}
