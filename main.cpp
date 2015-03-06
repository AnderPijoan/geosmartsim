#include <QCoreApplication>
#include <QDebug>
#include <geos/geom/LineString.h>

#include "environment/Environment.h"
#include "usecases/home_to_work_people/HomeToWork.h"
#include "environment/skills/graph/DijkstraSkill.h"
#include "utils/db/PostgresDriver.h"

#include "agents/graphs/roads/Road.h"

using namespace geos;

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    QCoreApplication app(argc, argv);

    // CREATE ENVIRONMENT
    Environment * environment = Environment::getInstance();
    environment->setBounds(
                environment->getGeometryFromWKT(
                    QString("\
                            POLYGON((-2.966437339782715 43.39470154352386,-2.966437339782715 43.37212017346092,-2.996220588684082 43.37212017346092,-2.996220588684082 43.39470154352386,-2.966437339782715 43.39470154352386)) \
                            ") ) );
                            environment->setMaxParallelAgents( 1000 );
                    environment->setEnvironmentTime( QDateTime::currentDateTime(), 1 );


                // CREATE HTTP AND SOCKET SERVER FOR FRONTEND
                environment->createHttpServer( 3000 );
            environment->createWebSocketServer( 3001 );

    // TEST GRAPH EXAMPLE

    PostgresDriver::initDB("energia.deusto.es", 5432, "bizkaia", "gisuser", "gisuser");
    PostgresDriver* driver = new PostgresDriver("main-connection");
    driver->connectDB();

    QList<GraphEdge*> roads;

    QListIterator<QSqlRecord> i(
                driver->executeCustomQuery(
                    QString("SELECT \
                                ST_ASTEXT(the_geom) AS the_geom, to_cost AS cost, one_way AS one_way \
                                FROM geosmartsim_roads \
                                WHERE ST_INTERSECTS( the_geom , ST_GEOMFROMTEXT( '%1' , 4326) ) AND \
                                class_id < 200")
                    .arg( QString::fromStdString(environment->getBounds()->toString()) )));
    while( i.hasNext() ){
        QSqlRecord record = i.next();

        try{
            LineString* line = dynamic_cast<LineString*>(environment->getGeometryFromWKT( record.value("the_geom").toString() ) );
                for(int p = 0; p < line->getNumPoints()-1; p++){

                    double cost = record.value( cost ).toDouble();
                    GraphNode* start = new GraphNode();
                    start->setGeometry( line->getPointN( p ) );

                    GraphNode* end = new GraphNode();
                    end->setGeometry( line->getPointN( p+1 ) );

                    // One direction

                    Road* road = new Road(RoadConstants::h_unclassified , start , end);
                    road->setGeometry(
                                environment->getGeometryFromWKT(
                                    QString("LINESTRING(%1 %2, %3 %4)")
                                    .arg( line->getPointN( p )->getX() )
                                    .arg( line->getPointN( p )->getY() )
                                    .arg( line->getPointN( p+1 )->getX() )
                                    .arg( line->getPointN( p+1 )->getY() )
                                    ) );
                    road->setWeight( line->getLength() );
                    environment->addAgent(road);
                    roads.append( road );
                    if ( !record.value("one_way").toBool() ){
                        // Reverse direction
                        road = new Road(RoadConstants::h_unclassified , end , start);
                        road->setGeometry(
                                    environment->getGeometryFromWKT(
                                                QString("LINESTRING(%1 %2, %3 %4)")
                                                .arg( line->getPointN( p+1 )->getX() )
                                                .arg( line->getPointN( p+1 )->getY() )
                                                .arg( line->getPointN( p )->getX() )
                                                .arg( line->getPointN( p )->getY() )
                                                ) );
                        road->setWeight( line->getLength() );
                        environment->addAgent(road);
                        roads.append( road );
                    }
            }
        } catch(...){}
    }

    int amount = 400;
    for(int i = 0; i < amount; i++){
        HomeToWork* hometowork = new HomeToWork(VehicleConstants::car);
        hometowork->setStartingPoint( environment->getRandomPoint( qrand() ) );
        hometowork->setRoads( roads );
        hometowork->setMaxSpeed( hometowork->getId() % 10 );
        environment->addAgent( hometowork );
    }

    qDebug() << "GEOSMARTSIM ready" << endl;

    // EXECUTE APP
    app.exec();
}
