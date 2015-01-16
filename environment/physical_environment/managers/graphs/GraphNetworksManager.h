#ifndef GRAPHNETWORKSMANAGER_H
#define GRAPHNETWORKSMANAGER_H

#include <QObject>
#include <QMap>
#include <QMutex>
#include "geos/geom/LineString.h"

#include "agents/graphs/GraphEdge.h"
QT_FORWARD_DECLARE_CLASS(GraphEdge)

using namespace geos::geom;

class GraphNetworksManager : public QObject
{
    Q_OBJECT
public:
    static GraphEdge* createEdge(LineString* geometry);
    static GraphEdge* getEdge(LineString* geometry);
    static bool insertEdge(GraphEdge* edge , bool replace = false);
    static void clearEdges();

private:
    static QHash<QString , GraphEdge*> graph_edges;

    // Mutex
    static QMutex mutex;
};

#endif // GRAPHNETWORKSMANAGER_H
