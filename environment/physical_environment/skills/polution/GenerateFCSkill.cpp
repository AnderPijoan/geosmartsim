#include "GenerateFCSkill.h"

GenerateFCSkill::GenerateFCSkill
(Agent* agent , VehicleConstants::vehicle_types type, QString technology, QString subsegment) :
    GenerateContaminantSkill(agent , type , technology , subsegment){
    this->contaminantName = "FC"; // In gr units
}

GenerateFCSkill::~GenerateFCSkill(){
}
