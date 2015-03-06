#ifndef GENERATECOSKILL_H
#define GENERATECOSKILL_H

#include "environment/physical_environment/skills/polution//GenerateContaminantSkill.h"

class GenerateCOSkill: public GenerateContaminantSkill
{
public:
    GenerateCOSkill(Agent* agent, VehicleConstants::vehicle_types type, QString technology, QString subsegment);
    ~GenerateCOSkill();
};

#endif // GENERATECOSKILL_H
