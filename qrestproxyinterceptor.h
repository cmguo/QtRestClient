#ifndef QRESTPROXYINTERCEPTOR_H
#define QRESTPROXYINTERCEPTOR_H

#include "QtRestClient_global.h"
#include "qrestinterceptor.h"

class QTRESTCLIENT_EXPORT QRestProxyInterceptor : public QRestInterceptor
{
public:
    QRestProxyInterceptor();

protected:
    void setUrl(QByteArray const & url);

private:
    virtual void attached(QRestClient *client) override;

    virtual QtPromise::QPromise<QNetworkReply *> intercept(QNetworkRequest &request) override;

private:
    QByteArray url_;
};

#endif // QRESTPROXYINTERCEPTOR_H
