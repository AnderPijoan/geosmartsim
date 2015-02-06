#include <QDebug>
#include "environment/time/Time.h"
#include "utils/json/GeometryToGeoJSON.h"
#include "environment/Environment.h"

#include "Agent.h"

unsigned int Agent::counter = 0;

Agent::Agent()
{
    this->born_datetime = QDateTime::currentDateTime();
    this->geometry = 0;
    this->id = ++Agent::counter;
    this->name = this->getClass() + "-" + QString::number(this->getId());
    this->style = new UiStyle();
}

Agent::~Agent() {
    Environment::getInstance()->deleteAgent(this->getId());
    if (this->geometry){
        delete this->geometry;
    }
    this->style->deleteLater();
}

void Agent::stop() {
    qDebug() << "Stopping agent" << this->getClass() << this->getId() << endl;
}

/**********************************************************************
 GETTERS
**********************************************************************/

QString Agent::toString(){
     return QString(QString(this->getClass()) + " - " + QString::number(this->getId()) );
}

QJsonObject Agent::toJSON(){
    QJsonObject agent;
    agent.insert("id", int(this->getId()) ); // ID
    agent.insert("class", this->getClass()); // CLASS
    agent.insert("name", this->getName());
    // Specific display variables
    if(this->style->map_follow) { agent.insert("map_follow", this->style->map_follow); }
    if ( this->getGeometry() ) {
        agent.insert("x", this->getGeometry()->getCentroid()->getX());
        agent.insert("y", this->getGeometry()->getCentroid()->getY());
        agent.insert("geojson", GeometryToGeoJSON::toJSON( this->getGeometry() , this->customGeoJSONProperties() )); // GEOMETRY
    }
    return agent;
}

QJsonObject Agent::customGeoJSONProperties(){
    QJsonObject properties;
    properties.insert("id" , int( this->getId() ) );
    properties.insert( "class", this->getClass());
    properties.insert("name", this->getName());
   // Specific display variables
    QJsonObject style_json = this->style->asJSON();
    foreach(QString key , style_json.keys()){
          properties.insert(key , style_json.value(key) );
    }
    return properties;
}

QJsonArray Agent::manyToJSON(QList<Agent*> agents){
    QJsonArray array;
    QListIterator<Agent*> i(agents);
    while(i.hasNext()){
        array.append( i.next()->toJSON() );
    }
    return array;
}

/**********************************************************************
 MESSAGES
**********************************************************************/

/*void Agent::emitMessageSignal(Message *message)
{
    emit sendMessageToEnvironment(message);
}

Message* Agent::createMessage(QString content)
{
    Message *message = new Message();
    message->setContent(content);
    message->setSender(this->metaObject()->className() + ',' + QString::number(id));
    return message;
}*/

/**********************************************************************
 SETTERS
**********************************************************************/

void Agent::setFromUI(QMap<QVariant, QVariant> parameters){
    if ( dynamic_cast<Point*>( this->getGeometry() ) ){
        if ( parameters.contains("x") ){
            Point* point = dynamic_cast<Point*>( this->getGeometry() );
            Coordinate* coor = new Coordinate(  parameters["x"].toDouble() , point->getY() );
            Point* new_point = point->getFactory()->createPoint( *coor );
            this->setGeometry( new_point );
        }
        if ( parameters.contains("y") ){
           Point* point = dynamic_cast<Point*>( this->getGeometry() );
           Coordinate* coor = new Coordinate( point->getX() ,  parameters["y"].toDouble() );
           Point* new_point = point->getFactory()->createPoint( *coor );
           this->setGeometry( new_point );
        }
    }
}

void Agent::setName(QString name){
    this->name = name;
}

void Agent::setGeometry(Geometry* geom){
    QMutexLocker locker(&mutex);
    if( this->geometry != geom){
        if( this->geometry ){
            delete this->geometry;
        }
        this->geometry = geom;
    }
}

void Agent::setBornDatetime(QDateTime born_datetime){
    this->born_datetime = born_datetime;
}

void Agent::memorizeAgent(Agent *agent){
    if ( !this->memory.keys().contains(agent->getClass()) ){
        QHash<unsigned int, Agent*> map;
        this->memory.insert(agent->getClass(), map);
    }
    this->memory[agent->getClass()].insert(agent->getId() , agent);
}

void Agent::forgetAgent(Agent *agent){
    if ( this->memory.keys().contains(agent->getClass()) ){
        this->memory[agent->getClass()].remove(agent->getId());
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

unsigned int Agent::getId(){
    return this->id;
}

QString Agent::getClass(){
    return QString(this->metaObject()->className()).toLower();
}

QString Agent::getName(){
    return this->name;
}

Geometry* Agent::getGeometry(){
    QMutexLocker locker(&mutex);
    return this->geometry;
}

QDateTime Agent::getBornDatetime(){
    return this->born_datetime;
}

Agent* Agent::getAgentFromMemory(QString class_name, unsigned int id){
    if ( this->memory.keys().contains(class_name) ){
        return this->memory[class_name].value(id);
    }
}

QList<Agent*> Agent::getAgentsFromMemory(QString class_name){
    QList<Agent*> agents;
    foreach(Agent* agent , this->memory[class_name].values()){
        agents.append(agent);
    }
    return agents;
}

/**********************************************************************
 SLOTS
**********************************************************************/

/*void Agent::receiveNotificationFromEnvironment(Message *message){
    qDebug() << "Received message from Sender: " << message->getSender() << ": " <<  message->getContent();
}*/
