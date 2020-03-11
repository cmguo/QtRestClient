#include "qrestretryinterceptor.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>

using namespace QtPromise;

QRestRetryInterceptor::QRestRetryInterceptor()
    : times_(3)
    , interval_(1000)
{
}

QPromise<QNetworkReply *> QRestRetryInterceptor::intercept(QNetworkRequest & request)
{
    return process(request, times_);
}

QPromise<QNetworkReply *> QRestRetryInterceptor::process(QNetworkRequest & request, int times)
{
    return processNext(request).then([this, times](QNetworkReply * reply) {
        if (reply->error() == QNetworkReply::NoError) {
            return QPromise<QNetworkReply *>::resolve(reply);
        } else if (times == 0 || !recoverable(reply->error())) {
            return QPromise<QNetworkReply *>::resolve(reply);
        } else {
            return QPromise<int>::resolve(0).delay(interval_).then([=]() {
                QNetworkRequest request = reply->request();
                reply->deleteLater();
                qDebug() << "QRestRetryInterceptor retry " << (times_ - times + 1);
                return process(request, times - 1);
            });
        }
    });
}

bool QRestRetryInterceptor::recoverable(QNetworkReply::NetworkError e)
{
    return e <= QNetworkReply::UnknownNetworkError || e >= QNetworkReply::ProtocolUnknownError;
}
