#ifndef GENERATEHCSKILL_H
#define GENERATEHCSKILL_H

#include "environment/physical_environment/skills/polution/GenerateContaminantSkill.h"

class GenerateHCSkill : public GenerateContaminantSkill
{
public:
    GenerateHCSkill(Agent* agent, VehicleConstants::vehicle_types type, QString technology, QString subsegment);
    ~GenerateHCSkill();
};

#endif // GENERATEHCSKILL_H
