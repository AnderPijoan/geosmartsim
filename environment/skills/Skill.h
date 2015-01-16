#ifndef SKILL_H
#define SKILL_H

#include <QMap>

#include "utils/drivers/PostgresDriver.h"
#include "agents/Agent.h"

using namespace geos::geom;

/**
 * @brief The Skill class
 * Empty skill which is parent of all skills
 */
class Skill: public QObject
{
    Q_OBJECT
public:
    Skill(Agent* skilled_agent);
    virtual ~Skill();
    Agent* getSkilledAgent();

private:
    Agent* skilled_agent; // The agent that inheritated this skill

};

#endif // SKILL_H
