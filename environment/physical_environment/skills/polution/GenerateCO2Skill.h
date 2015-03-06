#ifndef CONSUMECO2SKILL_H
#define CONSUMECO2SKILL_H

#include <QMap>

#include "environment/physical_environment/skills/polution//GenerateContaminantSkill.h"

class GenerateCO2Skill : public GenerateContaminantSkill
{
public:
    GenerateCO2Skill(Agent* agent, VehicleConstants::vehicle_types type, QString technology, QString subsegment);
    virtual  ~GenerateCO2Skill();
};

#endif // CONSUMECO2SKILL_H
