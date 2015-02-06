#include <QDebug>

#include "environment/Environment.h"
#include "environment/physical_environment/managers/graphs/GraphNetworksManager.h"
#include "PgRoutingBaseSkill.h"

PgRoutingBaseSkill::PgRoutingBaseSkill( Agent* agent , QVector<PgRoutingConstants::road_types> roads_to_use , double reverse_factor ) : AccessDBSkill(agent){
    this->roads_to_use = roads_to_use;
    this->reverse_factor = reverse_factor;
}

PgRoutingBaseSkill::~PgRoutingBaseSkill(){
}

/**********************************************************************
 SETTERS
**********************************************************************/

void PgRoutingBaseSkill::setUsableRoads(QVector<PgRoutingConstants::road_types> roads){
    this->roads_to_use = roads;
}

/**********************************************************************
 SKILL METHODS
**********************************************************************/

QVector<PgRoutingConstants::road_types> PgRoutingBaseSkill::getUsableRoads(){
    return this->roads_to_use;
}

/**********************************************************************
 SKILL METHODS
**********************************************************************/

int PgRoutingBaseSkill::getGraphNodeIdForPoint(Point *point){

    // Connect to DB
    if( !this->getDriver()->connectDB() ){
        return 0;
    }
    this->createTempRoutingTables();

    QString sql = QString("SELECT \
                                                            result.id AS id \
                                                FROM \
                                                            pgr_findNode( '%1' , %2 , %3 ) AS result;")
            .arg(pgr_index_table_name)
            .arg(point->getX())
            .arg(point->getY());
    QListIterator<QSqlRecord> i(this->getDriver()->executeCustomQuery(sql));
    this->getDriver()->disconnectDB();

     while(i.hasNext()){
         return i.next().value("id").toInt();
     }
    return 0;
}

Point* PgRoutingBaseSkill::getGraphNodeGeomForPoint(Point *point) {

    // Connect to DB
    if( !this->getDriver()->connectDB() ){
        return 0;
    }
    this->createTempRoutingTables();

    QString sql = QString("SELECT \
                                ST_ASTEXT(result.the_geom) AS st_astext \
                            FROM \
                                pgr_findNode( '%1' , %2 , %3 ) AS result;")
            .arg(pgr_index_table_name)
            .arg(point->getX())
            .arg(point->getY());
    QListIterator<QSqlRecord> i(this->getDriver()->executeCustomQuery(sql));
    geos::io::WKTReader* reader = new geos::io::WKTReader( Environment::getInstance()->getGeometryFactory() );
    this->getDriver()->disconnectDB();

     while( i.hasNext() ) {
         return dynamic_cast<Point*> ( reader->read( i.next().value("st_astext").toString().toStdString() ) );
     }
    return 0;
}

Point* PgRoutingBaseSkill::getNearestRoadPointToGeometry(Geometry *geometry){

    // Connect to DB
    if( !this->getDriver()->connectDB() ){
        return 0;
    }
    this->createTempRoutingTables();

    QString sql = QString("SELECT \
                                   ST_ASTEXT( ST_ClosestPoint( nearestroad.the_geom , ST_GEOMFROMTEXT( '%2' , 4326) ) ) AS st_astext \
                            FROM \
                                    (SELECT \
                                                roads.the_geom\
                                                FROM %1 AS roads , \
                                                ST_GEOMFROMTEXT( '%2' , 4326 ) AS geometry \
                                                ORDER BY roads.the_geom <-> geometry LIMIT 1 \
                                    ) AS nearestroad")
            .arg(pgr_table_name)
            .arg(QString::fromStdString( geometry->toString() ));

    QListIterator<QSqlRecord> i(this->getDriver()->executeCustomQuery(sql));
     geos::io::WKTReader* reader = new geos::io::WKTReader( Environment::getInstance()->getGeometryFactory() );
     this->getDriver()->disconnectDB();

     while(i.hasNext()){
        return dynamic_cast<Point*> ( reader->read( i.next().value("st_astext").toString().toStdString() ) );
     }
    return 0;
}

void PgRoutingBaseSkill::createTempRoutingTables(){
    if(this->roads_to_use.isEmpty()){
        qWarning() << "No road types to create graph";
        return;
    }
    // Create pgrouting table with only the usable roads in the environment bounds
    // Transform the road types this transport can use, to a sql string
    QString roadIdsAsString;
    foreach(int i , this->roads_to_use){
        if(!roadIdsAsString.isEmpty()){
            roadIdsAsString += " , ";
        }
        roadIdsAsString += QString::number( i );
    }
    // Create temp tables for environment bounds and filtering only wanted road types
    this->getDriver()->createEnvironmentBoundsTempTable("geosmartsim_roads", pgr_table_name, "the_geom",
                                                         QString(" class_id IN ( %1 )").arg(roadIdsAsString) );
    this->getDriver()->createEnvironmentBoundsTempTable("geosmartsim_roads_index", pgr_index_table_name, "the_geom",
                                                        QString(" class_ids && ARRAY[ %1 ]").arg(roadIdsAsString) ); // && = overlap/have elements in common operand
}

Road* PgRoutingBaseSkill::createOrReuseRoadForGeom(LineString *line, PgRoutingConstants::road_types type){
    GraphEdge* edge = GraphNetworksManager::getEdge(line); // See if edge already exists
    Road* road = 0;
    // If there is no edge
    if( !edge ){
        edge = GraphNetworksManager::createEdge(line);
        road = new Road( *edge , type );
        GraphNetworksManager::insertEdge(road , true); // Insert the road class
        edge->deleteLater();
    } else {
        // Else we have received a road
        road = dynamic_cast<Road*> (edge);
        delete line;
    }
    return road;
}
