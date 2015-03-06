#include "Building.h"
#include "environment/Environment.h"

Building::Building(int levels,  QString class_name) : Agent(class_name){
    this->levels = levels;
    this->inhabitants_contain_skill = new AgentContainSkill(this);
    this->routing_skill = 0;
}

Building::~Building(){
}

QJsonObject Building::customGeoJSONProperties(){
    QJsonObject properties = Agent::customGeoJSONProperties();
    properties.insert("height", this->getLevels() * 3);
    properties.insert("levels", this->getLevels() );
    properties.insert("inside", int(this->inhabitants_contain_skill->getAmountInside()));
    return properties;
}

/**********************************************************************
 GETTERS
**********************************************************************/

int Building::getCapacity(){
   return this->inhabitants_contain_skill->getMaxCapacity();
}

int Building::getLevels(){
    return this->levels;
}

double Building::getArea(){
    return this->area;
}

QList<Agent*> Building::getInhabitants(){
    return this->inhabitants_contain_skill->getAgentsInside();
}

QVector<Point*> Building::getEntrances(){
    return this->entrances;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void Building::setCapacity(int inhabitants){
    this->inhabitants_contain_skill->setMaxCapacity(inhabitants);
}

void Building::addInhabitant(Person* person){
    this->inhabitants_contain_skill->enter(person);
}

void Building::addEntrance(Point *entrance){
    this->entrances.append(entrance);
}

void Building::setArea(double area){
    this->area = area;
}

/**********************************************************************
 ENTITY METHODS
**********************************************************************/

void Building::loadEntrances(QVector<PgRoutingConstants::road_types> roads_to_use , QString entrance_where){

    bool created = false;
    if (!this->routing_skill){
         this->routing_skill = new PgRoutingAtoManySkill(this , roads_to_use);
        created = true;
    }
 this->routing_skill->getDriver()->connectDB();

    this->routing_skill->getDriver()->createEnvironmentBoundsTempTable("geosmartsim_lines", "lines", "the_geom");
    this->routing_skill->getDriver()->createEnvironmentBoundsTempTable("geosmartsim_points", "points", "the_geom");

    QString sql = QString("SELECT \
                          ST_ASTEXT( entrances.the_geom ) AS st_astext \
                          FROM %1 AS entrances \
                          WHERE \
                          ST_INTERSECTS( ST_BUFFER( ST_GEOMFROMTEXT( '%2' , 4326 )::geography , 1) , entrances.the_geom) \
                          AND ( %3 ) ")
                          .arg("points")
                          .arg(QString::fromStdString( this->getGeometry()->toString() ))
                          .arg( entrance_where );
            QListIterator<QSqlRecord> i(this->routing_skill->getDriver()->executeCustomQuery(sql));
    while(i.hasNext()){
        QSqlRecord record = i.next();
        Point* entrance = dynamic_cast<Point*>( Environment::getInstance()->getGeometryFromWKT( record.value("st_astext").toString() ));
        this->addEntrance( entrance );
    }

    // Get roads that cross the amenity (for parks and landuses)
    sql = QString("SELECT \
                  ST_ASTEXT( (points).geom) AS st_astext \
                  FROM \
                  (SELECT ST_DumpPoints( ST_INTERSECTION( ST_ExteriorRing( ST_GEOMFROMTEXT( '%2' , 4326 ) ) , roads.the_geom ) ) AS points \
                   FROM %1 AS roads \
                   WHERE \
                   ST_Intersects( ST_GEOMFROMTEXT( '%2' , 4326 ) , roads.the_geom) \
                   AND \
                   roads.tags ? 'highway' \
                                ) AS foo ")
                  .arg("lines")
                  .arg(QString::fromStdString( this->getGeometry()->toString() ));
            QListIterator<QSqlRecord> m(this->routing_skill->getDriver()->executeCustomQuery(sql));
    while(m.hasNext()){
        QSqlRecord record = m.next();
        Point* entrance = dynamic_cast<Point*>(Environment::getInstance()->getGeometryFromWKT( record.value("st_astext").toString() ));
        this->addEntrance( entrance );
    }

    // If still the amenity has no entrance
    if ( this->getEntrances().isEmpty() ){
        // Get the nearest point to a road
        this->addEntrance( this->routing_skill->getNearestRoadPointToGeometry( this->getGeometry() ) );
    }

    this->routing_skill->getDriver()->disconnectDB();
    if( created ){
            delete this->routing_skill;
    }
}
