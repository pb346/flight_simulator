#include "stateTimer.h"

StateTimer::StateTimer(QObject *parent) : QObject(parent)
{
    count = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCount()));
    timer->start(10);
}

void StateTimer::updateCount()
{
    ++count;
}
