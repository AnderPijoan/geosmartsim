#ifndef ROAD_H
#define ROAD_H

#include "agents/graphs/GraphEdge.h"
#include "agents/graphs/roads/RoadConstants.h"
#include "environment/skills/contain/AgentContainSkill.h"

class Road : public GraphEdge
{
        Q_OBJECT
public:
    Road(RoadConstants::road_types type, GraphNode* start_node, GraphNode* end_node, QString class_name = "Road");
    ~Road();
     virtual QJsonObject customGeoJSONProperties(); // Overwrite to add extra variables to geojson properties

    // SETTERS
    void setLength(double length);
    void setRoadType(RoadConstants::road_types road_type);

    // GETTERS
    double getLength();
    RoadConstants::road_types getRoadType();

    // ENTITY METHODS
    bool enterRoad(Agent* enterint_agent);
    bool exitRoad(Agent* exiting_agent);
    int getAgentAmountInside();

    // ENTITY SKILLS
    AgentContainSkill* agent_contain_skill;

private:

    double length; // In meters
    RoadConstants::road_types road_type; // Road type
};

#endif // ROAD_H
