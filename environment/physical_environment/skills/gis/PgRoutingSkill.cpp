#include <QDebug>
#include <QtCore/qmath.h>
#include <QSqlRecord>
#include <geos/geom/LineString.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/io/WKTReader.h>

#include "environment/Environment.h"
#include "PgRoutingSkill.h"

PgRoutingSkill::PgRoutingSkill(BaseEntity* entity) : AccessDBSkill(entity)
{
    this->max_speed = 27; // m/s
    this->starting_point = Environment::getInstance()->getRandomPoint(qrand());
    this->ending_point = Environment::getInstance()->getRandomPoint(qrand());
    this->route_coordinates = 0;
    this->current_point_index = 0;
}

PgRoutingSkill::~PgRoutingSkill(){
    delete this->starting_point;
    delete this->ending_point;
    delete this->route_coordinates;
}

void PgRoutingSkill::parseBodyVariables(QMap<QString,QVariant> body){
    if (body.contains("pgr_max_speed")){
        this->max_speed = body["max_speed"].toDouble();
    }
    if (body.contains("pgr_starting_point_x")){
        double y = this->starting_point->getY();
        delete this->starting_point;
        Coordinate coor(body["pgr_starting_point_x"].toDouble(), y);
        this->starting_point = Environment::getInstance()->getGeometryFactory()->createPoint(coor);
    }
    if (body.contains("pgr_starting_point_y")){
        double x = this->starting_point->getX();
        delete this->starting_point;
        Coordinate coor(x, body["pgr_starting_point_y"].toDouble());
        this->starting_point = Environment::getInstance()->getGeometryFactory()->createPoint(coor);
    }
    if (body.contains("pgr_ending_point_x")){
        double y = this->ending_point->getY();
        delete this->ending_point;
        Coordinate coor(body["pgr_ending_point_x"].toDouble(), y);
        this->ending_point = Environment::getInstance()->getGeometryFactory()->createPoint(coor);
    }
    if (body.contains("pgr_ending_point_y")){
        double x = this->ending_point->getX();
        delete this->ending_point;
        Coordinate coor(x, body["pgr_ending_point_y"].toDouble());
        this->ending_point = Environment::getInstance()->getGeometryFactory()->createPoint(coor);
    }
}

/** TRACE SHORTEST ROUTE FROM A TO B
 * @brief PgRoutingSkill::traceRouteFromStartToEnd
 * @param factory
 * @return
 */
