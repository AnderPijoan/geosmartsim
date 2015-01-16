#include <QDebug>
#include <string.h>
#include <QJsonArray>

#include <geos/geom/Point.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>

#include "GeometryToGeoJSON.h"

QJsonObject GeometryToGeoJSON::toJSON(Geometry *geom, QJsonObject properties){
    QJsonObject geojson;
    if(geom && geom->isValid()){

        // Geometry
        QJsonObject geometry;
        if(geom->getGeometryType().compare(std::string("Point")) == 0){
            Point* point = dynamic_cast<Point*>(geom);
            geometry.insert("type", "Point");

            QJsonArray coordinates;
            coordinates.append(point->getX());
            coordinates.append(point->getY());
            geometry.insert("coordinates", coordinates);

        } else if(geom->getGeometryType().compare("LineString") == 0){
            LineString* line = dynamic_cast<LineString*>(geom);
            geometry.insert("type", "LineString");

            QJsonArray lineCoordinates;

            for(unsigned int i = 0; i < line->getCoordinates()->size(); i++){
                QJsonArray coordinatePair;
                coordinatePair.append(line->getCoordinates()->getAt(i).x);
                coordinatePair.append(line->getCoordinates()->getAt(i).y);
                lineCoordinates.append(coordinatePair);
            }
            geometry.insert("coordinates", lineCoordinates);

        } else if(geom->getGeometryType().compare("Polygon") == 0){
            Polygon* polygon = dynamic_cast<Polygon*>(geom);
            geometry.insert("type", "Polygon");

            QJsonArray rings;
            QJsonArray ringCoordinates;

            for(unsigned int i = 0; i < polygon->getCoordinates()->size(); i++){
                QJsonArray coordinatePair;
                coordinatePair.append(polygon->getCoordinates()->getAt(i).x);
                coordinatePair.append(polygon->getCoordinates()->getAt(i).y);
                ringCoordinates.append(coordinatePair);
            }
            rings.append(ringCoordinates);
            geometry.insert("coordinates", rings);
        }
        geojson.insert("type", "Feature");
        geojson.insert("geometry", geometry);

        // Properties
        if(properties.length()){
            geojson.insert("properties", properties);
        } else {
            QJsonObject empty;
            geojson.insert("properties", empty);
        }
    }
    return geojson;
}
