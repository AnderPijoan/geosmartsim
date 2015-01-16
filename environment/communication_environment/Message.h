#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QString>
#include <QVector>

class Message : public QObject
{
        Q_OBJECT
public:
    Message();

    void setContent (QString content);
    void setSender (QString sender);
    void addReceiver(QString receiver);
    void reply(QString content, QString sender);
    QString toString();

    QString getContent();
    QString getSender();
    QVector<QString> getReceivers();

private:
    QString content;
    QString sender;
    QVector<QString> receivers;
//    QString performative;

};

#endif // MESSAGE_H
