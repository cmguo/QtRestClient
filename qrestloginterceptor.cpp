#include "qrestloginterceptor.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

QRestLogInterceptor::QRestLogInterceptor()
{
}

static char const * methodNames[] = {
    "HEAD", "GET", "PUT", "POST", "DELETE"
};

extern QNetworkRequest::Attribute AttributeMethod;
extern QNetworkRequest::Attribute AttributeBody;

QtPromise::QPromise<QNetworkReply *> QRestLogInterceptor::intercept(QNetworkRequest & request)
{
    int method = request.attribute(AttributeMethod).toInt();
    QByteArray body = request.attribute(AttributeBody).toByteArray();
    qDebug().noquote() << methodNames[method] << request.url().toString();
    for (auto h : request.rawHeaderList())
        qDebug().noquote() << h + ":" << request.rawHeader(h);
    if (!body.isEmpty())
        qDebug().nospace() << "\n" << body;
    return processNext(request).tap([](QNetworkReply * reply) {
        if (reply->error() != QNetworkReply::NoError && reply->error() <= QNetworkReply::UnknownNetworkError) {
            qDebug().noquote() << reply->error() << reply->errorString();
            return;
        }
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        QVariant reasonPhrase = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute);
        qDebug().noquote() << statusCode.toInt() << reasonPhrase.toString();
        for (auto h : reply->rawHeaderPairs())
            qDebug().noquote() << h.first + ":" << h.second;
        static char body[2048];
        int n = static_cast<int>(reply->peek(body, sizeof(body) - 1));
        if (n >= 0) {
            body[n] = 0;
            qDebug().nospace() << "\n" << body;
        }
    });
}

