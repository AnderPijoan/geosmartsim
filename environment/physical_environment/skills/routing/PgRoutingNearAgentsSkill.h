#ifndef PGROUTINGNEARAGENTSSKILL_H
#define PGROUTINGNEARAGENTSSKILL_H

#include "environment/physical_environment/skills/routing/PgRoutingAtoManySkill.h"

class PgRoutingNearAgentsSkill : public PgRoutingAtoManySkill
{
            Q_OBJECT
public:
    PgRoutingNearAgentsSkill(Agent* agent, QVector<PgRoutingConstants::road_types> roads, double reverse_factor = 1);
    ~PgRoutingNearAgentsSkill();

    QMap<GraphRoute*, Agent*> orderByDistance(QList<Agent*> agents, const GeometryFactory *factory);
};

#endif // PGROUTINGNEARAGENTSSKILL_H
