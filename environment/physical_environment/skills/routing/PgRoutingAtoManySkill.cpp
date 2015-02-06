#include <QDebug>
#include "PgRoutingAtoManySkill.h"

PgRoutingAtoManySkill::PgRoutingAtoManySkill(Agent *agent, QVector<PgRoutingConstants::road_types> roads_to_use , double reverse_factor) : PgRoutingBaseSkill(agent, roads_to_use , reverse_factor){
}

PgRoutingAtoManySkill::~PgRoutingAtoManySkill() {

}

/**********************************************************************
SKILL METHODS
**********************************************************************/

QVector<GraphRoute*> PgRoutingAtoManySkill::getAtoManyRoute(Point *starting_point, QVector<Point *> ending_points, const GeometryFactory *factory){
    QVector<GraphRoute*> routes;
    QMap<QString, Point*> snapped_to_real;

    if( ending_points.isEmpty() ){
        qWarning() << "No ending points" << endl;
        return routes;
    }

    // Connect to DB
    if( !this->getDriver()->connectDB() ){
        return routes;
    }

    geos::io::WKTReader* reader = new geos::io::WKTReader(factory);
    try{
        this->createTempRoutingTables();

        // PGROUTING WRAPPER (See : doc/geosmartsim_roads AND http://workshop.pgrouting.org/chapters/wrapper.html)
        // Delete previous temp table
        // Calculate route, make one line with all segments and insert it in temp table
        // Separate the line into segments and length from temp table

        QString endings_array;
        foreach(Point* point , ending_points){
            if (endings_array.isEmpty()){
                endings_array.append( "ARRAY[" );
            } else {
                endings_array.append(",");
            }
            Point* snapped = this->getGraphNodeGeomForPoint( point );
            snapped_to_real.insert( QString::fromStdString( snapped->toString() ) , point);
            endings_array.append( QString("[%1 , %2]").arg( snapped->getCoordinate()->x ).arg( snapped->getCoordinate()->y ) );
        }
        endings_array.append( "]" );

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
                    pgr_fromAtoMany('%1', '%2', %3, %4, %5, %6) \
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
                .arg(endings_array)                                             // Ending points as array ARRAY[[1,1],[2,2]]
                .arg(  this->reverse_factor );                             // Reverse factor

        QListIterator<QSqlRecord> i(this->getDriver()->executeCustomQuery(sql));
        GraphRoute* route = new GraphRoute(dynamic_cast<Point*>(starting_point->clone()) , 0 );

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
        int seq = 0; // To know when we receive a new graphroute
        route->addEdge(from_entrance_to_road);
        route->addEdge(from_road_to_routestart);

        // Store last created edge
        Point* last_point = starting_point;

        while( i.hasNext() ){
            QSqlRecord record = i.next();

            if ( record.value("seq").toInt() < seq){
                // In this case we are receiving a new route
                // Set last built routes end point and append
                Point* real_point = snapped_to_real.value( QString::fromStdString( last_point->toString() ) );
                if (real_point){
                    route->setEndingPoint( real_point );
                }

                routes.append(route);
                // Build new route
                route = new GraphRoute(dynamic_cast<Point*>(starting_point->clone()) , dynamic_cast<Point*>(starting_point->clone()) );
                route->addEdge(from_entrance_to_road);
                route->addEdge(from_road_to_routestart);
                last_point = starting_point;
            }

            seq = record.value("seq").toInt();
            LineString* line = dynamic_cast<LineString*> ( reader->read( record.value("st_astext").toString().toStdString() ) );
            if (line){
                last_point = line->getEndPoint();
                Road* road = this->createOrReuseRoadForGeom( line , PgRoutingConstants::roadClassToRoadType( record.value("road_type").toInt() ) );
                if (road) {
                    road->setMaxSpeed( record.value("maxspeed_forward").toDouble() * 0.2777777 ); // To m/s
                    road->setLength( record.value("st_length").toDouble() );
                    route->addEdge(road);
                }
            }

        }
        // Set last built routes end point and append
        Point* real_point = snapped_to_real.value( QString::fromStdString( last_point->toString() ) );
        if (real_point){
            route->setEndingPoint( real_point );
        }
        routes.append(route);

    } catch(...){}

    // Disconnect from DB
    this->getDriver()->disconnectDB();

    delete reader;
    return routes;
}
