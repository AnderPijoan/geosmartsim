#include "PgRoutingTspSkill.h"
#include <QDebug>

PgRoutingTspSkill::PgRoutingTspSkill( Agent* entity , QVector<PgRoutingConstants::road_types> roads_to_use , double reverse_factor ) : PgRoutingAtoBSkill(entity , roads_to_use, reverse_factor){
}

PgRoutingTspSkill::~PgRoutingTspSkill(){
}

/**********************************************************************
SKILL METHODS
**********************************************************************/
/**
 * @brief PgRoutingTspSkill::orderThroughPoints Taking into account the set starting and ending points, order a vector
 * of points with TSP algorithm. For TSP to really use routing distance, first we need to calculate the routing distance matrix
 * of every point. For that purposse we will use Kdijkstra from each point to all others.
 * @param through_points
 * @return
 */
QVector<Point*> PgRoutingTspSkill::orderThroughPointsGenetic(Point* starting_point , Point* ending_point, QVector<Point *> through_points){
    QVector<Point*> ordered_through_points; // without starting and ending point
    QVector<int> points_ids; // Points database ids in the order of [START_P + through_points + END_P]
    QMap< int , QMap < int , double > > distance_matrix; // < Source < Target , distance > >

    if(!this->getDriver()->connectDB()){
        qWarning() << "Could not connect to database to order points";
        return ordered_through_points;
    }

        this->createTempRoutingTables();

    // Insert points and reference to their ids
    through_points.prepend( starting_point );
    through_points.append( ending_point );
    foreach(Point* point , through_points){
        points_ids.append( this->getGraphNodeIdForPoint( point ) );
    }

    // Calculate distance matrix for each point with all others
    foreach(int id , points_ids){
        distance_matrix.insert( id , QMap<int, double>() );
        distance_matrix[ id ].insert(id , 0);

        // Transform OTHER points ids to String
        QString pointIdsAsString;
        foreach(int other_id , points_ids ) {
            if ( other_id != id ){
                if(!pointIdsAsString.isEmpty()){
                    pointIdsAsString += " , ";
                }
                pointIdsAsString += QString::number( other_id );
            }
        }

        QString sql = QString("SELECT \
                                                seq, id1 AS source, id2 AS target, cost AS cost FROM pgr_kdijkstraCost( \
                                                'SELECT id, source, target, to_cost AS cost, \
                                                (case \
                                                            when one_way = true then reverse_cost::float * %2 \
                                                            when one_way = false then reverse_cost::float \
                                                end ) AS reverse_cost \
                                                FROM %1', \
                                                %3 , array[ %4 ], true, true \
                                                ); ")
                .arg( pgr_table_name )
                .arg( this->reverse_factor )
                .arg( id )
                .arg( pointIdsAsString );

        // Fetch results
        QListIterator<QSqlRecord> i(this->getDriver()->executeCustomQuery(sql));
        while( i.hasNext() ){
            QSqlRecord record = i.next();
            distance_matrix[ record.value( "source" ).toInt() ].insert( record.value( "target" ).toInt() , record.value( "cost" ).toDouble() );
        }
    }

    // Create the distance matrix in postgres text array format
    QString distancesArrayAsText;
    foreach(int source , points_ids){
        QString str;
        foreach(int target , points_ids ){
            if( !str.isEmpty() ){
                str += ",";
            }
            str += QString::number( distance_matrix[ source ][ target ] );
        }
        if ( !distancesArrayAsText.isEmpty() ){
            distancesArrayAsText += ",";
        }
        distancesArrayAsText += QString("{ %1 }").arg( str );
    }
    QString matrixAsText =
            QString("{ %1 }")
            .arg( distancesArrayAsText );

    // Now use pgr_tsp with the distance matrix to get the order points should be visited
    QString sql = QString("SELECT \
                                                seq, id \
                                                FROM \
                                                pgr_tsp( ' %3 '::float8[] , %1 , %2 ) ")
                          .arg( through_points.indexOf( starting_point ) )
                          .arg( through_points.indexOf( ending_point ) )
                          .arg( matrixAsText );

            // Get the ordered points
            QListIterator<QSqlRecord> i(this->getDriver()->executeCustomQuery( sql ));
    while( i.hasNext() ){
        QSqlRecord record = i.next();
        qDebug() << record.value("id") << points_ids;
        if ( record.value("id").toInt() != through_points.indexOf( starting_point ) &&
             record.value("id").toInt() != through_points.indexOf( ending_point ) ) {
            ordered_through_points.append(  through_points.at( record.value("id").toInt() ) );
        }
    }
    this->getDriver()->disconnectDB();
    return ordered_through_points;
}

