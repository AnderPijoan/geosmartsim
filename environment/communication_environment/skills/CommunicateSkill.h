#ifndef COMMUNICATESKILL_H
#define COMMUNICATESKILL_H

#include "environment/skills/Skill.h"

class CommunicateSkill : public Skill
{
        Q_OBJECT
public:
    CommunicateSkill(Agent* agent);
};

#endif // COMMUNICATESKILL_H
