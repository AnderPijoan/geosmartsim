#ifndef ROAD_H
#define ROAD_H

#include "agents/graphs/GraphEdge.h"
#include "environment/skills/contain/AgentContainSkill.h"
#include "environment/physical_environment/skills/routing/PgRoutingConstants.h"

class Road : public GraphEdge
{
        Q_OBJECT
public:
    Road(GraphEdge edge, PgRoutingConstants::road_types type);
    ~Road();
     virtual QJsonObject customGeoJSONProperties(); // Overwrite to add extra variables to geojson properties

    // SETTERS
    void setRoadType(PgRoutingConstants::road_types road_type);

    // GETTERS
    PgRoutingConstants::road_types getRoadType();

    // ENTITY METHODS
    bool enterRoad(Agent* enterint_agent);
    bool exitRoad(Agent* exiting_agent);
    int getEntityAmountInside();

    // ENTITY SKILLS
    AgentContainSkill* entity_contain_skill;

private:
    PgRoutingConstants::road_types road_type;
};

#endif // ROAD_H
