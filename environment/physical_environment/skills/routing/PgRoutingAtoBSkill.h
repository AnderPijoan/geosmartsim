#ifndef PGROUTINGATOBSKILL_H
#define PGROUTINGATOBSKILL_H

#include "environment/physical_environment/skills/routing/PgRoutingBaseSkill.h"

class PgRoutingAtoBSkill : public PgRoutingBaseSkill
{
            Q_OBJECT
public:
    PgRoutingAtoBSkill( Agent* agent , QVector<PgRoutingConstants::road_types> roads_to_use , double reverse_factor = 1);
    virtual ~PgRoutingAtoBSkill();

    //GraphRoute* getAtoBRoute(Point* starting_point , Point* ending_point, const GeometryFactory* factory);
};

#endif // PGROUTINGATOBSKILL_H
