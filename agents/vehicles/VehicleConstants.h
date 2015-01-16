#ifndef VEHICLECONSTANTS_H
#define VEHICLECONSTANTS_H

#include <QObject>
#include <QVector>
#include "environment/physical_environment/skills/routing/PgRoutingConstants.h"

class VehicleConstants : public QObject
{
    Q_OBJECT
     Q_ENUMS(vehicle_types)
public:

    enum vehicle_types {foot, bike, motorbike, car, truck, bus, tram, metro, train};

    static QVector<PgRoutingConstants::road_types> roadTypesForVehicle(VehicleConstants::vehicle_types vehicle){
        QVector<PgRoutingConstants::road_types> roads;
        switch(vehicle){
        case car:
        case motorbike:
            roads.append(PgRoutingConstants::h_living_street);
            roads.append(PgRoutingConstants::h_path);
        case bus:
        case truck:
            roads.append(PgRoutingConstants::h_motorway);
            roads.append(PgRoutingConstants::h_motorway_link);
            roads.append(PgRoutingConstants::h_motorway_junction);
            roads.append(PgRoutingConstants::h_trunk);
            roads.append(PgRoutingConstants::h_trunk_link);
            roads.append(PgRoutingConstants::h_primary);
            roads.append(PgRoutingConstants::h_primary_link);
            roads.append(PgRoutingConstants::h_secondary);
            roads.append(PgRoutingConstants::h_secondary_link);
            roads.append(PgRoutingConstants::h_tertiary);
            roads.append(PgRoutingConstants::h_tertiary_link);
            roads.append(PgRoutingConstants::h_residential);
            roads.append(PgRoutingConstants::h_service);
            roads.append(PgRoutingConstants::h_unclassified);
            roads.append(PgRoutingConstants::h_road);
            roads.append(PgRoutingConstants::j_roundabout);
            return roads;
        case train:
            roads.append(PgRoutingConstants::r_narrow_gauge);
            roads.append(PgRoutingConstants::r_rail);
            return roads;
        case metro:
            roads.append(PgRoutingConstants::r_subway);
            return roads;
        case tram:
            roads.append(PgRoutingConstants::r_tram);
            return roads;
        case bike:
            roads.append(PgRoutingConstants::h_secondary);
            roads.append(PgRoutingConstants::h_secondary_link);
            roads.append(PgRoutingConstants::h_tertiary);
            roads.append(PgRoutingConstants::h_tertiary_link);
            roads.append(PgRoutingConstants::h_residential);
            roads.append(PgRoutingConstants::h_living_street);
            roads.append(PgRoutingConstants::h_service);
            roads.append(PgRoutingConstants::h_unclassified);
            roads.append(PgRoutingConstants::h_road);
            roads.append(PgRoutingConstants::j_roundabout);
            roads.append(PgRoutingConstants::c_lane);
            roads.append(PgRoutingConstants::c_opposite);
            roads.append(PgRoutingConstants::c_opposite_lane);
            roads.append(PgRoutingConstants::c_track);
            roads.append(PgRoutingConstants::h_cycleway);
            roads.append(PgRoutingConstants::h_path);
            return roads;
        case foot:
            roads.append(PgRoutingConstants::h_primary);
            roads.append(PgRoutingConstants::h_primary_link);
            roads.append(PgRoutingConstants::h_secondary);
            roads.append(PgRoutingConstants::h_secondary_link);
            roads.append(PgRoutingConstants::h_tertiary);
            roads.append(PgRoutingConstants::h_tertiary_link);
            roads.append(PgRoutingConstants::h_residential);
            roads.append(PgRoutingConstants::h_service);
            roads.append(PgRoutingConstants::h_unclassified);
            roads.append(PgRoutingConstants::h_road);
            roads.append(PgRoutingConstants::j_roundabout);
            roads.append(PgRoutingConstants::h_bridleway);
            roads.append(PgRoutingConstants::h_footway);
            roads.append(PgRoutingConstants::h_living_street);
            roads.append(PgRoutingConstants::h_path);
            roads.append(PgRoutingConstants::h_pedestrian);
            roads.append(PgRoutingConstants::h_steps);
            roads.append(PgRoutingConstants::h_track);
            return roads;
        }
        return roads;
    }
};

#endif // VEHICLECONSTANTS_H
