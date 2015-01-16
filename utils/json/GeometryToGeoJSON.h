#ifndef GEOMETRYTOGEOJSON_H
#define GEOMETRYTOGEOJSON_H

#include <QJsonObject>
#include <QtScript/QScriptEngine>
#include <geos/geom/Geometry.h>

using namespace geos::geom;

class GeometryToGeoJSON
{
public:
    static QJsonObject toJSON(Geometry * geom, QJsonObject properties);
    //static Geometry* fromJSON(QByteArray json);
};

#endif // GEOMETRYTOGEOJSON_H
