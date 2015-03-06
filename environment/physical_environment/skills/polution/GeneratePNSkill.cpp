#include "GeneratePNSkill.h"

GeneratePNSkill::GeneratePNSkill
(Agent* agent , VehicleConstants::vehicle_types type, QString technology, QString subsegment) :
    GenerateContaminantSkill(agent , type , technology , subsegment){
    this->contaminantName = "PN";
}

GeneratePNSkill::~GeneratePNSkill(){
}
