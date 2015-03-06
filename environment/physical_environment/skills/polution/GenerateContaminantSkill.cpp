#include <QSqlRecord>
#include <QDebug>
#include "GenerateContaminantSkill.h"

GenerateContaminantSkill::GenerateContaminantSkill(
    Agent* agent , VehicleConstants::vehicle_types type, QString technology, QString subsegment) : AccessDBSkill(agent){
    this->vehicle_type = type;
    this->vehicle_technology = technology;
    this->vehicle_subsegment = subsegment;
    this->contaminantAmount = 0;
}

GenerateContaminantSkill::~GenerateContaminantSkill(){
    this->getDriver()->disconnectDB();
}

QMap<QString, double> GenerateContaminantSkill::loadEmissionFactorsFromDB(QString dbname){

    // Connect to DB
    if(this->getDriver()->connectDB()){

        // Query database for emission factor according to vehicle characteristics
        // EFA IS IN g/KM!! convert to meters
        QString sql = QString("SELECT trafficsit, gradient, efa / 1000 as efa, v FROM %5 \
                              WHERE vehcat = '%1' \
                AND technology = '%2' \
                AND subsegment LIKE '%3' \
                AND component = '%4' \
                ")
                .arg(this->vehicleTypeToString( this->vehicle_type ))
                .arg(this->vehicle_technology)
                .arg(this->vehicle_subsegment)
                .arg(this->contaminantName)
                .arg(dbname);

        QListIterator<QSqlRecord> i(this->getDriver()->executeCustomQuery(sql));
        while(i.hasNext()){
            QSqlRecord record = i.next();
            QString key = QString("%1/%2")
                    .arg(record.value("trafficsit").toString())
                    .arg(record.value("gradient").toString());
            if (!this->emissionFactorMap.contains(key)){
                this->emissionFactorMap.insert(key, record.value("efa").toDouble()); // EFA in meters!
            }
        }
        // Disconnect from DB
        this->getDriver()->disconnectDB();

    }
    return this->emissionFactorMap;
}

double GenerateContaminantSkill::sumContaminantToTotal(QString roadCharacteristics, double road_length){
    double emission = 0;
    if (this->emissionFactorMap.contains(roadCharacteristics)) {
        // EFA (g/m) * road length (m)
        emission = this->emissionFactorMap.value(roadCharacteristics) * road_length;
        this->contaminantAmount += emission;
    }
    return emission;
}

GenerateContaminantSkill::efa_road_types GenerateContaminantSkill::pgrTypeToRoadType(PgRoutingConstants::road_types type){
    switch(type){
    case PgRoutingConstants::h_motorway:
    case PgRoutingConstants::h_motorway_link:
    case PgRoutingConstants::h_motorway_junction:
        return motorway;
    case PgRoutingConstants::h_trunk:
    case PgRoutingConstants::h_trunk_link:
        return trunk;
    case PgRoutingConstants::h_primary:
    case PgRoutingConstants::h_primary_link:
        return trunk_primary;
    case PgRoutingConstants::h_secondary:
    case PgRoutingConstants::h_secondary_link:
        return distributor_secondary;
    case PgRoutingConstants::h_tertiary:
    case PgRoutingConstants::h_tertiary_link:
    case PgRoutingConstants::h_road:
    case PgRoutingConstants::h_unclassified:
        return local;
    case PgRoutingConstants::h_residential:
    case PgRoutingConstants::h_service:
    case PgRoutingConstants::h_living_street:
    case PgRoutingConstants::h_track:
    case PgRoutingConstants::h_path:
        return access;
    default:
        return access;
    }
}

