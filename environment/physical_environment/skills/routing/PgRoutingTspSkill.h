#ifndef PGROUTINGTSPSKILL_H
#define PGROUTINGTSPSKILL_H

#include <QMap>

#include "environment/physical_environment/skills/routing/PgRoutingAtoBSkill.h"

class PgRoutingTspSkill : public PgRoutingAtoBSkill
{
            Q_OBJECT
public:
    PgRoutingTspSkill( Agent* entity , QVector<PgRoutingConstants::road_types> roads_to_use , double reverse_factor = 1);
    virtual ~PgRoutingTspSkill();

    // METHODS
    QVector<GraphRoute*> getTSPRoute(Point* starting_point , Point* ending_point , QVector<Point*> through_points, const GeometryFactory* factory);
    QVector<Point*> orderThroughPointsGenetic(Point* starting_point , Point* ending_point, QVector<Point*> through_points);
    QVector<Point*> orderThroughPointsNearest(Point* starting_point , Point* ending_point, QVector<Point*> through_points);
};

#endif // PGROUTINGTSPSKILL_H
