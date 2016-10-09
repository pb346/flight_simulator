#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "timerThread.h"
#include "plane.h"
#include <QMainWindow>
void parseXML();
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
    void updateValues(joystick_event* event);
    void updateSliders(joystick_event* event);
    int runningFlag;
    Plane* planeState;

private slots:
    void onUpdateGUI(joystick_event*);
    void on_pushButton_clicked();
    void onEventLoopStarted();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
