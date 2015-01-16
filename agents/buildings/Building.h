#ifndef BUILDING_H
#define BUILDING_H

#include "agents/Agent.h"
#include "environment/skills/contain/AgentContainSkill.h"
#include "environment/physical_environment/skills/routing/PgRoutingAtoManySkill.h"
#include "agents/people/Person.h"
#include "geos/geom/Point.h"

class Building : public Agent
{
    Q_OBJECT
public:
    Building(int levels);
    ~Building();
     virtual QJsonObject customGeoJSONProperties(); // Overwrite to add extra variables to geojson properties

    // GETTERS
    int getCapacity();
    int getLevels();
    double getArea();
    QList<Agent*> getInhabitants();
    QVector<Point*> getEntrances();

    // SETTERS
    void setArea(double area);
    void setCapacity(int inhabitants);
    void addInhabitant(Person* person);
    void addEntrance(Point* entrance);

    // ENTITY METHODS
    void loadEntrances(QVector<PgRoutingConstants::road_types> roads_to_use, QString entrance_where = "entrances.tags ? 'entrance'");

protected:
    // ENTITY SKILLS
   AgentContainSkill* inhabitants_contain_skill;
   PgRoutingAtoManySkill* routing_skill;

private:

   QVector<Point*> entrances;
   double area; // m2
   int levels;
};

#endif // BUILDING_H
