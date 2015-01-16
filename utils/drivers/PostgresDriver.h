#ifndef POSTGRESDRIVER_H
#define POSTGRESDRIVER_H
#include <QObject>
#include <QMutex>
#include <QString>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QVector>
#include <QMap>
#include <QVariant>
#include <geos/geom/Geometry.h>

using namespace geos::geom;

class PostgresDriver : public QObject
{
    Q_OBJECT
public:
    static void initDB(QString db_url, QString db_name, QString db_user, QString db_pass, int max_queries = 20);
    PostgresDriver(QString connectionName = "");
    ~PostgresDriver();
    bool connectDB();
    void disconnectDB();
    void forceDisconnectDB();

    void createEnvironmentBoundsTempTable(QString real_name, QString temp_name, QString geometry_column, QString extra_where = "");
    QList<QSqlRecord> getAllRows(QString dbTable, QString dbWhere = "");
    QList<QSqlRecord> executeCustomQuery(QString sql);
    int getOpenConnectionsCount();

private:
    QSqlDatabase database;
    QString connection_name;
    int driver_connection_count; // Counter just to open connection once and reuse it. Once the counter gets to 0 it will disconnect

    // Statics
    static QString db_url;
    static QString db_name;
    static QString db_user;
    static QString db_pass;
    static int db_max_queries; // How many queries can we run paralelly
    static int db_executing_queries_count; // Counter to know how many queries are being executed to the DB
};

#endif // POSTGRESDRIVER_H
