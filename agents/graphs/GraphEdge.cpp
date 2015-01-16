#include "GraphEdge.h"
#include <QDebug>

GraphEdge::GraphEdge(GraphEdge &edge) : Agent(){
    this->point_iterator_position = -1;  // STARTS IN -1
    try{
        this->setGeometry( edge.getGeometry()->clone() );
    } catch(...){}
    this->edge_length = this->getGeometry()->getLength();
    this->edge_max_speed = 1; //~ 4km/h
}

GraphEdge::GraphEdge(LineString* geometry) : Agent(){
    this->point_iterator_position = -1;  // STARTS IN -1
    this->setGeometry( geometry );
    this->edge_length = geometry->getLength();
    this->edge_max_speed = 1; //~ 4km/h
}

GraphEdge::~GraphEdge(){
}

/**********************************************************************
 GETTERS
**********************************************************************/

double GraphEdge::getLength(){
    return this->edge_length;
}

double GraphEdge::getMaxSpeed(){
    return this->edge_max_speed;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void GraphEdge::setLength(double length){
    this->edge_length = length;
}

void GraphEdge::setMaxSpeed(double speed){
    this->edge_max_speed = speed;
}

/**********************************************************************
 ENTITY METHODS
**********************************************************************/

Point* GraphEdge::nextPoint(){
    if( this->point_iterator_position < this->getGeometry()->getNumPoints()-1 ){
        this->point_iterator_position++;
        return dynamic_cast<LineString*> (this->getGeometry())->getPointN(this->point_iterator_position);
    }
    return 0;
}

Point* GraphEdge::previousPoint(){
    if( this->point_iterator_position > 0 ){
        this->point_iterator_position--;
        return dynamic_cast<LineString*> (this->getGeometry())->getPointN(this->point_iterator_position);
    }
    return 0;
}

void GraphEdge::goToBeginning(){
    this->point_iterator_position = -1; // MUST START IN -1
}

double GraphEdge::secondsToGoThrough(double element_max_speed){
    double speed = qMin( this->edge_max_speed ,element_max_speed );
    return this->edge_length / speed;
}

Point* GraphEdge::currentPoint(){
    dynamic_cast<LineString*> (this->getGeometry())->getPointN(this->point_iterator_position);
}

QVector<Point*> GraphEdge::getPoints(){
    QVector<Point*> points;
    for(int i = 0 ; i < this->getGeometry()->getNumPoints() ; i++ ){
        points.append( dynamic_cast<LineString*> (this->getGeometry())->getPointN( i ) );
    }
    return points;
}
