#include "FuzzyLogicSkill.h"

FuzzyLogicSkill::FuzzyLogicSkill(Agent* agent) : Skill(agent){
this->fuzzy_logic_engine = new Engine("");
}

FuzzyLogicSkill::~FuzzyLogicSkill(){
    delete this->fuzzy_logic_engine;
}

/**********************************************************************
  GETTERS
**********************************************************************/
Engine* FuzzyLogicSkill::getEngine(){
    return this->fuzzy_logic_engine;
}

bool FuzzyLogicSkill::isReady(){
    return this->fuzzy_logic_engine->isReady();
}

/**********************************************************************
   SETTERS
**********************************************************************/
void FuzzyLogicSkill::addInputVariable(QString name, InputVariable *variable){
    variable->setName(name.toStdString());
    this->fuzzy_logic_engine->addInputVariable(variable);
}

void FuzzyLogicSkill::addOutputVariable(QString name, OutputVariable *variable){
    variable->setName(name.toStdString());
    this->fuzzy_logic_engine->addOutputVariable(variable);
}

void FuzzyLogicSkill::addRuleBlock(RuleBlock* block){
    this->fuzzy_logic_engine->addRuleBlock(block);
}

/**********************************************************************
   SKILL METHODS
**********************************************************************/
void FuzzyLogicSkill::process(){
    this->fuzzy_logic_engine->process();
}
