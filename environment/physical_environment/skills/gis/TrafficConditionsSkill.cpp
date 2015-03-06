#include "TrafficConditionsSkill.h"

TrafficConditionsSkill::TrafficConditionsSkill(Agent* agent) : AccessAPISkill(agent){
}

TrafficConditionsSkill::~TrafficConditionsSkill(){
}

int TrafficConditionsSkill::getTrafficForRoadSegment(int road_id){
    getRequest(QUrl("www.deusto.es"));
    return 0;
}
