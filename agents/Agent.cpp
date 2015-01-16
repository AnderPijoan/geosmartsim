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
    // Important to to ++ before, otherwise if two agents are being created together
    // all inheritance will receive the same id for both of them
    this->id = ++Agent::counter;
    this->name = this->getClass() + "-" + QString::number(this->getId());
     // Specific display variables
    this->ui_map_follow = false;
}

Agent::~Agent() {
    Environment::getInstance()->deleteAgent(this->getId());
    if (this->geometry){
        delete this->geometry;
    }
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
    if(this->ui_map_follow) { agent.insert("map_follow", this->ui_map_follow); }
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
    if(!this->ui_color.isEmpty()) { properties.insert("color" , this->ui_color); }
    if(!this->ui_border_color.isEmpty()) { properties.insert("border_color" , this->ui_border_color); }
    if(!this->ui_border_weight.isEmpty()) { properties.insert("weight" , this->ui_border_weight); }
    if(!this->ui_opacity.isEmpty()) { properties.insert("fill_opacity" , this->ui_opacity); }
    if(!this->ui_border_opacity.isEmpty()) { properties.insert("border_opacity" , this->ui_border_opacity); }
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

void Agent::emitMessageSignal(Message *message)
{
    emit sendMessageToEnvironment(message);
}

Message* Agent::createMessage(QString content)
{
    Message *message = new Message();
    message->setContent(content);
    message->setSender(this->metaObject()->className() + ',' + QString::number(id));
    return message;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void Agent::setFromFrontend(QMap<QVariant, QVariant> parameters){
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
    if( this->geometry ){
        delete this->geometry;
    }
    this->geometry = geom;

}

void Agent::setBornDatetime(QDateTime born_datetime){
    this->born_datetime = born_datetime;
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

/**********************************************************************
 SLOTS
**********************************************************************/

void Agent::receiveNotificationFromEnvironment(Message *message){
    qDebug() << "Received message from Sender: " << message->getSender() << ": " <<  message->getContent();
}
