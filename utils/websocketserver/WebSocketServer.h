#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QtWebSockets>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtCore/QByteArray>

class WebSocketServer : public QObject
{
    Q_OBJECT
        friend class Environment;
public:
    WebSocketServer(unsigned int port, QObject *parent = Q_NULLPTR);
    ~WebSocketServer();

private slots:
    void broadcastSocketMessage(QByteArray message); // Only environment can connect entities to websockets
    void onNewConnection();
    void socketDisconnected();

private:
    QWebSocketServer *server;
    QList<QWebSocket *> connected_clients;
};

#endif // WEBSOCKETSERVER_H
