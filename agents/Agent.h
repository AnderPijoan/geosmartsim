#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QRunnable>
#include <QThread>
#include <QMutex>

#include "geos/geom/Geometry.h"
#include "geos/geom/Point.h"

#include "environment/communication_environment/Message.h"
#include "utils/json/JSONSerializer.h"

QT_FORWARD_DECLARE_CLASS(Message)

using namespace geos::geom;

class Agent : public QObject, public QRunnable
{
    Q_OBJECT // Needed in every agent to get its correct metaobject, if a child has no Q_OBJECT, it will have its parents classname in getClass()
public:
    ~Agent();
    virtual void stop();
    virtual QJsonObject toJSON(); // Can overwrite if want to add extra variables to the web JSON
    virtual QJsonObject customGeoJSONProperties(); // Can overwrite if want to add extra variables to geojson properties

    // Messages
    Message* createMessage(QString content);
    void emitMessageSignal(Message *message);

    // Setters
    virtual void setFromFrontend(QMap<QVariant, QVariant> parameters); // Update agent according to parameters sent from frontend
    void setName(QString name);
    void setGeometry(Geometry* geom);
    void setBornDatetime(QDateTime born_datetime);

    // Getters
    unsigned int getId();
    QString getClass();
    QString getName();
    Geometry* getGeometry();
    QDateTime getBornDatetime();
    QString toString();
    static QJsonArray manyToJSON(QList<Agent*> agents);

    // Specific display variables
    bool ui_map_follow;
    QString ui_color;
    QString ui_border_color;
    QString ui_border_weight;
    QString ui_opacity;
    QString ui_border_opacity;

signals:
    void broadcastSocketMessage(QByteArray message);
    void sendMessageToEnvironment(Message *message);

public slots:
    void receiveNotificationFromEnvironment(Message *message);

protected:
    // Abstract agent, cant be created
    Agent();

private:
    virtual void run(){} // Only environment can call RUN, empty for passive entities
    static unsigned int counter; // Autoincremental for ids

    // ALL AGENTS (Agents and passive entities) common variables
    unsigned int id; // Agent id
    QString name; // Agent name
    Geometry *geometry; // Entity geometry
    QDateTime born_datetime; // Entity creation/borning datetime

    // Mutex
    QMutex mutex;
};

#endif // AGENT_H
