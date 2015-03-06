#ifndef RADIUSNEARBUILDING_H
#define RADIUSNEARBUILDING_H

#include "agents/buildings/Building.h"
#include "environment/physical_environment/skills/gis/NearAgentsSkill.h"

/**
 * @brief The RadiusNearBuilding class Building that finds its nearest buildings in a X meters radius
 */
class RadiusNearBuilding : public Building
{
    Q_OBJECT
public:
    RadiusNearBuilding(int levels, double accessible_meters);
    ~RadiusNearBuilding();

private:
    virtual void run();

    double accessible_meters;
    NearAgentsSkill* near_skill;

};

#endif // RADIUSNEARBUILDING_H
