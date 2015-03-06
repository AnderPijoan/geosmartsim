#include "AgentContainSkill.h"
#include "environment/Environment.h"
#include "QMutexLocker"

AgentContainSkill::AgentContainSkill(Agent* agent) : Skill(agent){
    this->max_capacity = 1;
}

AgentContainSkill::~AgentContainSkill(){
}

bool AgentContainSkill::enter(Agent* agent){
    if( !Environment::getInstance()->getAgent(agent->getClass() , agent->getId()) ){
        qWarning() << "Can't add agent which is not in the environment";
        return false;
    }

    QMutexLocker locker(&mutex);
    // See if it is already inside
    if (!this->agents_inside.isEmpty() && this->agents_inside.contains(agent)){
        return true;
    }

    if ( !this->isFull() ) {
        this->agents_inside.append(agent);
        return true;
    }
    return false;
}

bool AgentContainSkill::exit(Agent* agent){
    QMutexLocker locker(&mutex);
    return this->agents_inside.removeAll( agent );
}

void AgentContainSkill::clear(){
    QMutexLocker locker(&mutex);
    this->agents_inside.clear();
}

/**********************************************************************
 GETTERS
**********************************************************************/

bool AgentContainSkill::isFull(){
    return this->agents_inside.size() >= this->max_capacity;
}

unsigned int AgentContainSkill::getMaxCapacity(){
    return this->max_capacity;
}

unsigned int AgentContainSkill::getAmountInside(){
    return this->agents_inside.size();
}

QList<Agent*> AgentContainSkill::getAgentsInside(){
    return this->agents_inside;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void AgentContainSkill::setMaxCapacity(unsigned int capacity){
    QMutexLocker locker(&mutex);
    this->max_capacity = capacity;
}
