#include <QDebug>
#include <QFile>

#include "agents/Agent.h"
#include "environment/Environment.h"
#include "utils/httpserver/HttpHandler.h"
#include "utils/json/JSONSerializer.h"

QT_FORWARD_DECLARE_CLASS(Entity)

HttpHandler::HttpHandler()
{
    this->busy  = false;
}

/**
 * @brief HttpHandler::handleHttpRequest
 * @param req
 * @param res
 * @param actor
 */
void HttpHandler::handleHttpRequest(QHttpRequest *req, QHttpResponse *res)
{
    qDebug() << "Request";

    // Find if the req matches the environment urls
    bool found = false;
    if (urls.keys().contains(req->method()))
    {
        QVectorIterator<UrlMatch*> i(urls[req->method()]);
        while( i.hasNext() && !found ){
            UrlMatch *urlmatch = i.next();
            QRegularExpressionMatch matchedRegexp = urlmatch->match( req->url() );
            if ( matchedRegexp.hasMatch() ){

                // New request received, but its data wont be ready
                // untill end() signal is called. So store the
                // references in the urlmatch and once the req has
                // ended, the urlmatch will emit signal.
                req->storeBody();

                // If server is parsing another previous request, wait untill it finished
                while( this->busy ){
                    qDebug() << "BUSY";
                    QThread::currentThread()->msleep( 1 );
                }

                // Store references and wait for req to end to parse body
                urlmatch->storeReferencesUntillReqEnd(req, res, matchedRegexp);
                connect( req , SIGNAL( end() ), urlmatch , SLOT( requestEnded() ));
                found = true;
            }
        }
    }
    if (!found){
        qDebug() << "Unexpected Http request :" << req->methodString() << req->url().path() << endl;
    }
    qDebug() << "FINISH";
}

/**
 * @brief HttpHandler::requestFinished Listen to QHTTPSERVER signal to determine when a request has
 * been completely parsed
 */
void HttpHandler::parsingFinished(){
    this->busy = false;
}

/**
 * @brief HttpHandler::containsUrlMath
 * @param method
 * @param urlRegex
 * @return
 */
bool HttpHandler::containsUrlMath(QHttpRequest::HttpMethod method, QRegularExpression urlRegex){
    bool contains = false;
    if(urls.keys().contains(method)){
        QVectorIterator<UrlMatch*> i(urls[method]);
        while (i.hasNext() && !contains){
            contains = urlRegex == i.next()->getUrlRegex();
        }
    }
    return contains;
}

/**
 * @brief HttpHandler::addUrlMatch
 * @param method
 * @param urlRegex
 */
UrlMatch* HttpHandler::addUrlMatch(QHttpRequest::HttpMethod method, QRegularExpression urlRegex){
    if (!this->containsUrlMath(method, urlRegex)){
        UrlMatch *urlmatch = new UrlMatch(method, urlRegex);
        urls[method].append(urlmatch);
        return urlmatch;
    }
    return 0;
}

/**********************************************************************
 SLOTS
**********************************************************************/

void HttpHandler::HTTP_env(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body){
    Q_UNUSED(req);
    Q_UNUSED(match);
    Q_UNUSED(body);
    res->writeHead(200);
    res->setProperty("Content-Type", "application/json");
    //res->end(BaseAgent::manyToJSON(Environment::getInstance()->getEntities(), HttpServer::create_entity));
}

void HttpHandler::HTTP_envTime(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body){
    Q_UNUSED(req);
    Q_UNUSED(match);
    Q_UNUSED(body);
    res->writeHead(200);
    res->setProperty("Content-Type", "application/json");
    res->end(Time::toJSON());
}

void HttpHandler::HTTP_GetAll(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    res->writeHead(200);
    res->setProperty("Content-Type", "application/json");
    res->end(JSONSerializer::createJSONMessage(
                 Agent::manyToJSON(Environment::getInstance()->getAgents(match.captured("class_name")) ),
                JSONSerializer::UPDATE )
             );
}

void HttpHandler::HTTP_GetOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    Agent* entity = Environment::getInstance()->getAgent(match.captured("class_name"), match.captured("id").toInt());
    if(entity){
        res->writeHead(200);
        res->setProperty("Content-Type", "application/json");
        res->end( JSONSerializer::createJSONMessage(entity->toJSON() , JSONSerializer::UPDATE ) );
    } else {
        res->writeHead(404);
        res->setProperty("Content-Type", "application/json");
        res->end();
    }
}

