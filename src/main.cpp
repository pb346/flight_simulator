#include "mainwindow.h"
#include <QApplication>
#include "glheading.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTimer::singleShot(0, &w, SLOT(onEventLoopStarted()));
/*******************************************************
    QMainWindow *window = new QMainWindow();
    window->setWindowTitle(QString("Testing with OpenGL"));
    GLHeading* glheading = new GLHeading(window);
    glheading->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    window->setCentralWidget(glheading);
    window->setFixedSize(150,150);
    window->show();
Testing openGL functions ******************************/
    w.show();

    return a.exec();
}
