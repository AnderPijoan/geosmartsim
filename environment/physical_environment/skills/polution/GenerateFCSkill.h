#ifndef GENERATEFCSKILL_H
#define GENERATEFCSKILL_H

#include "environment/physical_environment/skills/polution//GenerateContaminantSkill.h"

class GenerateFCSkill : public GenerateContaminantSkill
{
public:
    GenerateFCSkill(Agent* agent, VehicleConstants::vehicle_types type, QString technology, QString subsegment);
    ~GenerateFCSkill();

};

#endif // GENERATEFCSKILL_H
