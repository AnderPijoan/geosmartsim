#include <QDebug>

#include "AccessDBSkill.h"

AccessDBSkill::AccessDBSkill(Agent* agent) : Skill(agent)
{
    // Each Agent has its own connection (hash generated here)
    if (agent){
        this->driver = new PostgresDriver( this->getSkilledAgent()->getClass() +  QString::number( this->getSkilledAgent()->getId() ) );
    } else {
        this->driver = new PostgresDriver( "shared_connection" );
    }

}

AccessDBSkill::~AccessDBSkill(){
    delete driver;
}

PostgresDriver* AccessDBSkill::getDriver(){
    return this->driver;
}
