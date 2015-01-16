#ifndef GRAPHROUTE_H
#define GRAPHROUTE_H

#include <QObject>
#include <geos/geom/Point.h>
#include <geos/geom/LineString.h>

#include "agents/graphs/GraphEdge.h"

using namespace geos::geom;

class GraphRoute : public QObject
{
    Q_OBJECT
public:
    explicit GraphRoute(Point* starting, Point* ending, QObject *parent = 0);
    ~GraphRoute();

    // GETTERS
    Point* getStartingPoint();
    Point* getEndingPoint();
    double getTotalLength();

    // SETTERS
    void setStartingPoint(Point* point);
    void setEndingPoint(Point* point);

    // METHODS
    bool addEdge(GraphEdge* edge);
    GraphEdge* nextEdge();
    GraphEdge* previousEdge();
    GraphEdge* currentEdge();
    void goToBeginning();

private:
    QVector<GraphEdge*> route_edges; // Keys to get the edges from GraphNetworkManager
    Point* starting_point;
    Point* ending_point;
    int edge_iterator_position; // STARTS IN -1
};

#endif // GRAPHROUTE_H
