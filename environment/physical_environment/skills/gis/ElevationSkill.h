#ifndef ELEVATIONSKILL_H
#define ELEVATIONSKILL_H

#include <QVector>

#include "geos/geom/LineString.h"
#include "geos/geom/Point.h"
#include "environment/skills/db/AccessDBSkill.h"

using namespace geos::geom;

class ElevationSkill : public AccessDBSkill
{
        Q_OBJECT
public:

    ElevationSkill(Agent* agent);
    virtual ~ElevationSkill();

    // THIS skill methods
    double getPointElevation(Point* point);
    double calculateSlopeGradient(double verticalDisplacement, double horizontalDisplacement);

};

#endif // ELEVATIONSKILL_H
