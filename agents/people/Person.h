#ifndef PERSON_H
#define PERSON_H

#include "agents/Agent.h"
#include "agents/people/PersonConstants.h"

class Person : public Agent
{
    Q_OBJECT
public:
    Person(QDateTime born_date, PersonConstants::gender gender);
    ~Person();

    // GETTERS
    PersonConstants::gender getGender();

private:
    PersonConstants::gender gender;
};

#endif // PERSON_H
