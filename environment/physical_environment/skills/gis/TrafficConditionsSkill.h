#ifndef TRAFFICCONDITIONSSKILL_H
#define TRAFFICCONDITIONSSKILL_H

#include "environment/skills/api/AccessAPISkill.h"

class TrafficConditionsSkill : public AccessAPISkill
{
        Q_OBJECT
public:
    TrafficConditionsSkill(Agent* agent);
    virtual ~TrafficConditionsSkill();

    // THIS skill methods
    int getTrafficForRoadSegment(int road_id);
};

#endif // TRAFFICCONDITIONSSKILL_H