bool PgRoutingSkill::traceRouteFromStartToEnd(GeometryFactory* factory){
    // Clear old route
    delete this->route_coordinates;
    this->route_lengths.clear();
    this->route_speeds.clear();
    this->route_types.clear();
    this->current_point_index = 0;

    if(this->starting_point->equals(this->ending_point)){
        qDebug() << "Same start and end point"
                 << this->starting_point->getX()
                 << this->starting_point->getY() << endl;
        return false;
    }    

    // Connect to DB
    if(!this->getDriver()->connectDB()){
        return false;
    }

    QString roadIdsAsString;
    QVector<int> roadIds = this->roadsForTransports(this->transports_to_use);
    for(int i = 0; i < roadIds.size(); i++){
        if(!roadIdsAsString.isEmpty()){
            roadIdsAsString += " , ";
        }
        roadIdsAsString += QString::number(roadIds.at(i));
    }

    this->getDriver()->createTempTable("geosmartsim_roads", "pgr_skill_roads", "the_geom");

    // PGROUTING WRAPPER (See : doc/geosmartsim_roads AND http://workshop.pgrouting.org/chapters/wrapper.html)
    // Delete previous temp table
    // Calculate route, make one line with all segments and insert it in temp table
    // Separate the line into segments and length from temp table

        QString sql = QString("\
                  CREATE TEMP TABLE temp_route AS ( \
                        SELECT \
                            the_geom AS the_geom, \
                            length AS length, \
                            road_type AS road_type, \
                            maxspeed_forward AS maxspeed_forward \
                        FROM \
                            pgr_fromAtoB('%1', %2, %3, %4, %5, %6, '%7') \
                            );\
                  \
                  SELECT \
                        ST_AsText( ST_MakeLine(sp,ep) ) AS st_astext, \
                        ST_Length( ST_MakeLine(sp,ep)::geography ) AS st_length, \
                        road_type AS road_type, \
                        maxspeed_forward AS maxspeed_forward \
                  FROM \
                  ( \
                        SELECT ST_pointn(the_geom, generate_series(1, ST_npoints(the_geom)-1)) AS sp, \
                        ST_pointn(the_geom, generate_series(2, ST_npoints(the_geom)  )) AS ep, \
                        length AS length, \
                        road_type AS road_type, \
                        maxspeed_forward AS maxspeed_forward \
                        FROM temp_route ) AS foo;")
            .arg("pgr_skill_roads")                                     // Table name
            .arg(this->starting_point->getX())                      // Start point X
            .arg(this->starting_point->getY())                      // Start point Y
            .arg(this->ending_point->getX())                        // End point X
            .arg(this->ending_point->getY())                        // End point Y
            .arg(1)                                                 // Reverse factor
            .arg(roadIdsAsString);    // Usable road classes ids

        //qDebug()<<sql;

    QList<QSqlRecord> temp = this->getDriver()->executeCustomQuery(sql);
    qDebug() << QString::number(temp.length());
    QListIterator<QSqlRecord> i(temp);

    // Disconnect from DB
    this->getDriver()->disconnectDB();

    // Iterate on all the segments
    geos::io::WKTReader* reader = new geos::io::WKTReader(factory);
    while(i.hasNext()){

        QSqlRecord record = i.next();
        LineString* line = dynamic_cast<LineString*> (reader->read(
                    record.value("st_astext").toString().toStdString()));

        if (this->route_coordinates == 0){
            delete this->starting_point;
            this->starting_point = line->getStartPoint();
            this->route_coordinates = line->getCoordinates();
        } else {
            this->route_coordinates->add(line->getCoordinates()->back());
            delete this->ending_point;
            this->ending_point = line->getEndPoint();
        }
        this->route_lengths.append(
                    record.value("st_length").toDouble());
        this->route_speeds.append(
                    record.value("maxspeed_forward").toDouble() * 0.2777777 ); // To m/s
        this->route_types.append(
                    this->pgrRoadClassToRoadType(record.value("road_type").toInt()));

        delete line;
    }
    delete reader;

    return this->route_coordinates != 0;
}

bool PgRoutingSkill::traceRouteFromStartToEndTSP(GeometryFactory* factory){
    // Clear old route
    delete this->route_coordinates;
    this->route_lengths.clear();
    this->route_speeds.clear();
    this->route_types.clear();
    this->current_point_index = 0;

    // Connect to DB
    if(!this->getDriver()->connectDB()){
        return false;
    }
}

/**
 * @brief PgRoutingSkill::moveToNextPoint
 * Move agent to next point in route (current_point_index++)
 * @param wait Stop thread the amount of time the moving would
 * need in the real world.
 * @return
 */
bool PgRoutingSkill::moveToNextPoint(bool wait){
    if(this->current_point_index >= this->route_lengths.size() ||
            this->current_point_index >= this->route_speeds.size() ||
            this->current_point_index >= this->route_types.size()){
        return false;
    }

    // Wait the time the agent would need to move the the point
    if (wait){
        double speed = qMin(
                    this->route_speeds.at(this->current_point_index),
                    this->max_speed);

        double seconds = (this->route_lengths.at(this->current_point_index) / speed);
        QThread::sleep(Time::secondsToEnvironmentTime(seconds));
    }

    this->current_point_index++;
    return true;
}

/**********************************************************************
 PRIVATES
**********************************************************************/

