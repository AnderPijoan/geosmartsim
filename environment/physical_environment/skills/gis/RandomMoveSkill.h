#ifndef RANDOMMOVESKILL_H
#define RANDOMMOVESKILL_H

#include <QDebug>
#include <geos/geom/Coordinate.h>

#include "skills/ParentSkill.h"

/**
 * @brief The RandomMoveSkill class
 * Skill that translates the geometry to a random position
 */
class RandomMoveSkill : public ParentSkill
{
public:
    RandomMoveSkill(Agent* skilled_agent);
    virtual ~RandomMoveSkill();
    virtual void parseBodyVariables(QMap<QString,QVariant> body) {Q_UNUSED(body)} // Nothing to do

protected:
    void translateRandom(Geometry* geom);
};

#endif // RANDOMMOVESKILL_H
