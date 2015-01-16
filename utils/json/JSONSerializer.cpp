#include "JSONSerializer.h"
#include <QJsonDocument>
#include <QVariantMap>

QByteArray JSONSerializer::createJSONMessage(QJsonArray json_elements, JSONSerializer::operations operation){
    QVariantMap json_elements_and_type;
     json_elements_and_type.insert("operation", operation);
    json_elements_and_type.insert("data", json_elements.toVariantList());
    return  QJsonDocument::fromVariant( json_elements_and_type) .toJson();
}

QByteArray JSONSerializer::createJSONMessage(QJsonObject json_element, JSONSerializer::operations operation){
    QVariantMap json_elements_and_type;
     json_elements_and_type.insert("operation", operation);
    json_elements_and_type.insert("data", json_element.toVariantMap());
    return  QJsonDocument::fromVariant( json_elements_and_type) .toJson();
}
