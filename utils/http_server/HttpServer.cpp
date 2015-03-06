#include <qhttpserver.h>
#include <QThread>

#include "HttpServer.h"

HttpServer* HttpServer::getInstance(unsigned int port){
    static HttpServer instance(port);
    return &instance;
}

HttpServer::HttpServer(unsigned int port)
{
    this->handler = new HttpHandler();

    this->url_creator = new UrlCreator(this->handler);
    QThread url_creator_thread;
    this->url_creator->moveToThread(&url_creator_thread);

    this->port = port;
    this->server = new QHttpServer(this);

    if (server->listen(QHostAddress::Any, this->port)){
        // Connect to qHttpServer signal
        connect(this->server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
        this->handler, SLOT(handleHttpRequest(QHttpRequest*, QHttpResponse*)));

        qDebug() << "HttpServer initiated in port " << port << endl;
    } else {
        qDebug() << "HttpServer could not be initiated in port " << port << endl;
    }
}

HttpServer::~HttpServer(){
    delete this->handler;
    delete this->server;
    delete this->url_creator;
}

void HttpServer::createUrls(QString class_name, QStringList operations){
    this->url_creator->createUrlsForAgents(this->handler, class_name, operations);
}
