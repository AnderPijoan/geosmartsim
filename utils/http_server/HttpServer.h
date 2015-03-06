#ifndef HTTPSERVER_H
#define HTTPSERVER_H

// QHTTPSERVER
// https://github.com/nikhilm/qhttpserver
#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>

#include "utils/http_server/UrlCreator.h"

class HttpServer : public QObject
{
    Q_OBJECT
public:

    static HttpServer *getInstance(unsigned int port);
    void createUrls(QString class_name, QStringList operations);

private:
    HttpServer(unsigned int port);
    HttpServer(HttpServer const&);
    ~HttpServer();

    QHttpServer* server;
    unsigned int port;
    UrlCreator* url_creator;
    HttpHandler* handler;
};

#endif // HTTPSERVER_H
