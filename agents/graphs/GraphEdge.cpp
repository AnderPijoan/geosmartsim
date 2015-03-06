#include "GraphEdge.h"
#include <QtMath>

GraphEdge::GraphEdge(GraphNode* start_node, GraphNode* end_node, QString class_name) : Agent(class_name){
    this->weight = 1;
    this->start_node = start_node;
    this->end_node = end_node;
    this->max_speed = 1; //~ 4km/h
}

GraphEdge::~GraphEdge(){
}

/**********************************************************************
 GETTERS
**********************************************************************/

double GraphEdge::getWeight(){
    return this->weight;
}


double GraphEdge::getMaxSpeed(){
    return this->max_speed;
}

GraphNode* GraphEdge::getStartNode(){
    return this->start_node;
}

GraphNode* GraphEdge::getEndNode(){
    return this->end_node;
}

double GraphEdge::getsecondsToGoThrough(double speed){
    return this->getGeometry()->getLength() * 10000 / qMin(this->max_speed, speed);
}

/**********************************************************************
 SETTERS
**********************************************************************/

void GraphEdge::setGeometry(Geometry *geom){
    // An edge's geometry must be a LineString
    LineString* line = dynamic_cast<LineString*>(geom);
    Agent::setGeometry(line);
}

void GraphEdge::setWeight(double weight){
    this->weight = weight;
}

void GraphEdge::setMaxSpeed(double max_speed){
    this->max_speed = max_speed;
}
