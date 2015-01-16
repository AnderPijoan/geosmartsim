#ifndef ACCESSAPISKILL_H
#define ACCESSAPISKILL_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "environment/skills/Skill.h"

/**
 * @brief The AccessAPISkill class
 * This skill adds a driver to agents in order to make queries
 * to an internet API
 */
class AccessAPISkill : public Skill
{
    Q_OBJECT
public:
    AccessAPISkill(Agent* agent);
    ~AccessAPISkill();

    QByteArray getRequest(QUrl url, bool async = false);
    QByteArray postRequest(QUrl url, bool async = false);
    QByteArray putRequest(QUrl url, bool async = false);
    QByteArray deleteRequest(QUrl url, bool async = false);

public slots:
    void parseNetworkResponse( QNetworkReply *finished );

private:
    QByteArray waitForResponse();

    QNetworkAccessManager network_manager;
    QByteArray response_data;
    QNetworkReply::NetworkError response_error;
};

#endif // ACCESSAPISKILL_H
