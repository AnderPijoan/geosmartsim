#include "FindNearestAmenityPerson.h"
#include "environment/Environment.h"
#include "agents/vehicles/VehicleConstants.h"
#include "utils/loggers/Logger.h"

FindNearestAmenityPerson::FindNearestAmenityPerson(QDateTime born_date, PersonConstants::gender gender) : Person(born_date , gender){
    QVector<PgRoutingConstants::road_types> roads;

    roads.append(PgRoutingConstants::h_primary);
    roads.append(PgRoutingConstants::h_primary_link);
    roads.append(PgRoutingConstants::h_secondary);
    roads.append(PgRoutingConstants::h_secondary_link);
    roads.append(PgRoutingConstants::h_tertiary);
    roads.append(PgRoutingConstants::h_tertiary_link);
    roads.append(PgRoutingConstants::h_residential);
    roads.append(PgRoutingConstants::h_service);
    roads.append(PgRoutingConstants::h_unclassified);
    roads.append(PgRoutingConstants::h_road);
    roads.append(PgRoutingConstants::j_roundabout);
    roads.append(PgRoutingConstants::h_footway);
    roads.append(PgRoutingConstants::h_living_street);
    roads.append(PgRoutingConstants::h_path);
    roads.append(PgRoutingConstants::h_pedestrian);
    roads.append(PgRoutingConstants::h_steps);
    roads.append(PgRoutingConstants::h_track);

    if (QDateTime::currentDateTime().date().year() - this->getBornDatetime().date().year() < 12){
        roads.removeOne(PgRoutingConstants::h_path);
        this->speed = 1.42 / 2.75; // 5km/h / 2.75
    } else if (QDateTime::currentDateTime().date().year() - this->getBornDatetime().date().year() < 30){
        this->speed = 1.42; // 5km/h
    } else if (QDateTime::currentDateTime().date().year() - this->getBornDatetime().date().year() < 45){
        this->speed = 1.42 / 1.25; // 5km/h / 1.25
    } else if (QDateTime::currentDateTime().date().year() - this->getBornDatetime().date().year() < 60){
        this->speed = 1.42 / 2; // 5km/h / 2
    } else {
        roads.removeOne(PgRoutingConstants::h_path);
        roads.removeOne(PgRoutingConstants::h_steps);
        this->speed = 1.42 / 3.25; // 5km/h / 3.25
    }

    this->near_amenities_skill = new NearAgentsSkill(this);
    this->near_amenities_routing_skill = new  PgRoutingNearAgentsSkill(this , roads);
    this->elevation_skill = new ElevationSkill(this);

}

FindNearestAmenityPerson::~FindNearestAmenityPerson(){
    delete this->near_amenities_skill;
    delete this->near_amenities_routing_skill;
    delete this->elevation_skill;
}

/**********************************************************************
AGENT METHODS
**********************************************************************/

void FindNearestAmenityPerson::run(){
    try{
        QList<Agent*> amenities = Environment::getInstance()->getAgents("building");
        if( amenities.length() ){
            QMap<double, Agent*> direct_ordered = this->near_amenities_skill->orderByDistance( amenities );
            QMap<GraphRoute*, Agent*> ordered = this->near_amenities_routing_skill->orderByDistance( amenities , Environment::getInstance()->getGeometryFactory());
            /*double total_seconds = 0;
            while( ordered.firstKey()->nextEdge() ){
                double edge_speed = this->speed;
                GraphEdge* edge = ordered.firstKey()->currentEdge();
                double elev1 = this->elevation_skill->getPointElevation( edge->getPoints().first() );
                double elev2 = this->elevation_skill->getPointElevation( edge->getPoints().last() );
                double gradient = this->elevation_skill->calculateSlopeGradient( elev2 - elev1 , edge->getLength());
                // http://en.wikipedia.org/wiki/Naismith%27s_rule
                if (gradient > 12){ // Up the hill
                    edge_speed  = edge_speed - 0.5; //
                }
                if (gradient < -5){ // Down the hill
                    edge_speed = edge_speed + 0.5 ;
                }
                total_seconds += edge->getLength() / edge_speed;
            }*/

            Logger::log(QString(" %1,%2,%3,%4,%5,%6,%7,%8 ")
                        .arg(this->getId())
                        .arg( this->getName() )
                        .arg( QDateTime::currentDateTime().date().year() - this->getBornDatetime().date().year() )
                        .arg( this->getGender() )
                        .arg( this->speed )
                        .arg( direct_ordered.firstKey() / 1.12 ) // 4km/h = 1.12m/s
                        .arg( ordered.firstKey()->getTotalLength() / 1.12 ) // 4km/h = 1.12m/s
                        .arg( ordered.firstKey()->getTotalLength() / this->speed ) );
        }
    } catch(...){}
}
