#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>

class JSONSerializer : public QObject
{
    Q_OBJECT
public:
    enum operations{
        CREATE = 0,
        UPDATE = 1,
        DELETE = 2
    };

    static QByteArray createJSONMessage(QJsonObject json_element, JSONSerializer::operations operation);
    static QByteArray createJSONMessage(QJsonArray json_elements, JSONSerializer::operations operation);
};

#endif // JSONSERIALIZER_H
