#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include <QObject>
#include "geos/geom/LineString.h"

#include "agents/Agent.h"
#include "environment/physical_environment/managers/graphs/GraphNetworksManager.h"

class GraphEdge : public Agent
{
    Q_OBJECT
    friend class GraphNetworksManager;
public:
    GraphEdge(GraphEdge &edge); // Used for child elements
    ~GraphEdge();

    // SETTERS
    void setMaxSpeed(double speed);
    void setLength(double length);

    // GETTERS
    double getMaxSpeed();
    double getLength();

    // ENTITY METHODS
    Point* nextPoint();
    Point* previousPoint();
    QVector<Point*> getPoints();
    Point* currentPoint();
    void goToBeginning();
    double secondsToGoThrough(double element_max_speed); // In m/s

private:
    GraphEdge(LineString* geometry); // Only manager can create

    double edge_max_speed; // m/s
    double edge_length; // m

    int point_iterator_position;
};

#endif // GRAPHEDGE_H
