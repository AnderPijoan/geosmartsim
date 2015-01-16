#include "Person.h"

Person::Person(QDateTime born_date, PersonConstants::gender gender) : Agent(){
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
