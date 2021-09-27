#ifndef QRESTHOSTINTERCEPTOR_H
#define QRESTHOSTINTERCEPTOR_H

#include "QtRestClient_global.h"

#include "qrestinterceptor.h"

class QTRESTCLIENT_EXPORT QRestHostInterceptor : public QRestInterceptor
{
public:
    QRestHostInterceptor();

protected:
    virtual QString mapHost(QString const & host);

private:
    virtual QtPromise::QPromise<QNetworkReply *> intercept(QNetworkRequest & request) override;
};

#endif // QRESTHOSTINTERCEPTOR_H
