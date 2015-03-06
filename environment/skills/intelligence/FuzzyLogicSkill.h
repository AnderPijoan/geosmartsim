#ifndef FUZZYLOGICSKILL_H
#define FUZZYLOGICSKILL_H

#include "environment/skills/Skill.h"

// Fuzzylite library
// https://github.com/fuzzylite/fuzzylite
#include "fl/Engine.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"
#include "fl/rule/RuleBlock.h"

using namespace fl;

class FuzzyLogicSkill : public Skill
{
    Q_OBJECT
public:

    FuzzyLogicSkill(Agent* agent);
    ~FuzzyLogicSkill();

    // GETTERS
    Engine* getEngine();
    bool isReady();

    // SETTERS
    void addInputVariable(QString name, InputVariable* variable);
    void addOutputVariable(QString name , OutputVariable* variable);
    void addRuleBlock(RuleBlock* block);

    // METHIDS
    void process();

private:
 Engine* fuzzy_logic_engine;


};

#endif // FUZZYLOGICSKILL_H
