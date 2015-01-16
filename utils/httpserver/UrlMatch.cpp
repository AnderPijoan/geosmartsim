#include <QDebug>
#include "UrlMatch.h"

/**
 * @brief UrlMatch::UrlMatch
 * @param method
 * @param urlRegex
 */
UrlMatch::UrlMatch(QHttpRequest::HttpMethod method, QRegularExpression urlRegex) : QObject()
{
    this->method = method;
    this->url_regex = urlRegex;
}

UrlMatch::~UrlMatch(){
}

/**
 * @brief UrlMatch::match
 * @param url
 * @return
 */
QRegularExpressionMatch UrlMatch::match(QUrl url)
{
    QRegularExpressionMatch match = url_regex.match(url.path());
    return match;
}

void UrlMatch::storeReferencesUntillReqEnd(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch matchedRegexp){
    this->request = req;
    this->response = res;
    this->matched_regex = matchedRegexp;
}

/**********************************************************************
 GETTERS
**********************************************************************/

QHttpRequest::HttpMethod UrlMatch::getMethod() const{
    return this->method;
}

 QRegularExpression UrlMatch::getUrlRegex() const{
    return this->url_regex;
}

 /**********************************************************************
  SLOTS
 **********************************************************************/

void UrlMatch::requestEnded(){
    // Now parse body
    QMap<QVariant, QVariant> body;
    foreach(QByteArray parameter , this->request->body().split('&')){
        QList<QByteArray> keyAndValue = parameter.split('=');
        if (keyAndValue.size() == 2){
            body.insert( keyAndValue.at(0) , keyAndValue.at(1) );
        }
    }
    // Call the SLOT from HttpHandler that will be waiting these elements
    emit matched(this->request, this->response, this->matched_regex, body);
}
