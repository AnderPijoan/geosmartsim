#include "Message.h"

Message::Message(){}

void Message::reply(QString content, QString sender)
{
    this->content = content;
    QString aux = this->sender;
    this->sender = sender;
    this->receivers.clear();
    this->receivers.append(aux);
}

QString Message::toString()
{
    return "Sender:" + this->sender + " Message:" + this->content;
}

/**********************************************************************
 GETTERS
**********************************************************************/

QString Message::getContent()
{
   return this->getContent();
}

QString Message::getSender()
{
    return this->sender;
}

QVector<QString> Message::getReceivers()
{
    return this->receivers;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void Message::setContent(QString content)
{
    this->content = content;
}

void Message::setSender(QString sender)
{
    this->sender = sender;
}

void Message::addReceiver(QString receiver)
{
    this->receivers.append(receiver);
}

