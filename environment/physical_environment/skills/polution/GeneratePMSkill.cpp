#include "GeneratePMSkill.h"

GeneratePMSkill::GeneratePMSkill
(Agent* agent , VehicleConstants::vehicle_types type, QString technology, QString subsegment) :
    GenerateContaminantSkill(agent, type , technology , subsegment){
    this->contaminantName = "PM";
}

GeneratePMSkill::~GeneratePMSkill(){
}
