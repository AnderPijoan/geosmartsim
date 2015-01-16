#include "Scheduler.h"
#include <QThread>
#include "environment/time/Time.h"

Scheduler::Scheduler(int seconds)
{
    this->seconds = seconds;
}

void Scheduler::start()
{
    this->running = true;
    while(this->running){
        QThread::sleep(Time::msecondsToEnvironmentMseconds(this->seconds));
        emit this->activate();
        qDebug() << "HOLA!";
    }
}

void Scheduler::stop(){
    this->running = false;
}
