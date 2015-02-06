#ifndef FINDNEARESTAMENITYPERSON_H
#define FINDNEARESTAMENITYPERSON_H

#include "agents/people/Person.h"
#include "environment/physical_environment/skills/gis/ElevationSkill.h"
#include "environment/physical_environment/skills/routing/PgRoutingNearAgentsSkill.h"
#include "environment/physical_environment/skills/gis/NearAgentsSkill.h"

/**
 * @brief The FindNearestAmenityPerson class Person that finds its nearest amenity by routing and direct distance
 */
class FindNearestAmenityPerson : public Person
{
    Q_OBJECT
public:
    FindNearestAmenityPerson(QDateTime born_date, PersonConstants::gender gender);
    ~FindNearestAmenityPerson();

    // AGENT METHODS
private:
    virtual void run();

    NearAgentsSkill* near_amenities_skill;
    PgRoutingNearAgentsSkill* near_amenities_routing_skill;
    ElevationSkill* elevation_skill;
    double speed; // m/s
};

#endif // FINDNEARESTAMENITYPERSON_H
