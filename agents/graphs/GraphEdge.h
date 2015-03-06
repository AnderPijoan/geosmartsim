#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include <QObject>

#include "geos/geom/Point.h"
#include "geos/geom/LineString.h"
#include <lemon/core.h>
#include <lemon/list_graph.h>

#include "agents/Agent.h"
#include "agents/graphs/GraphNode.h"

using namespace lemon;

class GraphEdge : public Agent
{
    Q_OBJECT
public:
     GraphEdge(GraphNode* start_node, GraphNode* end_node, QString class_name = "GraphEdge");
     ~GraphEdge();

    // GETTERS
    double getWeight();
    double getMaxSpeed();
    GraphNode* getStartNode();
    GraphNode* getEndNode();
    double getsecondsToGoThrough(double speed); // Speed in meters/seconds

    // SETTERS
    void setGeometry(Geometry* geom);
    void setWeight(double weight);
     void setMaxSpeed(double max_speed);

private:
    GraphNode* start_node;
    GraphNode* end_node;
    double weight; // For path finding
    double max_speed; // In meter/second
};

#endif // GRAPHEDGE_H
