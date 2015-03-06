#include <QDateTime>

#include "RandomMoveSkill.h"

RandomMoveSkill::RandomMoveSkill(Agent* agent) : ParentSkill(agent){
}

RandomMoveSkill::~RandomMoveSkill(){
}

void RandomMoveSkill::translateRandom(Geometry* geometry){
    for(unsigned int i = 0; i < geometry->getCoordinates()->size(); i++){
        geometry->getCoordinates()->setAt(
                *Environment::getInstance()->getRandomPoint(qrand())->getCoordinate(), i);
    }
}
