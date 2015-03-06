#include <QDebug>
#include <QFile>
#include <QMutexLocker>

#include "Logger.h"

QString Logger::common_path = "./../logs/";
//QString Logger::common_path = "logs/";
QString Logger::default_filename = "log";

void Logger::log(QVariant info, QString filename){
    if (!filename.isEmpty()){
       QFile logfile( Logger::common_path + filename);
       if(logfile.open(QIODevice::Append)){
           QTextStream out(&logfile);
           out << info.toString() << "\n";
       }
       logfile.close();
    } else {
       QFile logfile( Logger::common_path + Logger::default_filename);
       if(logfile.open(QIODevice::Append)){
           QTextStream out(&logfile);
           out << info.toString() << "\n";
       }
       logfile.close();
    }
}

void Logger::setDefaultFileName(QString filename){
    Logger::default_filename = filename;
}

QString Logger::getDefaultFileName(){
    return Logger::default_filename;
}

