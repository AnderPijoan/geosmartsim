#ifndef PERSON_H
#define PERSON_H

#include "agents/Agent.h"
#include "agents/people/PersonConstants.h"

class Person : public Agent
{
    Q_OBJECT
public:
    Person(PersonConstants::gender gender, QDateTime born_date = QDateTime::currentDateTime());
    ~Person();

    // GETTERS
    PersonConstants::gender getGender();

private:
    PersonConstants::gender gender;
};

#endif // PERSON_H
