#include "DijkstraSkill.h"

#include <QDebug>

#include <lemon/dijkstra.h>
#include <lemon/bellman_ford.h>
#include <lemon/path.h>

DijkstraSkill::DijkstraSkill(Agent* agent) : Skill(agent){
}

DijkstraSkill::~DijkstraSkill(){
}

/**********************************************************************
 METHODS
**********************************************************************/

QList<GraphEdge*> DijkstraSkill::dijkstraShortestPath(QList<GraphEdge*> edges, GraphNode* start_node, GraphNode* end_node){
    QList<GraphEdge*> result;

    if( start_node->getId() == end_node->getId() ){
        qWarning() << QString("Same start and end node") << endl;
        return result;
    }

    // Create graph and edge weights map
    this->graph = new ListDigraph();
    this->edge_weights = new ListDigraph::ArcMap<double>( *this->graph );

    // Create nodes and edges
    foreach(GraphEdge* edge, edges){

        // Create or retrieve edge start node
        QString start_node_str = QString::fromStdString( edge->getStartNode()->getGeometry()->toString() );
        ListDigraph::Node s;
        if( !this->node_geometries.contains( start_node_str ) ){
            s = this->graph->addNode();
            this->node_geometries.insert( start_node_str , s );
            this->nodes.insert( s , edge->getStartNode() );
        } else {
            s = this->node_geometries.value( start_node_str );
        }

        // Create or retrieve edge end node
        QString end_node_str = QString::fromStdString( edge->getEndNode()->getGeometry()->toString() );
        ListDigraph::Node e;
        if( !this->node_geometries.contains( end_node_str ) ){
            e = this->graph->addNode();
            this->node_geometries.insert( end_node_str , e );
            this->nodes.insert( e , edge->getEndNode() );
        } else {
            e = this->node_geometries.value( end_node_str );
        }

        // Create edge and its cost
        ListDigraph::Arc arc = this->graph->addArc(s , e);
        this->arcs.insert( arc , edge );
        this->edge_weights->set( arc , edge->getWeight() );
    }

    QString start_node_str = QString::fromStdString( start_node->getGeometry()->toString() );
    QString end_node_str = QString::fromStdString( end_node->getGeometry()->toString() );
    if ( !this->node_geometries.contains( start_node_str ) || !this->node_geometries.contains( end_node_str ) ){
        qWarning() << QString("Start or end nodes are not in graph") << endl;
    } else {

        // Compute dijkstra
        ListDigraph::Node start = this->node_geometries.value( start_node_str );
        ListDigraph::Node end = this->node_geometries.value( end_node_str );
        Dijkstra<ListDigraph, ListDigraph::ArcMap<double> > dijkstra( *this->graph , *this->edge_weights);

        if( !dijkstra.run( start , end ) ){
            qWarning() << QString("Can not reach end node from start") << endl;
        } else {

            Path<ListDigraph> shortest_path = dijkstra.path( end );
            for(int i = 0 ; i < shortest_path.length() ; i++) {
                ListDigraph::Arc arc = shortest_path.nth( i );
                result.append( this->arcs.value( arc ) );
            }
        }
    }

    // Delete created stuff
    delete this->graph;
    delete this->edge_weights;
    this->node_geometries.clear();
    this->arcs.clear();
    this->nodes.clear();

    // Return
    return result;
}

