#include "Road.h"
#include <QJsonObject>

#include "environment/Environment.h"

Road::Road(RoadConstants::road_types type, GraphNode* start_node, GraphNode* end_node, QString class_name) :
    GraphEdge(start_node, end_node, class_name){
    this->agent_contain_skill = new AgentContainSkill(this);
    this->setRoadType( type );
}

Road::~Road(){
        delete this->agent_contain_skill;
}

QJsonObject Road::customGeoJSONProperties(){
    QJsonObject properties = Agent::customGeoJSONProperties();
    if (this->agent_contain_skill->getAmountInside() > 0){
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
    return this->agent_contain_skill->enter(enterint_entity);
}

bool Road::exitRoad(Agent* exiting_entity){
    return this->agent_contain_skill->exit(exiting_entity);
}

int Road::getAgentAmountInside(){
    return this->agent_contain_skill->getAmountInside();
}

/**********************************************************************
 SETTERS
**********************************************************************/

void Road::setRoadType(RoadConstants::road_types road_type){
    this->road_type = road_type;
    switch (this->road_type) {
    case RoadConstants::h_motorway:
    case RoadConstants::h_trunk:
    case RoadConstants::h_primary:
        this->agent_contain_skill->setMaxCapacity( 2 );
        break;
    default:
        this->agent_contain_skill->setMaxCapacity( 1 );
        break;
    }
}

void Road::setLength(double length){
    this->length = length;
}

/**********************************************************************
 GETTERS
**********************************************************************/

RoadConstants::road_types Road::getRoadType(){
    return this->road_type;
}

double Road::getLength(){
    return this->length;
}
