#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <QObject>

#include "geos/geom/Point.h"
#include <lemon/core.h>
#include <lemon/list_graph.h>

#include "agents/Agent.h"

using namespace lemon;

class GraphNode : public Agent
{
    Q_OBJECT
public:
     GraphNode(QString class_name = "GraphNode");
     ~GraphNode();

    // GETTERS

    // SETTERS
    void setGeometry(Geometry* geom);
};

#endif // GRAPHNODE_H
