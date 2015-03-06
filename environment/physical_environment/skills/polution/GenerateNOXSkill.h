#ifndef GENERATENOXSKILL_H
#define GENERATENOXSKILL_H

#include "environment/physical_environment/skills/polution/GenerateContaminantSkill.h"

class GenerateNOXSkill : public GenerateContaminantSkill
{
public:
    GenerateNOXSkill(Agent* agent, VehicleConstants::vehicle_types type, QString technology, QString subsegment);
    ~GenerateNOXSkill();
};

#endif // GENERATENOXSKILL_H

