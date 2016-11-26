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
    int startFlag;
    int clockCycles;
    //int rotate = 90;
    QImage* imageObject;
    QImage* altObject;
    QImage* speedObject;
    QImage* angularObject;
    QImage* statusObject;
    QPixmap image;
    QPixmap altImage;
    QPixmap speedImage;
    QPixmap angularImage;
    QPixmap statusImage;
    double pitch;



    QPixmap rotateImage;
    QGraphicsScene* scene;
    QGraphicsScene* altScene;
    QGraphicsScene* speedScene;
    QGraphicsScene* angularScene;
    QGraphicsScene* statusScene;
    int headingAngle;
    int headerTimerCount;
    int statusCount;
    int crashCount;
    bool isCrashed;
    void updateValues(joystick_event* event);
    void updateSliders(joystick_event* event);
    void updateHeading();
    void updateAltitude();
    void updateSpeed();
    void updateAngular();
    void updateStatus();
    void updateFuel();
    void headingInit();
    void altitudeInit();
    void speedInit();
    void angularInit();
    void statusInit();
    PlaneModel* parseXML();

private slots:
    void onUpdateGUI(joystick_event*);
    void on_pushButton_clicked();
    void closeEvent(QCloseEvent *event);
    void onEventLoopStarted();

    void on_pushButton_reset_clicked();

    void on_pushButton_quit_clicked();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
