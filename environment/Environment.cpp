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

bool Environment::runAgents(QString class_name){
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
    if( !this->environment_agents.keys().contains(agent->getClass()) ){
        // Create HTTP URLS for these agent type
        QStringList httpOperations;
        httpOperations << "GETALL" << "GETONE" << "RUNALL" << "RUNONE" << "STOP";
        this->http_server->createUrls(agent->getClass(), httpOperations);
        // Insert new map with the agents class
        QHash<unsigned int, Agent*> map;
        // Insert new quadtree with the agents class
        Quadtree* index = new Quadtree();
        this->environment_agents.insert(agent->getClass(), map);
        this->spatial_index.insert(agent->getClass() , index);
    }

    // Add to agent map and spatial index
    this->environment_agents[agent->getClass()].insert(agent->getId(), agent);
    if( agent->getGeometry() && agent->getGeometry()->getEnvelopeInternal() ){
        this->spatial_index.value(agent->getClass())->insert(agent->getGeometry()->getEnvelopeInternal(), agent);
    }
    // Connect with UI
    CommunicationEnvironment::connect( agent , SIGNAL( updateUI(QByteArray) ) , this->websocket_server , SLOT( updateUI(QByteArray)));
    return true;
}

bool Environment::deleteAgent(unsigned int id){
    QMutexLocker locker(&mutex);
    Agent* remove_agent = this->getAgent(id);
    if (remove_agent){
        this->environment_agents[remove_agent->getClass()].remove( remove_agent->getId() );
        if( remove_agent->getGeometry() && remove_agent->getGeometry()->getEnvelopeInternal() ){
        this->spatial_index.value(remove_agent->getClass())->remove(remove_agent->getGeometry()->getEnvelopeInternal() , remove_agent);
        }
        return true;
    }
    return false;
}

/* UPDATE
**********************************************************************/

void Environment::updateAgentGeometry(Agent* agent, Geometry* old_geom, Geometry* new_geom){
    QMutexLocker locker(&mutex);
    // If it does not belong to the environment, do not update in the spatial index
    if ( this->containsAgent(agent) ){
        // Old_geom might be null if the agent did not have any geometry when initialized
        if( old_geom && old_geom->getEnvelopeInternal() ){
            this->spatial_index.value(agent->getClass())->remove(old_geom->getEnvelopeInternal() , agent);
        }
        if ( new_geom && new_geom->getEnvelopeInternal() ){
            this->spatial_index[agent->getClass()]->insert(new_geom->getEnvelopeInternal() , agent);
        }
    }
}

bool Environment::containsAgent(Agent* agent){
    if ( !this->environment_agents.keys().contains(agent->getClass()) ){
        return false;
    }
    return this->environment_agents[ agent->getClass() ].contains( agent->getId() );
}

Agent* Environment::getAgent(QString class_name, unsigned int id){
    if (environment_agents.contains(class_name) && environment_agents[class_name].value(id) != 0){
         return this->environment_agents.value(class_name).value(id);
    }
    return 0;
}

Agent* Environment::getAgent(unsigned int id){
    foreach(QString class_name, this->environment_agents.keys()){
        Agent* agent = this->getAgent(class_name , id);
        if (agent){
            return agent;
        }
    }
    return 0;
}

QList<Agent*> Environment::getAgents(QString class_name){
    QMutexLocker locker(&mutex);
    QList<Agent*> agents_list;
    QHashIterator<unsigned int, Agent*> i(environment_agents[class_name]);
    while(i.hasNext()){
        Agent* agent = i.next().value();
        if (agent != 0)
            agents_list.append(agent);
    }
    return agents_list;
}
