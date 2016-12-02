#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "timerThread.h"
#include "plane.h"
#include "planemodel.h"
#include "QMediaPlayer"
#include <QMainWindow>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
struct ProgramTime
{
    int hours;
    int minutes;
    int seconds;
    int milli;
    ProgramTime(){};
    ProgramTime(int h, int min, int s, int mil)
    {
        hours = h;
        minutes = min;
        seconds = s;
        milli = mil;
    }
};

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
    bool warningActive;
    bool crashActive;
    bool noFuelActive;
    bool engineActive = false;
    int spoolCounter;
    bool angularFlip;
    bool spoolOne;
    bool planeSet;
    QMediaPlayer* warning1;
    QMediaPlayer* pullUp;
    QMediaPlayer* noFuel;
    QMediaPlayer* engine;
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
    QPixmap rotateImage;
    QGraphicsScene* scene;
    QGraphicsScene* altScene;
    QGraphicsScene* speedScene;
    QGraphicsScene* angularScene;
    QGraphicsScene* statusScene;
    ProgramTime* currentTime;
    int headingAngle;
    int headerTimerCount;
    int statusCount;
    int crashCount;
    int cycleTimer;
    bool isCrashed;
    double pitch;
    QString localPath;
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
    void printSnappyResponse();
    PlaneModel* parseXML();
    void updateTime();

private slots:
    void onUpdateGUI(joystick_event*);
    void on_pushButton_clicked();
    void closeEvent(QCloseEvent *event);
    void onEventLoopStarted();\
    void on_pushButton_reset_clicked();
    void on_pushButton_quit_clicked();
private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
