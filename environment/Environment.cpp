#include "Environment.h"
#include <QMutexLocker>

/**
 * @brief Environment::getInstance
 * @return
 */
Environment* Environment::getInstance(){
    static Environment instance;
    return &instance;
}

/**
 * @brief Environment::Environment
 * @param parent
 */
Environment::Environment() : PhysicalEnvironment()
{
    // Init random number generator seed
    qsrand(QDateTime::currentMSecsSinceEpoch());

    // Init environment DateTime
    this->environment_date_time = Time::getInstance();
    this->environment_date_time->initTime(QDateTime::currentDateTime());
    QThread environment_date_time_thread;
    this->environment_date_time->moveToThread(&environment_date_time_thread);

    //May need to configure /etc/security/limits.conf
    QThreadPool::globalInstance()->setMaxThreadCount(1000);

    qDebug() << "Environment created" << endl;
}

void Environment::setEnvironmentTime(QDateTime environmentTime, double time_speed){
    this->environment_date_time->initTime(environmentTime, time_speed);
}

Scheduler* Environment::createScheduler(int seconds){
    Scheduler* scheduler = new Scheduler(seconds);
    QThread scheduler_thread;
    scheduler->moveToThread(&scheduler_thread);
    return scheduler;
}

void Environment::createHttpServer(unsigned int port){
    this->http_server = HttpServer::getInstance(port);
}

void Environment::createWebSocketServer(unsigned int port){
    // Its in a separated thread
    this->websocket_server = new WebSocketServer(port);
}

void Environment::setMaxParallelAgents(unsigned int number){
    QThreadPool::globalInstance()->setMaxThreadCount(number);
}

/**********************************************************************
 AGENTS METHODS
**********************************************************************/
 /* RUN - STOP
**********************************************************************/

bool Environment::runAllAgents(QString class_name){
    QList<Agent*> agents = this->getAgents(class_name);
    bool run = !agents.isEmpty();
    while(!agents.isEmpty()){
        int rand = qrand() % agents.size();
        run = run && Environment::getInstance()->runAgent(agents.at(rand));
        agents.removeAt(rand);
    }
    return run;
}

Agent* Environment::runAgent(Agent* agent){
    if (agent){ QThreadPool::globalInstance()->start(agent); }
    return agent;
}

Agent* Environment::stopAgent(Agent* agent){
    if (agent){ agent->stop(); }
    return agent;
}

/* CREATE - DELETE
**********************************************************************/

bool Environment::addAgents(QList<Agent*> agents){
    QMutexLocker locker(&mutex);
    QListIterator<Agent*> i(agents);
    bool inserted = true;
    while(i.hasNext()){
        // Call for each agent
        inserted = inserted && this->addAgent(i.next());
    }
    return inserted;
}

bool Environment::addAgent(Agent *agent){
    QMutexLocker locker(&mutex);
    // If it is the first agent of this class we receive, create the class in the hashmap
    if(!agents.contains(agent->getClass())){
        // Create HTTP URLS for these agent type
        QStringList httpOperations;
        httpOperations << "GETALL" << "GETONE" << "RUNALL" << "RUNONE" << "STOP";
        this->getHttpServer()->createUrls(agent->getClass().toLower(), httpOperations);
        // Insert agent in hashmap
        QHash<unsigned int, Agent*> map;
        agents.insert(agent->getClass(), map);
    }

    // Add to agent map
    agents[agent->getClass()].insert(agent->getId(), agent);
    // Connect with websocket
    CommunicationEnvironment::connect( agent , SIGNAL( broadcastSocketMessage(QByteArray) ) ,
                   this->getWebSocketServer() , SLOT( broadcastSocketMessage(QByteArray))
                   );

    // Configure signal to enable communication between agents and environment
    //QObject::connect(agent, SIGNAL(sendMessageToEnvironment(Message*)), this, SLOT(receiveMessageFromAgent(Message*)));

    // Configure signal to enable communication between environment and agents
    //QObject::connect(this, SIGNAL(sendNotificationToAgent(Message*)), entity, SLOT(receiveNotificationFromEnvironment(Message*)));
    return true;
}

bool Environment::deleteAgent(unsigned int id){
    QMutexLocker locker(&mutex);
    bool found = false;
    for(int i = 0; i < agents.keys().size() && !found; i++ ){
         found = agents[ agents.keys().at(i) ].remove( id ) > 0;
    }
    return found;
}

/* UPDATE
**********************************************************************/

bool Environment::containsAgent(Agent* agent){
    return this->agents[ agent->getClass() ].value( agent->getId() );
}

Agent* Environment::getAgent(QString class_name, unsigned int id){
    if (agents.contains(class_name) && agents[class_name].value(id) != 0){
         return this->agents.value(class_name).value(id);
    }
    return 0;
}

QList<Agent*> Environment::getAgents(QString class_name){
    QMutexLocker locker(&mutex);
    QList<Agent*> agents_list;
    QHashIterator<unsigned int, Agent*> i(agents[class_name]);
    while(i.hasNext()){
        Agent* agent = i.next().value();
        if (agent != 0)
            agents_list.append(agent);
    }
    return agents_list;
}

QList<Agent*> Environment::getAgents(){
    QList<Agent*> agents_list;
    QHashIterator<QString, QHash<unsigned int, Agent*> > i(this->agents);
    while(i.hasNext()){
        QHashIterator<unsigned int, Agent*> j(i.next().value());
        while(j.hasNext()){
            Agent* agent = j.next().value();
            if (agent != 0){
                agents_list.append(agent);
            }
        }
    }
    return agents_list;
}

/**********************************************************************
 GETTERS
**********************************************************************/

HttpServer* Environment::getHttpServer(){
    return this->http_server;
}

WebSocketServer* Environment::getWebSocketServer(){
    return this->websocket_server;
}
