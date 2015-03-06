#ifndef PGROUTINGBASESKILL_H
#define PGROUTINGBASESKILL_H

#include <QSqlRecord>
#include <QVector>

#include <geos/io/WKTReader.h>
#include <geos/geom/Point.h>
#include <geos/geom/LineString.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/CoordinateSequence.h>

#include "environment/skills/db/AccessDBSkill.h"
#include "environment/physical_environment/skills/routing/PgRoutingConstants.h"
#include "agents/graphs/roads/Road.h"

using namespace geos::geom;

class PgRoutingBaseSkill : public AccessDBSkill
{
        Q_OBJECT
public:

    // GETTERS
    QVector<PgRoutingConstants::road_types> getUsableRoads();

    // SETTERS
    void setUsableRoads(QVector<PgRoutingConstants::road_types> roads);

    // THIS SKILL METHODS
    int getGraphNodeIdForPoint(Point* point);
    Point* getGraphNodeGeomForPoint(Point* point);
    Point* getNearestRoadPointToGeometry(Geometry* geometry);
    void createTempRoutingTables();
    Road* createOrReuseRoadForGeom(LineString* line, PgRoutingConstants::road_types type);

protected:
    PgRoutingBaseSkill( Agent* agent , QVector<PgRoutingConstants::road_types> roads_to_use, double reverse_factor = 1 );
    virtual ~PgRoutingBaseSkill();

     const QString pgr_table_name = "pgr_skill_roads"; // Table with all points and lines
     const QString pgr_index_table_name = "pgr_skill_roads_index"; // Table with only points indexed for fast finding
     double reverse_factor; // To multiply reverse_cost
    QVector<PgRoutingConstants::road_types> roads_to_use;
};

#endif // PGROUTINGBASESKILL_H
