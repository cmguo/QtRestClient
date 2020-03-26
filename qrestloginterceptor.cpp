#include "qrestloginterceptor.h"

#include <log4qt/logger.h>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QMetaEnum>

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
    QtPromise::QPromise<QNetworkReply *> result = processNext(request).tap([](QNetworkReply * reply) {
        log(*reply);
    });
    log(request);
    return result;
}

void QRestLogInterceptor::log(QNetworkRequest &request)
{
    static Log4Qt::Logger& log = *Log4Qt::Logger::logger("QRestLogInterceptor");
    int method = request.attribute(AttributeMethod).toInt();
    QByteArray body = request.attribute(AttributeBody).toByteArray();
    QString msg = methodNames[method];
    msg.append(' ');
    msg.append(request.url().toString());
    msg.append('\n');
    for (auto h : request.rawHeaderList())
        msg.append(h + ':' + request.rawHeader(h) + '\n');
    if (!body.isEmpty()) {
        msg.append('\n');
        msg.append(body);
    }
    log.debug(msg);
}

void QRestLogInterceptor::log(QNetworkReply &reply)
{
    static Log4Qt::Logger& log = *Log4Qt::Logger::logger("QRestLogInterceptor");
    if (reply.error() != QNetworkReply::NoError && reply.error() <= QNetworkReply::UnknownNetworkError) {
        char const * error = QMetaEnum::fromType<QNetworkReply::NetworkError>().valueToKey(reply.error());
        log.warn(error + (' ' + reply.errorString()));
        return;
    }
    QVariant statusCode = reply.attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QVariant reasonPhrase = reply.attribute(QNetworkRequest::HttpReasonPhraseAttribute);
    QString msg = statusCode.toString();
    msg.append(' ');
    msg.append(reasonPhrase.toString());
    msg.append('\n');
    for (auto h : reply.rawHeaderPairs())
        msg.append(h.first + ':' + h.second + '\n');
    static char body[2048];
    int n = static_cast<int>(reply.peek(body, sizeof(body) - 1));
    if (n >= 0) {
        body[n] = 0;
        msg.append('\n');
        msg.append(body);
    }
    log.debug(msg);
}