void HttpHandler::HTTP_Create(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body){
    Q_UNUSED(req);
    Q_UNUSED(res);
    Q_UNUSED(match);
    Q_UNUSED(body);
    /*Entity* entity = Environment::getInstance()->createEntity(class_name);
    if(entity){
        emit entity->sendWebSocketMessage(entity->toJSON(HttpServer::create_entity));
        res->writeHead(200);
        res->setProperty("Content-Type", "application/json");
        res->end(entity->toJSON(HttpServer::create_entity));
    } else {
        res->writeHead(400);
        res->setProperty("Content-Type", "application/json");
        res->end();
    }*/
}

void HttpHandler::HTTP_Update(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body){
    Q_UNUSED(req);
    Q_UNUSED(res);
    Q_UNUSED(body);
    Agent* entity = Environment::getInstance()->getAgent(match.captured("class_name"), match.captured("id").toInt());
    if(entity){
        entity->setFromFrontend( body );
        res->writeHead(200);
        res->setProperty("Content-Type", "application/json");
        entity->broadcastSocketMessage(JSONSerializer::createJSONMessage( entity->toJSON() , JSONSerializer::UPDATE) );
        res->end( JSONSerializer::createJSONMessage(entity->toJSON() , JSONSerializer::UPDATE ) );
    } else {
        res->writeHead(404);
        res->setProperty("Content-Type", "application/json");
        res->end();
    }
}

void HttpHandler::HTTP_RunAll(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    if(Environment::getInstance()->runAllAgents(match.captured("class_name"))){
        res->writeHead(200);
        res->setProperty("Content-Type", "application/json");
        res->end();
    } else {
        res->writeHead(404);
        res->setProperty("Content-Type", "application/json");
        res->end();
    }
}

void HttpHandler::HTTP_RunOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    Agent* entity = Environment::getInstance()->getAgent(match.captured("class_name"), match.captured("id").toInt());
    Agent* agent = dynamic_cast<Agent*> (entity);
    if(agent){
        Environment::getInstance()->runAgent(agent);
        res->writeHead(200);
        res->setProperty("Content-Type", "application/json");
        res->end( JSONSerializer::createJSONMessage( agent->toJSON(), JSONSerializer::UPDATE ) );
    } else {
        res->writeHead(404);
        res->setProperty("Content-Type", "application/json");
        res->end();
    }
}

void HttpHandler::HTTP_Stop(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    Agent* entity = Environment::getInstance()->getAgent(match.captured("class_name"), match.captured("id").toInt());
    Agent* agent = dynamic_cast<Agent*> (entity);
    if(agent){
        agent->stop();
        res->writeHead(200);
        res->setProperty("Content-Type", "application/json");
        res->end( JSONSerializer::createJSONMessage(agent->toJSON() , JSONSerializer::UPDATE ) );
    } else {
        res->writeHead(404);
        res->setProperty("Content-Type", "application/json");
        res->end();
    }
}

void HttpHandler::HTTP_Delete(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    Agent* agent = Environment::getInstance()->getAgent(match.captured("class_name"), match.captured("id").toInt());
    if (agent){
        Environment::getInstance()->deleteAgent( agent->getId() );
        res->writeHead(200);
        res->setProperty("Content-Type", "application/json");
        res->end( JSONSerializer::createJSONMessage( agent->toJSON() , JSONSerializer::DELETE ) );
        delete agent;
    } else {
        res->writeHead(404);
        res->setProperty("Content-Type", "application/json");
        res->end();
    }
}

void HttpHandler::HTTP_home(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body){
    Q_UNUSED(req);
    Q_UNUSED(match);
    Q_UNUSED(body);
    // Read index file
    QFile file("../web/index.html");
    QByteArray filecontent = "";
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            res->writeHead(404);
            res->end();
    } else {
        while (!file.atEnd()) {
                 filecontent += file.readLine();
             }
        res->setHeader("Content-Length", QString::number(filecontent.size()));
        res->writeHead(200);
        res->end(filecontent);
    }
}

void HttpHandler::HTTP_static(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    // Read the file
    QFile file("../web/" + match.captured("file"));
    QByteArray filecontent = "";
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            res->writeHead(404);
            res->end();
    } else {
        while (!file.atEnd()) {
                 filecontent += file.readLine();
             }
        res->setHeader("Content-Length", QString::number(filecontent.size()));
        res->writeHead(200);
        res->end(filecontent);
    }
}
