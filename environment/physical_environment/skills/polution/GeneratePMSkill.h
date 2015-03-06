#ifndef GENERATEPMSKILL_H
#define GENERATEPMSKILL_H

#include "environment/physical_environment/skills/polution/GenerateContaminantSkill.h"

class GeneratePMSkill : public GenerateContaminantSkill
{
public:
    GeneratePMSkill(Agent* agent, VehicleConstants::vehicle_types type, QString technology, QString subsegment);
    ~GeneratePMSkill();
};

#endif // GENERATEPMSKILL_H
