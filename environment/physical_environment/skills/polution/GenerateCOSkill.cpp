#include "GenerateCOSkill.h"

GenerateCOSkill::GenerateCOSkill
(Agent* agent , VehicleConstants::vehicle_types type, QString technology, QString subsegment) :
    GenerateContaminantSkill(agent , type , technology , subsegment){
    this->contaminantName = "CO";
}

GenerateCOSkill::~GenerateCOSkill(){
}
