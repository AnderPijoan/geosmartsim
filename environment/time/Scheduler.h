#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "environment/Environment.h"

#include <QObject>

class Scheduler : public QObject
{
    Q_OBJECT
    friend class Environment;
public:
    void start();
    void stop();

signals:
    void activate();

private: // Only environment can create one
     Scheduler(int seconds);
    int seconds;
    bool running;
};

#endif // SCHEDULER_H
