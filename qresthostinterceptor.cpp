#include "qresthostinterceptor.h"

#include <QtNetwork/QNetworkRequest>

QRestHostInterceptor::QRestHostInterceptor()
{

}

QString QRestHostInterceptor::mapHost(const QString &host)
{
    return host;
}

QtPromise::QPromise<QNetworkReply *> QRestHostInterceptor::intercept(QNetworkRequest &request)
{
    QString host = request.url().host();
    QString host2 = mapHost(host);
    if (host != host2) {
        QUrl url = request.url();
        url.setHost(host2);
        request.setUrl(url);
    }
    return processNext(request);
}
