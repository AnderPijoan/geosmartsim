#include "GraphRoute.h"
GraphRoute::GraphRoute(Point* starting, Point* ending, QObject *parent) : QObject(parent){
    this->starting_point = starting;
    this->ending_point = ending;
    this->edge_iterator_position = -1;   // STARTS IN -1
}

GraphRoute::~GraphRoute(){
    if( this->starting_point ){
        delete this->starting_point;
    }
    if ( this->ending_point ){
        delete this->ending_point;
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

Point* GraphRoute::getStartingPoint(){
    return this->starting_point;
}

Point* GraphRoute::getEndingPoint(){
    return this->ending_point;
}

double GraphRoute::getTotalLength(){
    double length = 0;
    foreach(GraphEdge* edge , this->route_edges){
        length += edge->getLength();
    }
    return length;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void GraphRoute::setStartingPoint(Point *point){
    this->starting_point = point;
}

void GraphRoute::setEndingPoint(Point *point){
    this->ending_point = point;
}

/**********************************************************************
 METHODS
**********************************************************************/

bool GraphRoute::addEdge(GraphEdge* edge){
    if( this->route_edges.contains(edge) ){
        return false;
    }
     this->route_edges.append(edge);
    return true;
}

GraphEdge* GraphRoute::nextEdge(){
    if( this->edge_iterator_position < this->route_edges.size() - 1 ){
        this->edge_iterator_position++;
        return this->route_edges.at( this->edge_iterator_position );
    }
    return 0;
}

GraphEdge* GraphRoute::previousEdge(){
    if( this->edge_iterator_position > 0 ){
        this->edge_iterator_position--;
        return this->route_edges.at( this->edge_iterator_position );
    }
    return 0;
}

GraphEdge* GraphRoute::currentEdge(){
    return this->route_edges.at( this->edge_iterator_position );
}

void GraphRoute::goToBeginning(){
    this->edge_iterator_position = -1; // MUST START IN -1
}
