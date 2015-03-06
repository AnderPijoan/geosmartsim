#ifndef DIJKSTRASKILL_H
#define DIJKSTRASKILL_H

#include "environment/skills/Skill.h"

#include <lemon/core.h>
#include <lemon/list_graph.h>

#include "agents/graphs/GraphEdge.h"
#include "agents/graphs/GraphNode.h"

using namespace lemon;

class DijkstraSkill : public Skill
{
public:
    DijkstraSkill(Agent* agent);
    ~DijkstraSkill();

    // METHODS
    QList<GraphEdge*> dijkstraShortestPath(QList<GraphEdge*> edges, GraphNode* start_node, GraphNode* end_node);

private:
    // Graph
    ListDigraph* graph;
    // Costs
    ListDigraph::ArcMap<double>* edge_weights;
    // Node geometry (as string) map to avoid duplicates
    QMap<QString, ListDigraph::Node> node_geometries;
    // Node and Edge arc map to retrieve them
    QMap<ListDigraph::Arc, GraphEdge*> arcs;
    QMap<ListDigraph::Node, GraphNode*> nodes;
};

#endif // DIJKSTRASKILL_H
