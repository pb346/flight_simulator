#ifndef TIMER_THREAD_H
#define TIMER_THREAD_H

#include "stateTimer.h"
#include "joystick_interface.h"
#include <QObject>
#include <QThread>

class timerThread : public QThread
{
    Q_OBJECT
public:
    explicit timerThread(QObject *parent = 0, bool b = false);
    void run();
    StateTimer *sTimer;
    bool Stop;
    void readState(joystick_event*);

signals:
    void updateGUI(joystick_event*);
};

#endif // TIMER_THREAD_H
