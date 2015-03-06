#include "RoutingNearBuilding.h"
#include "environment/Environment.h"
#include "agents/vehicles/VehicleConstants.h"
#include "utils/loggers/Logger.h"

RoutingNearBuilding::RoutingNearBuilding(int levels, double accessible_meters) : Building(levels){
    this->accessible_meters = accessible_meters;
    this->driving_skill = new PgRoutingDrivingDistanceSkill( this , VehicleConstants::roadTypesForVehicle(VehicleConstants::foot) );
}

RoutingNearBuilding::~RoutingNearBuilding(){
    delete this->driving_skill;
}

void RoutingNearBuilding::run(){

    this->driving_skill->getDriver()->connectDB();
    this->driving_skill->createTempRoutingTables();

    QList<Agent*> buildings = Environment::getInstance()->getAgents("building");
    QList<Building*> match_buildings;

    foreach(Agent* agent, buildings){
        if( agent->getGeometry()->distance(this->getGeometry()) < 0.003 ){
            Point* point = this->driving_skill->getNearestRoadPointToGeometry( agent->getGeometry() );
            agent->style->color = "#ADFF2F";
            dynamic_cast<Building*>(agent)->addEntrance( point );
            match_buildings.append( dynamic_cast<Building*>(agent) );
        }
    }

    // Load the amenities entrances
    this->loadEntrances( VehicleConstants::roadTypesForVehicle(VehicleConstants::foot) , "entrances.tags @ '\"entrance\"=>\"main\"' OR entrances.tags @ '\"entrance\"=>\"public\"' " );

    int inhabitants = 0;
    foreach(Point* entrance , this->getEntrances()){
        QVector<GraphRoute*> routes = this->driving_skill->getDrivingDistanceInMeters(entrance , 300 , Environment::getInstance()->getGeometryFactory());
        foreach(GraphRoute* route , routes){
                    while(route->nextEdge()){
                        Geometry* buffered_line = route->currentEdge()->getGeometry()->buffer(0.0001);
                        foreach(Building* building , match_buildings){
                            bool intersects = false;
                            foreach(Point* entrance , building->getEntrances()){
                                    intersects = intersects || buffered_line->intersects( entrance );
                            }
                            if ( intersects ){
                                //emit  broadcastSocketMessage( JSONSerializer::createJSONMessage( building->toJSON() , JSONSerializer::UPDATE) );
                                inhabitants += building->getCapacity();
                                match_buildings.removeOne( building );
                            }
                        }
                        //emit  broadcastSocketMessage( JSONSerializer::createJSONMessage( route->currentEdge()->toJSON() , JSONSerializer::UPDATE) );
                    }
            }
    }
     Logger::log( QString("%1 , %2").arg(this->getId()).arg(inhabitants) , "ROUTING");
    this->driving_skill->getDriver()->disconnectDB();
}
