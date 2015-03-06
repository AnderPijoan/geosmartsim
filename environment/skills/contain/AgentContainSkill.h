#ifndef AGENTCONTAINSKILL_H
#define AGENTCONTAINSKILL_H

#include "QMutex"

#include "environment/skills/Skill.h"

/**
 * @brief The AgentContainSkill class
 * This skill makes entities available to contain other agents.
 * It will be used for roads, pipelines, buildings, etc.
 */
class AgentContainSkill : public Skill
{
        Q_OBJECT
public:
    AgentContainSkill(Agent* agent);
    ~AgentContainSkill();

    // Getters
    bool isFull();
    unsigned int getMaxCapacity();
    unsigned int getAmountInside();
    QList<Agent*> getAgentsInside();

    // Setters
    void setMaxCapacity(unsigned int capacity);

    // SKILL METHODS
    bool enter(Agent* agent);
    bool exit(Agent* agent);
    void clear();

private:
    unsigned int max_capacity;
    QList<Agent*> agents_inside;
    // Mutex
    QMutex mutex;
};

#endif // AGENTCONTAINSKILL_H
