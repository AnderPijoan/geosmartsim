#include "GenerateNOXSkill.h"

GenerateNOXSkill::GenerateNOXSkill
(Agent* agent , VehicleConstants::vehicle_types type, QString technology, QString subsegment) :
    GenerateContaminantSkill(agent , type , technology , subsegment){
    this->contaminantName = "NOx";
}

GenerateNOXSkill::~GenerateNOXSkill(){
}
