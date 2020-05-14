#ifndef QRESTMOCKTERCEPTOR_H
#define QRESTMOCKTERCEPTOR_H

#include "QtRestClient_global.h"

#include "qrestinterceptor.h"

class QTRESTCLIENT_EXPORT QRestMockInterceptor : public QRestInterceptor
{
    Q_OBJECT
public:
    Q_INVOKABLE QRestMockInterceptor();

protected:
    virtual QtPromise::QPromise<QNetworkReply *> intercept(QNetworkRequest & request) override;
};

#endif // QRESTMOCKTERCEPTOR_H
