#ifndef PGROUTINGDRIVINGDISTANCESKILL_H
#define PGROUTINGDRIVINGDISTANCESKILL_H

#include "environment/physical_environment/skills/routing/PgRoutingBaseSkill.h"

class PgRoutingDrivingDistanceSkill : public PgRoutingBaseSkill
{
    Q_OBJECT
public:
    PgRoutingDrivingDistanceSkill(Agent* agent , QVector<PgRoutingConstants::road_types> roads , double reverse_factor = 1);
    ~PgRoutingDrivingDistanceSkill();

     QVector<GraphRoute*> getDrivingDistanceInMeters(Point* starting_point , int meters, const GeometryFactory* factory);
};

#endif // PGROUTINGDRIVINGDISTANCESKILL_H
