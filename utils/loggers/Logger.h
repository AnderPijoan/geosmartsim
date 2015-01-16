#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QMutex>

class Logger : public QObject
{
    Q_OBJECT
public:
    static void log(QVariant info, QString filename = "");
    static void setDefaultFileName(QString filename);
    static QString getDefaultFileName();

private:
    static QString common_path;
    static QString default_filename;
};

#endif // LOGGER_H

