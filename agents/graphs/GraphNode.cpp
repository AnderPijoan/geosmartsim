#include "GraphNode.h"

GraphNode::GraphNode(QString class_name) : Agent(class_name){
}

GraphNode::~GraphNode(){

}

/**********************************************************************
 GETTERS
**********************************************************************/


/**********************************************************************
 SETTERS
**********************************************************************/

void GraphNode::setGeometry(Geometry *geom){
    // An node's geometry must be a Point
    Point* point = dynamic_cast<Point*>(geom);
    Agent::setGeometry( point );
}
