#ifndef VEHICLE_H
#define VEHICLE_H

#include "agents/Agent.h"
#include "agents/vehicles/VehicleConstants.h"
#include "environment/physical_environment/skills/routing/PgRoutingAtoBSkill.h"

class Vehicle : public Agent
{
    Q_OBJECT
public:
    Vehicle(VehicleConstants::vehicle_types vehicle_type);
    ~Vehicle();

    // SETTERS
    void setStartingPoint(Point* point);
    void setEndingPoint(Point* point);
    void setMaxSpeed(double speed);

    // GETTERS
    Point* getStartingPoint();
    Point* getEndingPoint();
    double getMaxSpeed();

    // ENTITY METHODS

protected:

    // ENTITY SKILLS
    PgRoutingAtoBSkill* pgrouting_atob_skill;

    VehicleConstants::vehicle_types vehicle_type;
    double max_speed; // m/s
    Point* starting_point;
    Point* ending_point;
    GraphRoute* route;
};

#endif // VEHICLE_H
