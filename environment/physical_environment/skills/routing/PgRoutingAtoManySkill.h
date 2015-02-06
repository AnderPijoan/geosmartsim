#ifndef PGROUTINGATOMANYSKILL_H
#define PGROUTINGATOMANYSKILL_H

#include "environment/physical_environment/skills/routing/PgRoutingBaseSkill.h"

class PgRoutingAtoManySkill : public PgRoutingBaseSkill
{
    Q_OBJECT
public:
    PgRoutingAtoManySkill( Agent* agent , QVector<PgRoutingConstants::road_types> roads_to_use , double reverse_factor = 1);
    ~PgRoutingAtoManySkill();

    QVector<GraphRoute*> getAtoManyRoute(Point* starting_point , QVector<Point*> ending_points, const GeometryFactory* factory);
};

#endif // PGROUTINGATOMANYSKILL_H