QString GenerateContaminantSkill::buildRoadKey(efa_road_types type, int speed, int gradient, GenerateContaminantSkill::traffic_condition_types traffic){
    QString key = "URB/";

    switch (type) {
    case motorway: {
        int limits[] = {80,90,100,110,120,130};
        key += "MW-Nat./" + QString::number(this->buildSpeedKey(speed, limits)) + "/";
        break; }
    case motorway_city: {
        int limits[] = {60,70,80,90,100,110};
        key += "MW-City/" + QString::number(this->buildSpeedKey(speed, limits)) + "/";
        break; }
    case semi_motorway: {
        int limits[] = {90,110};
        key += "Semi-MW/" + QString::number(this->buildSpeedKey(speed, limits)) + "/";
        break; }
    case trunk: {
        int limits[] = {70,80,90,100,110};
        key += "Trunk-Nat./" + QString::number(this->buildSpeedKey(speed, limits)) + "/";
        break; }
    case trunk_primary: {
        int limits[] = {50,60,70,80,90};
        key += "Trunk-City/" + QString::number(this->buildSpeedKey(speed, limits)) + "/";
        break; }
    case distributor_secondary: {
        int limits[] = {50,60,70,80};
        key += "Distr/" + QString::number(this->buildSpeedKey(speed, limits)) + "/";
        break; }
    case distributor_sin: {
        int limits[] = {50,60,70,80,90,100};
        key += "Distr-sin./" + QString::number(this->buildSpeedKey(speed, limits)) + "/";
        break; }
    case local: {
        int limits[] = {50,60};
        key += "Local/" + QString::number(this->buildSpeedKey(speed, limits)) + "/";
        break; }
    case local_sin: {
        int limits[] = {50,60,70,80};
        key += "Local-sin./" + QString::number(this->buildSpeedKey(speed, limits)) + "/";
        break; }
    case access: {
        int limits[] = {30,40,50};
        key += "Access/" + QString::number(this->buildSpeedKey(speed, limits)) + "/";
        break; }
    }

    switch( traffic ){
    default:
    case freeflow: {
        key +="Freeflow/";
        break; }
    case heavy:{
        key += "Heavy/";
        break;
        }
    case saturated:{
        key += "Satur./";
        break;
        }
    case stop_and_go: {
        key +="St+Go/";
        break; }
    }

    if (gradient <= -6){
        key += "-6";
    } else  if (gradient <= -4){
        key += "-4";
    } else if (gradient <= -2){
        key += "-2";
    } else if (gradient <= 0){
        key += "0";
    } else if (gradient <= 2){
        key += "2";
    } else if (gradient <= 4){
        key += "4";
    } else {
        key += "6";
    }
    return key;
}

/**********************************************************************
 PRIVATES
**********************************************************************/

unsigned int GenerateContaminantSkill::buildSpeedKey(int speed, int values[]){
    int last = 0;
    for (unsigned int i = 0; i < sizeof(values); i++){
        last = values[i];
        if(speed <= values[i]){
            return values[i];
        }
    }
    return last;
}

QString GenerateContaminantSkill::vehicleTypeToString(VehicleConstants::vehicle_types type){
    switch (type) {
    case VehicleConstants::car:
        return "pass. car";
    case VehicleConstants::bus:
        return "urban bus";
    case VehicleConstants::motorbike:
        return "motorcycle";
    case VehicleConstants::truck:
        return "HGV";
    default:
        return "pass. car";
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

VehicleConstants::vehicle_types GenerateContaminantSkill::getVehicleType(){
    return this->vehicle_type;
}

QString GenerateContaminantSkill::getVehicleTechnology(){
    return this->vehicle_technology;
}

QString GenerateContaminantSkill::getVehicleSubsegment(){
    return this->vehicle_subsegment;
}

double GenerateContaminantSkill::getTotalContaminantAmount(){
    return this->contaminantAmount;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void GenerateContaminantSkill::setEmissionFactorMap(QMap<QString, double> map){
    this->emissionFactorMap = map;
}

void GenerateContaminantSkill::setVehicleType(VehicleConstants::vehicle_types type){
    this->vehicle_type = type;
}

void GenerateContaminantSkill::setVehicleTechnology(QString technology){
    this->vehicle_technology = technology;
}

void GenerateContaminantSkill::setVehicleSubsegment(QString subsegment){
    this->vehicle_subsegment = subsegment;
}

void GenerateContaminantSkill::setContaminantAmount(double amount){
    this->contaminantAmount = amount;
}
