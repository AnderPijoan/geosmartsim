#include "FindNearestAmenityBuilding.h"
#include "environment/Environment.h"
#include "agents/vehicles/VehicleConstants.h"

FindNearestAmenityBuilding::FindNearestAmenityBuilding(int levels) : Building(levels){
    this->near_amenity_skill = new PgRoutingNearAgentsSkill(this, VehicleConstants::roadTypesForVehicle(VehicleConstants::car), 1);
}

FindNearestAmenityBuilding::~FindNearestAmenityBuilding(){
    delete this->near_amenity_skill;
}


void FindNearestAmenityBuilding::run(){
    QList<Agent*> amenities = Environment::getInstance()->getAgents("building");
    if ( !amenities.isEmpty() ){
        try{
            QMap<GraphRoute*, Agent*> ordered = this->near_amenity_skill->orderByDistance( amenities, Environment::getInstance()->getGeometryFactory() );
            this->style->color = ordered.first()->style->color;
            this->style->border_color = ordered.first()->style->color;
            this->style->border_weight = 20;
            emit  updateUI( JSONSerializer::createJSONMessage( this->toJSON() , JSONSerializer::CREATE) );
        } catch(...){}
    }
}
