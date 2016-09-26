#ifndef STATE_TIMER_H
#define STATE_TIMER_H

#include <QObject>
#include <QTimer>


class StateTimer : public QObject
{
    Q_OBJECT
public:
    explicit StateTimer(QObject *parent = 0);
    int count;
public slots:
    void updateCount();

private:
    QTimer *timer;

};

#endif // STATE_TIMER_H
