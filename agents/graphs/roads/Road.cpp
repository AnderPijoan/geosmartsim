#include "Road.h"
#include <QJsonObject>

#include "environment/Environment.h"

Road::Road(GraphEdge edge, PgRoutingConstants::road_types type) : GraphEdge(edge){
    this->entity_contain_skill = new AgentContainSkill(this);
    this->setRoadType( type );
}

Road::~Road(){
    if( this->entity_contain_skill ){
        delete this->entity_contain_skill;
    }
}

QJsonObject Road::customGeoJSONProperties(){
    QJsonObject properties = Agent::customGeoJSONProperties();
    if (this->entity_contain_skill->getAmountInside() > 0){
        properties.insert("border_color", "#FF0000");
    } else {
        properties.insert("border_color", "#0000FF");
    }
    return properties;
}

/**********************************************************************
 METHODS
**********************************************************************/

bool Road::enterRoad(Agent* enterint_entity){
    return this->entity_contain_skill->enter(enterint_entity);
}

bool Road::exitRoad(Agent* exiting_entity){
    return this->entity_contain_skill->exit(exiting_entity);
}

int Road::getEntityAmountInside(){
    return this->entity_contain_skill->getAmountInside();
}

/**********************************************************************
 SETTERS
**********************************************************************/

void Road::setRoadType(PgRoutingConstants::road_types road_type){
    this->road_type = road_type;
    switch (this->road_type) {
    case PgRoutingConstants::h_motorway:
    case PgRoutingConstants::h_trunk:
    case PgRoutingConstants::h_primary:
        this->entity_contain_skill->setMaxCapacity( 2 );
        break;
    default:
        this->entity_contain_skill->setMaxCapacity( 1 );
        break;
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

PgRoutingConstants::road_types Road::getRoadType(){
    return this->road_type;
}
