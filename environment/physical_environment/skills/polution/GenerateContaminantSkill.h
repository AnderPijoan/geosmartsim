#ifndef GENERATECONTAMINANTSKILL_H
#define GENERATECONTAMINANTSKILL_H

#include "environment/skills/db/AccessDBSkill.h"
#include "environment/physical_environment/skills/routing/PgRoutingConstants.h"
#include "agents/vehicles/VehicleConstants.h"

class GenerateContaminantSkill  : public AccessDBSkill
{
public:

    enum efa_road_types         {
                            motorway,
                            motorway_city,
                            semi_motorway,
                            trunk,
                            trunk_primary,
                            distributor_secondary,
                            distributor_sin,
                            local,
                            local_sin,
                            access
                            };

    enum traffic_condition_types {
                            freeflow,
                            heavy,
                            saturated,
                            stop_and_go
    };

    QMap<QString, double> loadEmissionFactorsFromDB(QString dbname);

    // Getters
    VehicleConstants::vehicle_types getVehicleType();
    double getTotalContaminantAmount();
    GenerateContaminantSkill::traffic_condition_types getTrafficConditions();
    QString getVehicleTechnology();
    QString getVehicleSubsegment();

    // Setters
    void setVehicleType(VehicleConstants::vehicle_types vehicle_type);
    void setVehicleTechnology(QString technology);
    void setVehicleSubsegment(QString subsegment);
    void setContaminantAmount(double amount);
    void setEmissionFactorMap(QMap<QString, double> map);

    // SKILL METHODS
    QString buildRoadKey(efa_road_types type, int speed, int gradient, GenerateContaminantSkill::traffic_condition_types traffic);
    double sumContaminantToTotal(QString roadCharacteristics, double distance);
    static GenerateContaminantSkill::efa_road_types pgrTypeToRoadType(PgRoutingConstants::road_types type);
    QString vehicleTypeToString(VehicleConstants::vehicle_types type);

protected:

    GenerateContaminantSkill(Agent* agent, VehicleConstants::vehicle_types type, QString technology, QString subsegment);
    ~GenerateContaminantSkill();

    VehicleConstants::vehicle_types vehicle_type;
    QString vehicle_technology;
    QString vehicle_subsegment;

    QString contaminantName;
    double contaminantAmount; // g/m

    // QMAP < "TYPE/SPEED/TRAFFIC/GRADIENT" , EFA >
    QMap<QString, double> emissionFactorMap;

private:
    unsigned int buildSpeedKey(int speed, int values[]);

};

#endif // GENERATECONTAMINANTSKILL_H
