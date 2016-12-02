#include "mainwindow.h"
#include <QApplication>
bool rerun = true;
int main(int argc, char *argv[])
{
    while(rerun)
    {
        QApplication a(argc, argv);
        MainWindow w;
        QTimer::singleShot(0, &w, SLOT(onEventLoopStarted()));
        w.show();
        a.exec();
    }
    return 0;
}
