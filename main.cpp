#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>

#include "environment/Environment.h"

using namespace geos;

int main(int argc, char *argv[]){
    QCoreApplication app(argc, argv);

    // INIT DB
    PostgresDriver::initDB("****", "****", "****", "****", 20);

    // CREATE ENVIRONMENT
    Environment * environment = Environment::getInstance();
    environment->setBounds(
                QString("\
                POLYGON((-2.9632186889648438 43.28472849204868,-2.9572105407714844 43.28210423663192,-2.9481124877929688 43.27922992224204,-2.9378128051757812 43.27548061244783,-2.927684783935547 43.27485570502189,-2.9179000854492188 43.27023119062441,-2.9058837890625 43.26498131600872,-2.8967857360839844 43.26260622398263,-2.8887176513671875 43.25848084391599,-2.889232635498047 43.254480208420766,-2.8952407836914062 43.25373006000424,-2.8998756408691406 43.25022924521626,-2.901935577392578 43.2472283866619,-2.9077720642089844 43.24510268906883,-2.914295196533203 43.24460251414909,-2.9203033447265625 43.242726821624444,-2.9287147521972656 43.243352058882756,-2.940387725830078 43.24297691729779,-2.951030731201172 43.24472755826403,-2.962017059326172 43.24935401007663,-2.966480255126953 43.25385508538202,-2.9673385620117188 43.26135613832911,-2.9693984985351562 43.265356322068655,-2.975921630859375 43.26860627781872,-2.9764366149902344 43.27360587091847,-2.9754066467285156 43.277855202141474,-2.971973419189453 43.28247913719357,-2.9632186889648438 43.28472849204868)) \
                 "));
                environment->setMaxParallelAgents( 100 );
                environment->setEnvironmentTime( QDateTime::currentDateTime(), 1 );

    // CREATE HTTP AND SOCKET SERVER FOR FRONTEND
    environment->createHttpServer( 3000 );
    environment->createWebSocketServer( 3001 );
    
    // INSERT MAGIC HERE

    qDebug() << "GEOSMARTSIM ready" << endl;

    // EXECUTE APP
    app.exec();
}
