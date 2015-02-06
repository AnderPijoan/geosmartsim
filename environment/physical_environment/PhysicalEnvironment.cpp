#include <QDebug>

#include <geos/geom/PrecisionModel.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/Envelope.h>
#include <geos/geom/Point.h>
#include <geos/io/WKTReader.h>

#include "PhysicalEnvironment.h"

PhysicalEnvironment::PhysicalEnvironment(QObject *parent) : QObject(parent)
{
    // Create geometry factory
    this->geometry_factory = new GeometryFactory(new PrecisionModel(99999999999999999.0, 0, 0), 4326);

}

PhysicalEnvironment::~PhysicalEnvironment(){
    delete this->geometry_factory;
    delete this->bounds;
}

/**********************************************************************
 GETTERS
**********************************************************************/

GeometryFactory* PhysicalEnvironment::getGeometryFactory(){
    return this->geometry_factory;
}

Geometry* PhysicalEnvironment::getGeometryFromWKT(QString wkt){
    QMutexLocker locker(&mutex);
    geos::io::WKTReader reader = geos::io::WKTReader(this->geometry_factory);
    return reader.read(wkt.toStdString());
}

QString PhysicalEnvironment::getWKTFromGeometry(Geometry* geometry){
    if ( geometry ){
        return QString::fromStdString( geometry->toString() );
    }
        return QString("POLYGON EMPTY");
}

Geometry* PhysicalEnvironment::getBounds(){
    return this->bounds;
}

Point* PhysicalEnvironment::getRandomPoint(int seed){
     QMutexLocker locker(&mutex);
    srand( seed );
    Point* point = 0;
    geos::io::WKTReader* reader = new geos::io::WKTReader();
    double left = this->bounds->getEnvelopeInternal()->getMinX();
    double right = this->bounds->getEnvelopeInternal()->getMaxX();
    double top = this->bounds->getEnvelopeInternal()->getMaxY();
    double bottom = this->bounds->getEnvelopeInternal()->getMinY();
    do{
        if (point) { delete point; }
        double x = left + ((static_cast<double>(rand() % 1000)) / 1000 * ( right - left ));
        double y = bottom + ((static_cast<double>(rand() % 1000)) / 1000 * ( top - bottom ));
        point = dynamic_cast<Point*> (reader->read( QString("POINT(%1 %2)")
                     .arg( x )
                     .arg( y )
                     .toStdString()
                    ) );
    }  while( !this->bounds->contains( point ) );
    delete reader;
    return point;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void PhysicalEnvironment::setBounds(QString wkt){
    this->bounds = this->getGeometryFromWKT(wkt);
}

void PhysicalEnvironment::setBounds(Geometry *geometry){
    this->bounds = geometry;
}
