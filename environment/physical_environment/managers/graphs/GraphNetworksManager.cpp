#include "GraphNetworksManager.h"
#include <QDebug>

// Static variables
QHash<QString , GraphEdge*> GraphNetworksManager::graph_edges;
QMutex GraphNetworksManager::mutex;

GraphEdge* GraphNetworksManager::createEdge(LineString *geometry){
    QMutexLocker locker(&GraphNetworksManager::mutex);
    QString key = QString::fromStdString(geometry->toString());
    if( GraphNetworksManager::graph_edges.contains( key ) ){
        return GraphNetworksManager::graph_edges.value( key );
    }
    GraphEdge* edge = new GraphEdge(geometry);
    return edge;
}

bool GraphNetworksManager::insertEdge(GraphEdge *edge, bool replace){
    QMutexLocker locker(&GraphNetworksManager::mutex);
    QString key = QString::fromStdString(edge->getGeometry()->toString());
    bool contains = GraphNetworksManager::graph_edges.contains( key );
    if( !replace && contains ){
        return false;
    }
    // We are going to insert, so if there was a previous edge, delete it
   if ( contains ){
        GraphNetworksManager::graph_edges.remove( key );
    }
     GraphNetworksManager::graph_edges.insert( key , edge );
     return true;
}

GraphEdge* GraphNetworksManager::getEdge(LineString *geometry){
    QMutexLocker locker(&GraphNetworksManager::mutex);
     QString key = QString::fromStdString(geometry->toString());
     if( GraphNetworksManager::graph_edges.contains( key ) ){
         return GraphNetworksManager::graph_edges.value( key );
     }
    return 0;
}

void GraphNetworksManager::clearEdges(){
    qDeleteAll(GraphNetworksManager::graph_edges);
}
