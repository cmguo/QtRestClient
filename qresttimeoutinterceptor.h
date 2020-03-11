#ifndef QRESTTIMEOUTINTERCEPTOR_H
#define QRESTTIMEOUTINTERCEPTOR_H

#include "qrestinterceptor.h"

class QRestTimeoutInterceptor : public QRestInterceptor
{

    Q_OBJECT

    Q_PROPERTY(int timeout MEMBER timeout_)

public:
    QRestTimeoutInterceptor();

private:
    virtual QtPromise::QPromise<QNetworkReply *> intercept(QNetworkRequest & request);

private:
    int timeout_;
};

#endif // QRESTTIMEOUTINTERCEPTOR_H
