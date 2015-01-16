#include "Skill.h"

Skill::Skill(Agent* skilled_agent){
    this->skilled_agent = skilled_agent;
}

Skill::~Skill(){
}

Agent* Skill::getSkilledAgent(){
    return this->skilled_agent;
}
