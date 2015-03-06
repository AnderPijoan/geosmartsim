#ifndef URLCREATOR_H
#define URLCREATOR_H

#include <QStringList>

#include "utils/http_server/HttpHandler.h"

class UrlCreator : public QObject
{    Q_OBJECT
public:
    UrlCreator(HttpHandler* handler);
    void createUrlsForAgents(HttpHandler* handler, QString class_name, QStringList operations);
};

#endif // URLCREATOR_H
