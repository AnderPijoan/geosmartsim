#include <QDebug>

#include "CommunicationEnvironment.h"

CommunicationEnvironment::CommunicationEnvironment(QObject *parent) : QObject(parent){
}

/**********************************************************************
 SLOTS
**********************************************************************/

void CommunicationEnvironment::receiveMessageFromAgent(Message *message)
{
    qDebug() << "Environment received a message:" << message->toString() << endl;
}
