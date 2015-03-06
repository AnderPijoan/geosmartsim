#ifndef TIME_H
#define TIME_H

#include <QObject>
#include <QDebug>
#include <QDateTime>

#include "environment/Environment.h"

/**
 * @brief The Time class
 * Class to be used in environment to get time / hours.
 */
class Time : public QObject
{
    Q_OBJECT
    friend class Environment; // Only environment can call creator and setters
public:
    // GETTERS
    static double msecondsToEnvironmentMseconds(double seconds);
    static double secondsToEnvironmentMseconds(double seconds);
    static QDateTime getCurrentDateTime();
    static QVariantMap getCurrentDateTimeAsJSON();

private: // Only environment can create and init
    static Time* getInstance();
    static void initTime(QDateTime environment_time, double time_speed = 1);

    Time();
    Time(Time const&);

    // Environment start date/time expecified when init
    static QDateTime environment_time;
    // Time speed
    // (How many seconds will there be in a real world second)
    static double time_speed;
    // The real date/time the class was launched
    // to make calculations about time spent since start
    static QDateTime internal_start_time;
};

#endif // TIME_H
