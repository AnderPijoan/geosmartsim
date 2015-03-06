#ifndef URLMATCH_H
#define URLMATCH_H

#include <QString>
#include <QRegularExpression>
#include <QUrl>
#include <qhttpresponse.h>

#include "utils/http_server/HttpHandler.h"

class UrlMatch : public QObject
{
     Q_OBJECT

public:
    UrlMatch(QHttpRequest::HttpMethod method, QRegularExpression url_regex);
    ~UrlMatch();
    QRegularExpressionMatch match(QUrl url);
    void storeReferencesUntillReqEnd(QHttpRequest *request, QHttpResponse *response, QRegularExpressionMatch matched_regex);

    //Getters
    QHttpRequest::HttpMethod getMethod() const;
    QRegularExpression getUrlRegex() const;

signals:
    void matched(QHttpRequest *request, QHttpResponse *response, QRegularExpressionMatch matched_regex, QMap<QVariant, QVariant> body);

public slots:
    void requestEnded();

private:
    QHttpRequest::HttpMethod method; // HTTP METHOD (GET, POST, PUT, DELETE)
    QRegularExpression url_regex; // URL REGEX

    // Store request references, untill request has finished for POST and PUT
    QHttpRequest *request;
    QHttpResponse *response;
    QRegularExpressionMatch matched_regex;
};

#endif // URLMATCH_H
