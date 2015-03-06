#include <QDebug>
#include <QSqlQuery>
#include <QtCore/QtMath>
#include <QSqlRecord>
#include <QSqlError>
#include <QDateTime>
#include <environment/Environment.h>

#include "PostgresDriver.h"

QString PostgresDriver::db_url;
int PostgresDriver::db_port = 5432;
QString PostgresDriver::db_name;
QString PostgresDriver::db_user;
QString PostgresDriver::db_pass;
int PostgresDriver::db_max_queries;
int PostgresDriver::db_executing_queries_count;

void PostgresDriver::initDB(QString dbURL, int dbPort, QString dbName, QString dbUser, QString dbPass, int max_queries){
    PostgresDriver::db_url = dbURL;
    PostgresDriver::db_port = dbPort;
    PostgresDriver::db_name = dbName;
    PostgresDriver::db_user = dbUser;
    PostgresDriver::db_pass = dbPass;
    PostgresDriver::db_max_queries = max_queries;
    PostgresDriver::db_executing_queries_count = 0;
}

PostgresDriver::PostgresDriver(QString connectionName) : QObject()
{
    this->connection_name = connectionName;
    this->driver_connection_count = 0;
    if (PostgresDriver::db_url.isEmpty() ||
            PostgresDriver::db_name.isEmpty() ||
            PostgresDriver::db_user.isEmpty() ||
            PostgresDriver::db_pass.isEmpty()){
        qWarning() << "Database common data not initiated, call init method to set global parameters";
    }
}

PostgresDriver::~PostgresDriver(){
    this->database.close();
    this->database.removeDatabase(this->connection_name);
}

bool PostgresDriver::connectDB(){
    if (this->database.isOpen()){
        this->driver_connection_count++;
        return true;
    }

    if (PostgresDriver::db_url.isEmpty() ||
            PostgresDriver::db_name.isEmpty() ||
            PostgresDriver::db_user.isEmpty() ||
            PostgresDriver::db_pass.isEmpty()){
        qWarning() << "Database common data not initiated, call init method to set global parameters";
        return false;
    }

    this->database = QSqlDatabase::addDatabase("QPSQL" , this->connection_name);
    this->database.setHostName(PostgresDriver::db_url);
    this->database.setPort( PostgresDriver::db_port );
    this->database.setDatabaseName(PostgresDriver::db_name);
    this->database.setUserName(PostgresDriver::db_user);
    this->database.setPassword(PostgresDriver::db_pass);
    int wait = 2;
    while( !this->database.open() ){
        this->thread()->wait( wait * 1000 ); // Wait 2 seconds
        qWarning() << "Unable to connect to DB : " << this->database.hostName() << this->database.port() << this->database.databaseName() << " waiting..." << endl;

     }
   return true;
}

void PostgresDriver::disconnectDB(){
    this->driver_connection_count--;
    this->driver_connection_count = qMax(this->driver_connection_count , 0);
    if(this->database.isOpen() && this->driver_connection_count <= 0){
        //qDebug() << "Disconnected from DB : " << database.hostName() << database.port() << database.databaseName() << this->hash << endl;
        this->database.close();
        this->database.removeDatabase(this->connection_name);
    }
}

void PostgresDriver::forceDisconnectDB(){
    this->driver_connection_count = 0;
    this->disconnectDB();
}

int PostgresDriver::getOpenConnectionsCount(){
    return this->driver_connection_count;
}

void PostgresDriver::createEnvironmentBoundsTempTable(QString real_name, QString temp_name, QString geometry_column, QString extra_where){
    if (!Environment::getInstance()->getBounds()){
        qWarning() << "No bounds set, cant create bounds temp table";
        return;
    }
    QString sql = QString("CREATE TEMP TABLE %2 AS (\
                                                                SELECT * \
                                                                FROM %3 \
                                                                WHERE \
                                                                                ST_INTERSECTS( \
                                                                                                %4, \
                                                                                                ST_GeomFromText( '%1' , 4326) \
                                                                                ) %5 \
                          ); \
                           CREATE INDEX %2_hidden_spatial_index ON %2 USING GIST ( %4 );")
            .arg( QString::fromStdString(Environment::getInstance()->getBounds()->toString()) )
            .arg(temp_name)
            .arg(real_name)
            .arg(geometry_column)
            .arg( extra_where.length() ? " AND " + extra_where : extra_where );

        this->executeCustomQuery(sql);

    // Change geometry column name to THE_GEOM
   if( geometry_column != "the_geom" ){
       sql = QString("ALTER TABLE %1 RENAME COLUMN %2 TO the_geom;")
               .arg(temp_name)
               .arg(geometry_column);
       this->executeCustomQuery(sql);
   }
}

QList<QSqlRecord> PostgresDriver::executeCustomQuery(QString sql){
    QList<QSqlRecord> list;
    if(this->database.isOpen()){
        QSqlQuery query(this->database.database(this->connection_name));
        while(PostgresDriver::db_executing_queries_count > PostgresDriver::db_max_queries){
            QThread::currentThread()->sleep(1);
        }
        PostgresDriver::db_executing_queries_count++;
        query.exec(sql);
        QSqlError error = query.lastError();
        if (error.type() != QSqlError::NoError){
            qWarning() << error.text() << endl << sql << endl;
        }
        while(query.next()){
                list.append(query.record());
        }
        //qDebug() << "Query returned" << list.size() << "records." << endl;
    } else {
        qWarning() << "Could not execute query because database is closed. Have tried calling connectDB() method?" << endl;
    }
    PostgresDriver::db_executing_queries_count--;
    return list;
}
