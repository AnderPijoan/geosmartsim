#include <QSqlRecord>

#include "ElevationSkill.h"

ElevationSkill::ElevationSkill(Agent* agent) : AccessDBSkill(agent)
{
}

ElevationSkill::~ElevationSkill(){
}

double ElevationSkill::getPointElevation(Point* point){

    // Connect to DB and create temp roads table
    if(this->getDriver()->connectDB()){

        // Query database
        QString sql = QString("\
              SELECT AVG(st_value(elevation.rast, point.the_geom)) AS height \
                FROM \
                %1 AS elevation, \
                ( SELECT ST_GEOMFROMTEXT('POINT(%2 %3)' , 4326) AS the_geom ) AS point \
                WHERE ST_INTERSECTS(rast, point.the_geom)")
            .arg("geosmartsim_elevation")   // Table name
            .arg(point->getX())             // point X
            .arg(point->getY());            // point Y

        QListIterator<QSqlRecord> i(this->getDriver()->executeCustomQuery(sql));

        // Disconnect from DB
        this->getDriver()->disconnectDB();

        // Iterate on all the points
        while(i.hasNext()){
            QSqlRecord record = i.next();
            return record.value("height").toDouble();
        }
    }
    return -1;
}

double ElevationSkill::calculateSlopeGradient(double verticalDisplacement, double horizontalDisplacement){
    return (verticalDisplacement / horizontalDisplacement) * 100;
}
