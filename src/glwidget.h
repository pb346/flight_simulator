#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QOpenGLWidget>
#include <QtWidgets>

class GLWidget : public QOpenGLWidget
{
public:
    GLWidget(QWidget *parent);
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
public:
    QTimer* timer;
    double radius;
    float angle, x1,x2,y1,y2;
};

#endif // GLWIDGET_H
