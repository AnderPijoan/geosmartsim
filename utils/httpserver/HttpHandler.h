#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <QUrl>
#include <QVector>
#include <qhttprequest.h>
#include <qhttpresponse.h>

#include "utils/httpserver/UrlMatch.h"

QT_FORWARD_DECLARE_CLASS(UrlMatch)

class HttpHandler : public QObject
{
     Q_OBJECT

public:
    HttpHandler();
    UrlMatch* addUrlMatch(QHttpRequest::HttpMethod method, QRegularExpression urlRegex);
    bool containsUrlMath(QHttpRequest::HttpMethod method, QRegularExpression urlRegex);

public slots:
    // MAIN HANDLER
    void handleHttpRequest(QHttpRequest *req, QHttpResponse *resp);
    void parsingFinished();

    // URL SLOTS
    void HTTP_GetAll(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body);
    void HTTP_GetOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body);
    void HTTP_Create(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body);
    void HTTP_Update(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body);
    void HTTP_RunAll(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body);
    void HTTP_RunOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body);
    void HTTP_Stop(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body);
    void HTTP_Delete(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body);

    // SERVER SLOTS
    void HTTP_home(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body);
    void HTTP_static(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body);

    // ENVIRONMENT SLOTS
    void HTTP_env(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body);
    void HTTP_envTime(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QMap<QVariant, QVariant> body);

private:
    bool busy; // Flag to determine if a previous request is still being processed
    QMap<QHttpRequest::HttpMethod, QVector<UrlMatch*> >urls;
};

#endif // HTTPHANDLER_H
