#include "mainwindow.h"
#include <QApplication>
#include "glheading.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTimer::singleShot(0, &w, SLOT(onEventLoopStarted()));
    w.show();

    return a.exec();
}
