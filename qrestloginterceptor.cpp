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
    static int gseq = 0;
    int seq = ++gseq;
    QtPromise::QPromise<QNetworkReply *> result = processNext(request).tap([seq](QNetworkReply * reply) {
        log(seq, *reply);
    });
    log(seq, request);
    return result;
}

void QRestLogInterceptor::log(int seq, QNetworkRequest &request)
{
    static Log4Qt::Logger& log = *Log4Qt::Logger::logger("QRestLogInterceptor");
    int method = request.attribute(AttributeMethod).toInt();
    QString msg = QString::number(seq);
    msg.append('-');
    msg.append(methodNames[method]);
    msg.append(' ');
    msg.append(request.url().toString());
    msg.append('\n');
    for (auto h : request.rawHeaderList())
        msg.append(h + ':' + request.rawHeader(h) + '\n');
    if (log.isEnabledFor(Log4Qt::Level::DEBUG_INT)) {
        QByteArray body = request.attribute(AttributeBody).toByteArray();
        if (!body.isEmpty()) {
            msg.append('\n');
            msg.append(body);
        }
    }
    msg.append('\n');
    log.info(msg);
}

void QRestLogInterceptor::log(int seq, QNetworkReply &reply)
{
    static Log4Qt::Logger& log = *Log4Qt::Logger::logger("QRestLogInterceptor");
    if (reply.error() != QNetworkReply::NoError && reply.error() <= QNetworkReply::UnknownNetworkError) {
        char const * error = QMetaEnum::fromType<QNetworkReply::NetworkError>().valueToKey(reply.error());
        QString msg = QString::number(seq);
        msg.append('-');
        msg.append(error);
        msg.append(' ');
        msg.append(reply.errorString());
        msg.append('\n');
        log.warn(msg);
        return;
    }
    QVariant statusCode = reply.attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QVariant reasonPhrase = reply.attribute(QNetworkRequest::HttpReasonPhraseAttribute);
    QString msg = QString::number(seq);
    msg.append('-');
    msg.append(statusCode.toString());
    msg.append(' ');
    msg.append(reasonPhrase.toString());
    msg.append('\n');
    for (auto h : reply.rawHeaderPairs())
        msg.append(h.first + ':' + h.second + '\n');
    if (log.isEnabledFor(Log4Qt::Level::DEBUG_INT)) {
        static char body[2048];
        int n = static_cast<int>(reply.peek(body, sizeof(body) - 1));
        if (n >= 0) {
            body[n] = 0;
            msg.append('\n');
            msg.append(body);
        }
    }
    msg.append('\n');
    log.info(msg);
}

