#include "NearAgentsSkill.h"

NearAgentsSkill::NearAgentsSkill(Agent* agent)  : AccessDBSkill(agent){

}

NearAgentsSkill::~NearAgentsSkill(){

}

/**********************************************************************
SKILL METHODS
**********************************************************************/

QMap<double, Agent*> NearAgentsSkill::orderByDistance(QList<Agent *> agents){
    QMap<double, Agent*> ordered;
    QMap<int, Agent*> id_to_agent;
    this->getDriver()->connectDB();
    this->getDriver()->executeCustomQuery("CREATE TEMP TABLE nearest ( id integer , the_geom geography(Geometry,4326) )");

    foreach(Agent* agent , agents){
        id_to_agent.insert( agent->getId() , agent);
        this->getDriver()->executeCustomQuery(QString("INSERT INTO nearest (id , the_geom) VALUES (%1 , ST_GEOGFROMTEXT( '%2' ) )")
                                         .arg(agent->getId()).arg( QString::fromStdString(agent->getGeometry()->toString()) ) );
    }

    QString sql = QString("SELECT nearest.id AS id , ST_Distance( nearest.the_geom , geometry.the_geom ) AS distance \
                          FROM nearest , \
                          (SELECT ST_GEOGFROMTEXT( '%1' ) AS the_geom ) AS geometry \
                          ORDER BY distance")
                          .arg( QString::fromStdString(this->getSkilledAgent()->getGeometry()->toString() ) );

            QListIterator<QSqlRecord> i(this->getDriver()->executeCustomQuery(sql));
    this->getDriver()->disconnectDB();
    while( i.hasNext() ){
        QSqlRecord record = i.next();
        ordered.insert( record.value( "distance").toDouble() , id_to_agent.value( record.value( "id" ).toInt() ) );
    }

    return ordered;
}
