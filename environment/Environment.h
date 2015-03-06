#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QObject>
#include <QMutex>
#include <QThreadPool>
#include <QNetworkAccessManager>

#include "agents/Agent.h"

#include "utils/http_server/HttpServer.h"
#include "utils/websocket_server/WebSocketServer.h"

#include "environment/time/Time.h"
#include "environment/time/Scheduler.h"

// Physycal Environment
#include "physical_environment/PhysicalEnvironment.h"

// Communication Environment
#include "environment/communication_environment/CommunicationEnvironment.h"

QT_FORWARD_DECLARE_CLASS(Time)
QT_FORWARD_DECLARE_CLASS(Scheduler)

class Environment : public PhysicalEnvironment, public CommunicationEnvironment
{
public:
    static Environment* getInstance();

    // SETTERS
    void setMaxParallelAgents(unsigned int number);
    void createHttpServer(unsigned int port);
    void createWebSocketServer(unsigned int port);
    void setEnvironmentTime(QDateTime environmentTime, double time_speed);
    void updateAgentGeometry(Agent* agent, Geometry* old_geom, Geometry* new_geom); // Update agent geometry
    Scheduler* createScheduler(int seconds);

    // GETTERS
    Agent* getAgent(QString class_name, unsigned int id); // Get one agent
    Agent* getAgent(unsigned int id); // Get one agent
    QList<Agent*> getAgents(QString class_name); // Get only one class type agents

    // ENVIRONMENT METHODS
    bool containsAgent(Agent* agent);
    bool addAgent(Agent *agent);
    bool addAgents(QList<Agent*> agents);
    bool deleteAgent(Agent* agent);
    bool deleteAgent(unsigned int id);
    bool runAgents(QString class_name); // Run all
    Agent* runAgent(Agent* agent); // Run one
    Agent* stopAgent(Agent* agent); // Stop one

private:
    Environment();
    Environment(Environment const&);

    HttpServer * http_server;
    WebSocketServer * websocket_server;
    Time * environment_date_time;

    // QHASH<ClassName, QHASH< ID , AGENT>>
    QHash<QString, QHash<unsigned int, Agent*> > environment_agents;
    // Mutex
    QMutex mutex;
};

#endif // ENVIRONMENT_H
