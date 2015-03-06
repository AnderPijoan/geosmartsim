#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <environment/Environment.h>

#include <QThread>

#include "AccessAPISkill.h"

AccessAPISkill::AccessAPISkill(Agent* agent) : Skill(agent){    
    this->network_manager = new QNetworkAccessManager(this);
    QObject::connect(network_manager, SIGNAL(finished(QNetworkReply*)),
                         dynamic_cast<AccessAPISkill*>(this), SLOT(parseNetworkResponse(QNetworkReply*)));
}

AccessAPISkill::~AccessAPISkill(){
}


QByteArray AccessAPISkill::getRequest(QUrl url, bool async)
{
    this->response_data = 0;
    this->response_error = QNetworkReply::NoError ;
    QNetworkRequest req(url);
    this->network_manager->get(req);
    if (!async){
        return waitForResponse();
    }
    return 0;
}

QByteArray AccessAPISkill::postRequest(QUrl url, bool async)
{
    this->response_data = 0;
    this->response_error = QNetworkReply::NoError ;
    QNetworkRequest req(url);
    QHttpMultiPart* body = new QHttpMultiPart();
    this->network_manager->post(req, body);
    delete body;
    if (!async){
        return waitForResponse();
    }
    return 0;
}

QByteArray AccessAPISkill::putRequest(QUrl url, bool async)
{
    QNetworkRequest req(url);
    QHttpMultiPart* body = new QHttpMultiPart();
    this->network_manager->put(req, body);
    delete body;
    if (!async){
        return waitForResponse();
    }
    return 0;
}

QByteArray AccessAPISkill::deleteRequest(QUrl url, bool async)
{
    this->response_data = 0;
    this->response_error = QNetworkReply::NoError ;
    QNetworkRequest req(url);
    this->network_manager->deleteResource(req);
    if (!async){
        return waitForResponse();
    }
    return 0;
}

/**********************************************************************
 SLOTS
**********************************************************************/

void AccessAPISkill::parseNetworkResponse( QNetworkReply *finished )
{
    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        return;
    }

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    this->response_data = finished->readAll();
}

/**********************************************************************
 PRIVATE
**********************************************************************/

QByteArray AccessAPISkill::waitForResponse(){
    while(this->response_data == 0 && this->response_error == QNetworkReply::NoError){
        QThread::sleep(1);
    }
    return this->response_data;
}