QVector<int> PgRoutingSkill::roadsForTransports(QVector<PgRoutingSkill::transport_methods> transports){
    // SEE /DOC/GEOSMARTSIM_ROADS mapconfig_for_transport.xml
    QVector<int> road_classes;
    for (int i = 0; i < transports.size(); i++){
        switch(transports.at(i)){
        case train:
            road_classes << 501 << 502 << 503 << 504 << 505 << 506 << 507;
            break;
        case bus:
        case truck:
            road_classes << 101 << 102 << 103 << 104 << 105 << 106 << 107 << 108 << 109 << 110 << 111 << 112 << 113 << 114 << 198 << 199 << 401;
            break;
        case shared_car:
        case car:
            road_classes << 101 << 102 << 103 << 104 << 105 << 106 << 107 << 108 << 109 << 110 << 111 << 112 << 113 << 114 << 115 << 116 << 198 << 199 << 301 << 302 << 303 << 304 << 305 << 401;
            break;
        case motorbike:
            road_classes << 101 << 102 << 103 << 104 << 105 << 106 << 107 << 108 << 109 << 110 << 111 << 112 << 113 << 114 << 115 << 116 << 198 << 199 << 301 << 302 << 303 << 304 << 305 << 401;
            break;
        case bike:
            road_classes << 106 << 107 << 108 << 109 << 110 << 111 << 112 << 113 << 114 << 115 << 116 << 117 << 198 << 199 << 201 << 202 << 203 << 204 << 301 << 302 << 303 << 304 << 305 << 401;
            break;
        case foot:
            road_classes << 106 << 107 << 108 << 109 << 110 << 111 << 112 << 113 << 114 << 115 << 116 << 118 << 119 << 120 << 121 << 198 << 199 << 201 << 202 << 203 << 204 << 301 << 302 << 303 << 304 << 305 << 401;
            break;
        }
    }
    return road_classes;
}

PgRoutingSkill::road_types PgRoutingSkill::pgrRoadClassToRoadType(int id){
    switch(id){
    case 101: case 102: case 103: return motorway;
    case 104: case 105: return trunk;
    case 106: case 107: return primary;
    case 108: case 109: return secondary;
    case 110: case 111: return tertiary;
    case 112: return residential;
    case 113: return living_street;
    case 114: return service;
    case 115: return track;
    case 116: case 301: case 302: case 303: case 304: case 305: return path;
    case 117: case 201: case 202: case 203: case 204: return cycleway;
    case 118: return bridleway;
    case 119: return pedestrian;
    case 120: return footway;
    case 121: return steps;
    case 198: default: return unclassified;
    case 199: return road;
    case 501: case 502: case 503: case 504: case 505: case 506: case 507: return railway;
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

CoordinateSequence* PgRoutingSkill::getRouteCoordinates(){
    return this->route_coordinates;
}

QVector<double> PgRoutingSkill::getRouteLengths(){
    return this->route_lengths;
}

QVector<PgRoutingSkill::road_types> PgRoutingSkill::getRouteTypes(){
    return this->route_types;
}

 QVector<double> PgRoutingSkill::getRouteSpeeds(){
     return this->route_speeds;
 }

double PgRoutingSkill::getMaxSpeed(){
    return this->max_speed;
}

int PgRoutingSkill::getCurrentRoutePointIndex(){
    return this->current_point_index;
}

Point* PgRoutingSkill::getRouteStartingPoint(){
    return this->starting_point;
}

Point* PgRoutingSkill::getRouteEndingPoint(){
    return this->ending_point;
}

Point* PgRoutingSkill::getCurrentRoutePoint(){
    return Environment::getInstance()->getGeometryFactory()->createPoint(
                this->route_coordinates->getAt(this->current_point_index));
}

double PgRoutingSkill::getLastDrivenDistance(){
    return this->route_lengths.at(this->current_point_index - 1);
}

double PgRoutingSkill::getTotalRouteLength(){
    double total = 0;
    QVectorIterator<double> i(this->route_lengths);
    while(i.hasNext()){
        total += i.next();
    }
    return total;
}

QVector<PgRoutingSkill::transport_methods> PgRoutingSkill::getTransportMethods(){
    return this->transports_to_use;
}


/**********************************************************************
 SETTERS
**********************************************************************/

void PgRoutingSkill::setSpeed(double speed){
    this->max_speed = speed;
}

void PgRoutingSkill::setStartingPoint(Point *point){
    delete this->starting_point;
    this->starting_point = point;
}

void PgRoutingSkill::setEndingPoint(Point *point){
    delete this->ending_point;
    this->ending_point = point;
}

void PgRoutingSkill::setTransportMethods(QVector<PgRoutingSkill::transport_methods> transport_methods){
    this->transports_to_use = transport_methods;
}
