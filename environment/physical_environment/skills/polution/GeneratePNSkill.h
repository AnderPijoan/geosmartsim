#ifndef GENERATEPNSKILL_H
#define GENERATEPNSKILL_H

#include "environment/physical_environment/skills/polution/GenerateContaminantSkill.h"

class GeneratePNSkill : public GenerateContaminantSkill
{
public:
    GeneratePNSkill(Agent* agent, VehicleConstants::vehicle_types type, QString technology, QString subsegment);
    ~GeneratePNSkill();
};

#endif // GENERATEPNSKILL_H
