#include <QJsonDocument>
#include <QJsonObject>

#include "Time.h"

// Static variables
QDateTime Time::environment_time = QDateTime::currentDateTime();
double Time:: time_speed = 0;
QDateTime Time::internal_start_time = QDateTime::currentDateTime();

Time* Time::getInstance(){
    static Time instance;
    return &instance;
}

Time::Time()
{
}

void Time::initTime(QDateTime environmentTime, double timeSpeed){
    Time::environment_time = environmentTime;
    Time::time_speed = timeSpeed;
    // Time starts counting from now
    Time::internal_start_time = QDateTime::currentDateTime();
}

QByteArray Time::toJSON(){
    QVariantMap jsonMessage;
    jsonMessage.insert("time",Time::getTime().toMSecsSinceEpoch());

    return QJsonDocument::fromVariant(jsonMessage).toJson();
}

double Time::msecondsToEnvironmentMseconds(double mseconds){
    if(Time::time_speed == 0){
        qWarning() << "Time speed is 0";
        return 0;
    }
    return mseconds / time_speed;
}

double Time::secondsToEnvironmentMseconds(double seconds){
    if(Time::time_speed == 0){
        qWarning() << "Time speed is 0";
        return 0;
    }
    return (seconds / time_speed) * 1000;
}

QDateTime Time::getTime(){
    quint64 spent_mseconds = Time::internal_start_time.msecsTo(QDateTime::currentDateTime());
    QDateTime calculated_time = Time::environment_time.addMSecs(spent_mseconds * Time::time_speed);
    return calculated_time;
}
