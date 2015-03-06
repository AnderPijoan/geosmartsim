#include "Vehicle.h"

Vehicle::Vehicle(VehicleConstants::vehicle_types type, QString class_name) : Agent(class_name){
    this->vehicle_type = type;
    this->max_speed = 2.7; // ~10km/h
    this->starting_point = 0;
    this->ending_point = 0;
}

Vehicle::~Vehicle(){
    if(this->starting_point){
        delete this->starting_point;
    }
    if (this->ending_point){
        delete this->ending_point;
    }
}

/**********************************************************************
GETTERS
**********************************************************************/

Point* Vehicle::getStartingPoint(){
    return this->starting_point;
}

Point* Vehicle::getEndingPoint(){
    return this->ending_point;
}

double Vehicle::getMaxSpeed(){
    return this->max_speed;
}

/**********************************************************************
SETTERS
**********************************************************************/

void Vehicle::setStartingPoint(Point *point){
    this->starting_point = point;
}

void Vehicle::setEndingPoint(Point *point){
    this->ending_point = point;
}

void Vehicle::setMaxSpeed(double speed){
    this->max_speed = speed;
}
