#include "GenerateHCSkill.h"

GenerateHCSkill::GenerateHCSkill
(Agent* agent , VehicleConstants::vehicle_types type, QString technology, QString subsegment) :
    GenerateContaminantSkill(agent , type , technology , subsegment){
    this->contaminantName = "HC";
}

GenerateHCSkill::~GenerateHCSkill(){
}
