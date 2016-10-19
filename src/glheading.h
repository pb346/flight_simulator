#ifndef GLHEADING_H
#define GLHEADING_H
#include <QGLWidget>
#include <QtWidgets>

class GLHeading : public QGLWidget
{
public:
    GLHeading(QWidget* parent);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void repaint();
public:
    QTimer* timer;
    double radius;
    float angle, x1,x2,y1,y2;
};

#endif // GLHEADING_H
