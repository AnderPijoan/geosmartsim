#include <QDebug>

#include "RadiusNearBuilding.h"
#include "environment/Environment.h"
#include "utils/loggers/Logger.h"


RadiusNearBuilding::RadiusNearBuilding(int levels, double accessible_meters) : Building(levels){
    this->near_skill = new NearAgentsSkill(this);
    this->accessible_meters = accessible_meters;
}

RadiusNearBuilding::~RadiusNearBuilding(){

}

void RadiusNearBuilding::run(){
    QList<Agent*> buildings = Environment::getInstance()->getAgents("building");
    int inhabitants = 0;

    foreach(Agent* agent, buildings){
        if( agent->getGeometry()->distance(this->getGeometry()) <  0.003 ){
            agent->style->color = "#FF0000";
            //emit broadcastSocketMessage( JSONSerializer::createJSONMessage( agent->toJSON() , JSONSerializer::UPDATE) );
            inhabitants += dynamic_cast<Building*>(agent)->getCapacity();
        }
    }
    Logger::log( QString("%1 , %2").arg(this->getId()).arg(inhabitants) , "RADIUS");
}
