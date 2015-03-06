#ifndef VEHICLE_H
#define VEHICLE_H

#include "agents/Agent.h"
#include "agents/vehicles/VehicleConstants.h"

class Vehicle : public Agent
{
    Q_OBJECT
public:
    Vehicle(VehicleConstants::vehicle_types vehicle_type ,  QString class_name = "Vehicle");
    ~Vehicle();

    // SETTERS
    void setStartingPoint(Point* point);
    void setEndingPoint(Point* point);
    void setMaxSpeed(double speed);

    // GETTERS
    Point* getStartingPoint();
    Point* getEndingPoint();
    double getMaxSpeed();

protected:

    VehicleConstants::vehicle_types vehicle_type;
    double max_speed; // m/s
    Point* starting_point;
    Point* ending_point;
};

#endif // VEHICLE_H
