#ifndef PHYSICALENVIRONMENT_H
#define PHYSICALENVIRONMENT_H

#include <QObject>

#include <geos/geom/GeometryFactory.h>
#include <geos/index/quadtree/Quadtree.h>
#include <geos/geom/Polygon.h>
#include <agents/Agent.h>

using namespace geos::geom;
using namespace geos::index::quadtree;

class PhysicalEnvironment : public QObject
{
    Q_OBJECT
public:
    // GETTERS
    Geometry* getBounds();
    Point* getRandomPoint(int seed);
    Geometry* getGeometryFromWKT(QString wkt);
    QString getWKTFromGeometry(Geometry* geometry);
    GeometryFactory* getGeometryFactory();
    QList<Agent*> getAgentsByGeometry(Geometry* geometry, QString class_name);

    // SETTERS
    void setBounds(Geometry* geometry);

protected:
    explicit PhysicalEnvironment(QObject *parent = 0);
    ~PhysicalEnvironment();

    GeometryFactory* geometry_factory; // Geometry factory
    Geometry* bounds; // Environment bounds
    QHash<QString, Quadtree*> spatial_index; // Spatial index

private:
    // Mutex
    QMutex mutex;
};

#endif // PHYSICALENVIRONMENT_H
