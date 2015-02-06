#ifndef FINDNEARESTAMENITYBUILDING_H
#define FINDNEARESTAMENITYBUILDING_H

#include "agents/buildings/Building.h"
#include "environment/physical_environment/skills/routing/PgRoutingNearAgentsSkill.h"
class FindNearestAmenityBuilding : public Building
{
    Q_OBJECT
public:
    FindNearestAmenityBuilding(int levels);
    ~FindNearestAmenityBuilding();
PgRoutingNearAgentsSkill* near_amenity_skill;

private:
    virtual void run();

};

#endif // FINDNEARESTAMENITYBUILDING_H
