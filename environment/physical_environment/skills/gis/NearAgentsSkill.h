#ifndef NEARAGENTSSKILL_H
#define NEARAGENTSSKILL_H

#include "environment/skills/db/AccessDBSkill.h"

class NearAgentsSkill  : public AccessDBSkill
{
        Q_OBJECT
public:
    NearAgentsSkill(Agent* agent);
    ~NearAgentsSkill();

    // SKILL METHODS
    QMap<double, Agent*> orderByDistance(QList<Agent*> agents);
};

#endif // NEARAGENTSSKILL_H
