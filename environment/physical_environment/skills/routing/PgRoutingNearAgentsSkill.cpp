#include "PgRoutingNearAgentsSkill.h"

PgRoutingNearAgentsSkill::PgRoutingNearAgentsSkill(Agent* agent, QVector<PgRoutingConstants::road_types> roads, double reverse_factor) : PgRoutingAtoManySkill(agent, roads, reverse_factor){
}

PgRoutingNearAgentsSkill::~PgRoutingNearAgentsSkill(){
}

/**********************************************************************
SKILL METHODS
**********************************************************************/

QMap<GraphRoute*, Agent*> PgRoutingNearAgentsSkill::orderByDistance(QList<Agent *> agents, const GeometryFactory *factory){
    QMap<GraphRoute*, Agent*> ordered;

    if( !this->getDriver()->connectDB() ){
        return ordered;
    }

    this->createTempRoutingTables();

    QVector<Point*> points;
    foreach(Agent* agent, agents) {
        points.append( agent->getGeometry()->getCentroid() );
    }

    QVector<GraphRoute*> routes = this->getAtoManyRoute(this->getSkilledAgent()->getGeometry()->getCentroid(), points, factory);

    // Order routes
    QVector<GraphRoute*> ordered_routes;
    while( !routes.isEmpty() ){
        GraphRoute* shortest = routes.first();
        foreach(GraphRoute* route , routes){
            if ( route->getTotalLength() < shortest->getTotalLength() ){
                shortest = route;
            }
        }
        ordered_routes.append(shortest);
        routes.removeOne( shortest );
    }

    // Asign to each received route its agent
    foreach(GraphRoute* route, ordered_routes){
        Agent* nearest = 0;
        foreach(Agent* agent , agents){
            if( !nearest ){
                nearest = agent;
            }

            if ( agent->getGeometry()->getCentroid()->distance( route->getEndingPoint() ) <
                 nearest->getGeometry()->getCentroid()->distance( route->getEndingPoint() ) ){
                nearest = agent;
            }
        }
        ordered.insert(route , nearest);
    }
    this->getDriver()->disconnectDB();
    return ordered;
}
