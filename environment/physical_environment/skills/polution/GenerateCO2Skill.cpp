#include "GenerateCO2Skill.h"

GenerateCO2Skill::GenerateCO2Skill
(Agent* agent , VehicleConstants::vehicle_types type, QString technology, QString subsegment) :
    GenerateContaminantSkill(agent , type , technology , subsegment){
    this->contaminantName = "CO2(rep.)";
}

GenerateCO2Skill::~GenerateCO2Skill(){
}

