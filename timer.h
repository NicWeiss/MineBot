#ifndef TIMER_H
#define TIMER_H

#include <QThread>
#include "packet.h"
#include "player.h"

class Timer: public QThread
{
Q_OBJECT
public:
    Timer();

    // QThread interface
protected:
    void run();
};

#endif // TIMER_H
