#include "Person.h"

Person::Person(PersonConstants::gender gender, QDateTime born_date) : Agent(){
    this->gender = gender;
    this->setBornDatetime(born_date);
}

Person::~Person(){
}

/**********************************************************************
 GETTERS
**********************************************************************/

PersonConstants::gender Person::getGender(){
    return this->gender;
}
