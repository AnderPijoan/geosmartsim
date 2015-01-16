#ifndef COMMUNICATIONENVIRONMENT_H
#define COMMUNICATIONENVIRONMENT_H

#include <QObject>

#include "environment/communication_environment/Message.h"

class CommunicationEnvironment : public QObject
{
    Q_OBJECT
public:
    explicit CommunicationEnvironment(QObject *parent = 0);

signals:
    void sendNotificationToAgent(Message* message);

public slots:
    void receiveMessageFromAgent(Message *message);

};

#endif // COMMUNICATIONENVIRONMENT_H
