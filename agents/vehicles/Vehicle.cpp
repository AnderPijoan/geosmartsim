#include "Vehicle.h"

Vehicle::Vehicle(VehicleConstants::vehicle_types type) : Agent(){
    this->vehicle_type = type;
    this->max_speed = 0;
    this->starting_point = 0;
    this->ending_point = 0;
    this->pgrouting_atob_skill = 0;
    this->route = 0;
}

Vehicle::~Vehicle(){
    if(this->starting_point){
        delete this->starting_point;
    }
    if (this->ending_point){
        delete this->ending_point;
    }
    if (this->pgrouting_atob_skill){
        delete this->pgrouting_atob_skill;
    }
    if(this->route){
        delete this->route;
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