/**
 * @brief PgRoutingTspSkill::orderThroughPoints Taking into account the set starting and ending points, order a vector
 * of points getting the nearest next point.
 * @param through_points
 * @return
 */
QVector<Point*> PgRoutingTspSkill::orderThroughPointsNearest(Point* starting_point, Point* ending_point, QVector<Point *> through_points){
    QVector<Point*> ordered_through_points; // without starting and ending point
    QMap<Point* , int> points_ids; // Points and their database ids

    if(!this->getDriver()->connectDB()){
        qWarning() << "Could not connect to database to order points";
        return ordered_through_points;
    }

    // Insert points and reference to their ids
    through_points.prepend( starting_point );
    through_points.append( ending_point );
    foreach(Point* point , through_points){
        points_ids.insert(point, this->getGraphNodeIdForPoint( point ) );
    }

    // Go point by point getting its nearest untill all are visited
    Point* point_at = starting_point;
    // In the end only one point and ending_point will be inside
    while( points_ids.size() > 2 ){

        // Calculate distance matrix for POINT_AT point with all others
            Point* nearest_point = 0;
            double nearest_cost = std::numeric_limits<double>::max();

            // Transform OTHER points ids to String
            QString pointIdsAsString;
            foreach(int id , points_ids.values() ) {
                if ( id != points_ids[point_at] ){
                    if(!pointIdsAsString.isEmpty()){
                        pointIdsAsString += " , ";
                    }
                    pointIdsAsString += QString::number( id );
                }
            }

            QString sql = QString("SELECT \
                                                    seq, id1 AS source, id2 AS target, cost AS cost FROM pgr_kdijkstraCost( \
                                                    'SELECT id, source, target, to_cost AS cost, \
                                                    (case \
                                                                when one_way = true then reverse_cost::float * %2 \
                                                                when one_way = false then reverse_cost::float \
                                                    end ) AS reverse_cost \
                                                    FROM %1', \
                                                    %3 , array[ %4 ], true, true \
                                                    ); ")
                    .arg( pgr_table_name )
                    .arg(  this->reverse_factor )
                    .arg( points_ids[point_at] )
                    .arg( pointIdsAsString );

            // Fetch results
            QListIterator<QSqlRecord> i(this->getDriver()->executeCustomQuery(sql));
            while( i.hasNext() ) {
                QSqlRecord record = i.next();
                double new_cost = record.value( "cost" ).toDouble();
                Point* new_point = points_ids.key( record.value( "target" ).toInt() );

                if (new_cost < nearest_cost && !new_point->equals( starting_point ) && !new_point->equals( ending_point )) {
                    nearest_cost = new_cost;
                    nearest_point = new_point;
                }
            }

            // Remove the point we where at, set the next and add it to the list
            points_ids.remove(point_at);
            point_at = nearest_point;
            ordered_through_points.append(point_at);
    }

    // Once the while finished, remove the ending_point from point_ids and add the remaining point to the list
    points_ids.remove( ending_point );
    ordered_through_points.append( points_ids.keys().first() );

    this->getDriver()->disconnectDB();
    return ordered_through_points;
}

/**
 * @brief PgRoutingTspSkill::traceRoute Returns the routes from the first throug_point to the last throug_point. DOESNT MAKE THE ROUTE
 * FROM THE STARTING POINT TO THE FIRST THROUG_POINT OR TO THE END_POINT
 * @param through_points
 * @param factory
 * @return
 */
QVector<GraphRoute*> PgRoutingTspSkill::getTSPRoute(Point* starting_point, Point* ending_point, QVector<Point*> through_points, const GeometryFactory* factory){
    QVector<GraphRoute*> routes;

    // Order through_points and then add start and ending points
    QVector<Point*> ordered_through_points = this->orderThroughPointsNearest(starting_point, ending_point, through_points);

    // Wrap the route calculation between a connection for it not to open and close one for every
    // route piece
    if(!this->getDriver()->connectDB()){
        return routes;
    }

         // Now calculate the whole route from the first throug_point, to the last
        for(int i = 0 ; i < ordered_through_points.size()-1 ; i++) {
            starting_point = ordered_through_points.at( i );
            ending_point = ordered_through_points.at( i+1 );
            if ( starting_point && ending_point ){
                routes.append( this->getAtoBRoute(starting_point, ending_point, factory) );
            }
        }

        this->getDriver()->disconnectDB();
        return routes;
}
