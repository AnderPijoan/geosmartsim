#include "utils/httpserver/UrlMatch.h"
#include "UrlCreator.h"

UrlCreator::UrlCreator(HttpHandler* handler)
{
    // CREATE SERVER URL
    UrlMatch *urlmatch = handler->addUrlMatch(QHttpRequest::HTTP_GET, QRegularExpression("^/$"));
    QObject::connect(urlmatch, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)), handler, SLOT(HTTP_home(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)));

    // CREATE STATIC CONTENT URLS
    urlmatch = handler->addUrlMatch(QHttpRequest::HTTP_GET, QRegularExpression("^/static/(?<file>[a-z0-9/.]+$)"));
    QObject::connect(urlmatch, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)), handler, SLOT(HTTP_static(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)));

    // CREATE ENVIRONMENT URLS
    urlmatch = handler->addUrlMatch(QHttpRequest::HTTP_GET, QRegularExpression("^/environment$"));
    QObject::connect(urlmatch, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)), handler, SLOT(HTTP_env(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)));

    urlmatch = handler->addUrlMatch(QHttpRequest::HTTP_GET, QRegularExpression("^/environment/time$"));
    QObject::connect(urlmatch, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)), handler, SLOT(HTTP_envTime(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)));

}

void UrlCreator::createUrlsForAgents(HttpHandler* handler, QString class_name, QStringList operations){
    UrlMatch* urlmatch;
    foreach (QString str, operations) {
        if(str == "GETALL"){
            // Get all
            urlmatch = handler->addUrlMatch(QHttpRequest::HTTP_GET, QRegularExpression("^/(?<class_name>" + class_name + "$)"));
            QObject::connect(urlmatch, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)), handler, SLOT(HTTP_GetAll(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)));
        }
        else if (str == "GETONE"){
            // Get one
             urlmatch = handler->addUrlMatch(QHttpRequest::HTTP_GET, QRegularExpression("^/(?<class_name>" + class_name + ")/(?<id>[a-z0-9]+$)"));
             QObject::connect(urlmatch, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)), handler, SLOT(HTTP_GetOne(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)));
        }
        else if (str == "CREATE"){
            // Create
             urlmatch = handler->addUrlMatch(QHttpRequest::HTTP_POST, QRegularExpression("^/(?<class_name>" + class_name + "$)"));
             QObject::connect(urlmatch, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)), handler, SLOT(HTTP_Create(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)));
        }
        else if (str == "UPDATE"){
            // Update
             urlmatch = handler->addUrlMatch(QHttpRequest::HTTP_PUT, QRegularExpression("^/(?<class_name>" + class_name + ")/(?<id>[a-z0-9]+$)"));
             QObject::connect(urlmatch, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)), handler, SLOT(HTTP_Update(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)));
        }
        else if (str == "RUNALL"){
            // Run all
              urlmatch = handler->addUrlMatch(QHttpRequest::HTTP_POST, QRegularExpression("^/(?<class_name>" + class_name + ")/run$"));
              QObject::connect(urlmatch, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)), handler, SLOT(HTTP_RunAll(QHttpRequest*,QHttpResponse*,QRegularExpressionMatch,QMap<QVariant,QVariant>)));
        }
        else if (str == "RUNONE"){
            // Run one
              urlmatch = handler->addUrlMatch(QHttpRequest::HTTP_POST, QRegularExpression("^/(?<class_name>" + class_name + ")/(?<id>[a-z0-9]+)/run$"));
              QObject::connect(urlmatch, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)), handler, SLOT(HTTP_RunOne(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)));
        }
        else if (str == "STOP"){
            // Stop
             urlmatch = handler->addUrlMatch(QHttpRequest::HTTP_POST, QRegularExpression("^/(?<class_name>" + class_name + ")/(?<id>[a-z0-9]+)/stop$"));
             QObject::connect(urlmatch, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)), handler, SLOT(HTTP_Stop(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)));
        }
        else if (str == "DELETE"){
            // Delete
            urlmatch = handler->addUrlMatch(QHttpRequest::HTTP_DELETE, QRegularExpression("^/(?<class_name>" + class_name + ")/(?<id>[a-z0-9]+$)"));
            QObject::connect(urlmatch, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)), handler, SLOT(HTTP_Delete(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QMap<QVariant, QVariant>)));
        }
    }
}


