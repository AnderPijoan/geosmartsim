#ifndef ACCESSDBSKILL_H
#define ACCESSDBSKILL_H

#include "environment/skills/Skill.h"
#include "utils/drivers/PostgresDriver.h"

/**
 * @brief The AccessDBSkill class
 * This skill adds a driver to agents in order to make queries
 * to database
 */
class AccessDBSkill : public Skill
{
        Q_OBJECT
public:
    AccessDBSkill(Agent* agent);
    virtual ~AccessDBSkill();

    // Getters
    PostgresDriver* getDriver();

private:
    PostgresDriver* driver;

};

#endif // ACCESSDBSKILL_H
