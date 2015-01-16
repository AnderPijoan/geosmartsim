#ifndef PHYSICALENVIRONMENT_H
#define PHYSICALENVIRONMENT_H

#include <QObject>

#include <utils/drivers/PostgresDriver.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Polygon.h>
#include <agents/Agent.h>

using namespace geos::geom;

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

    // SETTERS
    void setBounds(QString wkt);
    void setBounds(Geometry* geometry);

signals:

public slots:

protected:

    explicit PhysicalEnvironment(QObject *parent = 0);
    ~PhysicalEnvironment();

    GeometryFactory* geometry_factory; // Geometry factory
    Geometry* bounds; // Environment bounds

private:
    // Mutex
    QMutex mutex;
};

#endif // PHYSICALENVIRONMENT_H
