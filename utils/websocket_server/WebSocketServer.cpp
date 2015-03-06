#include <QtCore/QDebug>

#include "WebSocketServer.h"

WebSocketServer::WebSocketServer(unsigned int port, QObject* parent):
QObject(parent),
server(new QWebSocketServer(QStringLiteral("Echo Server"),
             QWebSocketServer::NonSecureMode, this)),
             connected_clients()
{
    if (this->server->listen(QHostAddress::Any, port)) {
        qDebug() << "Web Socket Server initiated in port " << port << endl;
        QObject::connect(this->server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
        QObject::connect(this->server, SIGNAL(closed()), this, SLOT(socketDisconnected()));
    }
}

WebSocketServer::~WebSocketServer(){
    qDebug() << "Deleting Web Socket Server" << endl;
    this->server->close();
    qDeleteAll(this->connected_clients.begin(), this->connected_clients.end());
}

void WebSocketServer::onNewConnection()
{
    QWebSocket *socket = this->server->nextPendingConnection();
    if(!this->connected_clients.contains(socket)){
        qDebug() << "New socket connected : " << socket->peerAddress().toString() << endl;
        this->connected_clients << socket;
    }
}

void WebSocketServer::socketDisconnected(){
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (client) {
        qDebug() << "Socket disconnected : " <<client->peerAddress().toString() << endl;
        this->connected_clients.removeAll(client);
        client->deleteLater();
    }
}

void WebSocketServer::updateUI(QByteArray message){
    // Send to all connected clients
    QListIterator<QWebSocket*> i(connected_clients);
    while (i.hasNext()){
        try{
        i.next()->sendTextMessage(QString(message));
        } catch(...){}
    }
}
