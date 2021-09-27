#include "qrestclient.h"
#include "qrestproxyinterceptor.h"

QRestProxyInterceptor::QRestProxyInterceptor()
{
}

void QRestProxyInterceptor::setUrl(const QByteArray &url)
{
    url_ = url;
}

void QRestProxyInterceptor::attached(QRestClient *client)
{
    client->setProxyUrl(url_);
    client->removeInterceptor(this);
    delete this;
}

QtPromise::QPromise<QNetworkReply *> QRestProxyInterceptor::intercept(QNetworkRequest &request)
{
    return processNext(request);
}
