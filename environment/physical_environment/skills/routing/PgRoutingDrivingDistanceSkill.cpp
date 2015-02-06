#include "PgRoutingDrivingDistanceSkill.h"
#include <QDebug>

PgRoutingDrivingDistanceSkill::PgRoutingDrivingDistanceSkill(Agent* agent , QVector<PgRoutingConstants::road_types> roads, double reverse_factor) : PgRoutingBaseSkill(agent, roads, reverse_factor){
}

PgRoutingDrivingDistanceSkill::~PgRoutingDrivingDistanceSkill(){
}

/**********************************************************************
SKILL METHODS
**********************************************************************/

/** GET ALL REACHABLE ROADS FROM STARTING POINT IN X METERS */
QVector<GraphRoute*> PgRoutingDrivingDistanceSkill::getDrivingDistanceInMeters(Point *starting_point, int meters, const GeometryFactory *factory){
    QVector<GraphRoute*> routes;

    // Connect to DB
    if( !this->getDriver()->connectDB() ){
        return routes;
    }

    this->createTempRoutingTables();

    // PGROUTING WRAPPER (See : doc/geosmartsim_roads AND http://workshop.pgrouting.org/chapters/wrapper.html)
    // Delete previous temp table
    // Calculate route, make one line with all segments and insert it in temp table
    // Separate the line into segments and length from temp table

    QString sql = QString("\
                          DROP TABLE IF EXISTS temp_route; \
            CREATE TEMP TABLE temp_route AS ( \
                SELECT \
                seq AS seq, \
                the_geom AS the_geom, \
                length AS length, \
                road_type AS road_type, \
                maxspeed_forward AS maxspeed_forward \
                FROM \
                pgr_fromAReachableMeters('%1', '%2', %3, %4, %5, %6) \
                );\
    \
    SELECT \
            ST_AsText( ST_MakeLine(sp,ep) ) AS st_astext, \
            ST_Length( ST_MakeLine(sp,ep)::geography ) AS st_length, \
            seq AS seq, \
            road_type AS road_type, \
            maxspeed_forward AS maxspeed_forward \
            FROM \
            ( \
                SELECT ST_pointn(the_geom, generate_series(1, ST_npoints(the_geom)-1)) AS sp, \
                ST_pointn(the_geom, generate_series(2, ST_npoints(the_geom)  )) AS ep, \
                length AS length, \
                seq AS seq, \
                road_type AS road_type, \
                maxspeed_forward AS maxspeed_forward \
                FROM temp_route ) AS foo;")
    .arg(pgr_table_name)                                         // Table name
            .arg(pgr_index_table_name)                            // Node index table name
            .arg(starting_point->getX())                              // Start point X
            .arg(starting_point->getY())                              // Start point Y
            .arg( meters )                                                          // Distance in meters
            .arg(  this->reverse_factor );                             // Reverse factor

    QListIterator<QSqlRecord> i(this->getDriver()->executeCustomQuery(sql));
    GraphRoute* route = new GraphRoute(dynamic_cast<Point*>(starting_point->clone()) , dynamic_cast<Point*>(starting_point->clone()) );

    // Make edge from starting point to nearest road point
    CoordinateSequence* sequence1 = route->getStartingPoint()->getCoordinates();
    Point* road_point = this->getNearestRoadPointToGeometry(route->getStartingPoint());
    sequence1->add( *this->getNearestRoadPointToGeometry(route->getStartingPoint())->getCoordinate() );
     Road* from_entrance_to_road = this->createOrReuseRoadForGeom( factory->createLineString(sequence1)  , PgRoutingConstants::h_footway );

    // Make edge from nearest road point to route starting point (graph node)
    CoordinateSequence* sequence2 = road_point->clone()->getCoordinates();
    sequence2->add( *this->getGraphNodeGeomForPoint(road_point)->getCoordinate() );
    Road* from_road_to_routestart = this->createOrReuseRoadForGeom( factory->createLineString(sequence2) , PgRoutingConstants::h_footway );

    // Iterate on all the segments
    geos::io::WKTReader* reader = new geos::io::WKTReader(factory);
    route->addEdge(from_entrance_to_road);
    route->addEdge(from_road_to_routestart);

    while(i.hasNext()){
        QSqlRecord record = i.next();

            LineString* line = dynamic_cast<LineString*> ( reader->read( record.value("st_astext").toString().toStdString() ) );
            Road* road = this->createOrReuseRoadForGeom( line , PgRoutingConstants::roadClassToRoadType( record.value("road_type").toInt() ) );
            if (road) {
                road->setMaxSpeed( record.value("maxspeed_forward").toDouble() * 0.2777777 ); // To m/s
                road->setLength( record.value("st_length").toDouble() );
                route->addEdge(road);
            }
    }
    routes.append(route);

    // Disconnect from DB
    this->getDriver()->disconnectDB();

    delete reader;
    return routes;
}
