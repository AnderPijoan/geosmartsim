#ifndef ROUTINGNEARBUILDING_H
#define ROUTINGNEARBUILDING_H

#include "agents/buildings/Building.h"
#include "environment/physical_environment/skills/routing/PgRoutingDrivingDistanceSkill.h"

/**
 * @brief The RoutingNearBuilding class Building that finds its near buildings in X meters by routing
 */
class RoutingNearBuilding : public Building
{
    Q_OBJECT
public:
    RoutingNearBuilding(int levels, double accessible_meters);
    ~RoutingNearBuilding();

private:
    virtual void run();

    PgRoutingDrivingDistanceSkill* driving_skill;
    double accessible_meters;
};

#endif // ROUTINGNEARBUILDING_H
