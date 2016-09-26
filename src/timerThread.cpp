#include "timerThread.h"
#include "joystick_interface.h"
#include "stateTimer.h"

#define TIMER_SPEED (10)
timerThread::timerThread(QObject *parent, bool b):
    QThread(parent), Stop(b)
{
    QThread *thread1 = new QThread;
    thread1->start();
    sTimer = new StateTimer;
    sTimer->moveToThread(thread1);
}

void timerThread::run()
{
    struct joystick_event* event;
    event = new joystick_event;
    readState(event);
    delete event;
}

void timerThread::readState(joystick_event* event)
{
    while(!this->Stop)
    {
        if(true == this->Stop)break;
        saveWin(event);
        emit updateGUI(event);
        this->msleep(1);

        if(TIMER_SPEED <= sTimer->count)
        {
            sTimer->count = 0;
        }
    }
    return;
}
