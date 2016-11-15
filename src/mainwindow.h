#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "timerThread.h"
#include "plane.h"
#include "planemodel.h"
#include <QMainWindow>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
PlaneModel* parseXML();
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    timerThread* procThread;
    PlaneModel* currentModel;
    Plane* planeState;
    Plane* previousState;
    DebugValues* debug;
    DebugValues* previousDebug;
    int runningFlag;
    int rotate = 90;
    QImage* imageObject;
    QPixmap image;
    QPixmap rotateImage;
    QGraphicsScene* scene;
    void updateValues(joystick_event* event);
    void updateSliders(joystick_event* event);

private slots:
    void onUpdateGUI(joystick_event*);
    void on_pushButton_clicked();
    void onEventLoopStarted();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
