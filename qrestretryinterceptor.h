#ifndef QRESTRETRYINTERCEPTOR_H
#define QRESTRETRYINTERCEPTOR_H

#include "qrestinterceptor.h"
#include <QNetworkReply>

class QRestRetryInterceptor : public QRestInterceptor
{
    Q_OBJECT

    Q_PROPERTY(int times MEMBER times_)
    Q_PROPERTY(int interval MEMBER interval_)

public:
    QRestRetryInterceptor();

private:
    virtual QtPromise::QPromise<QNetworkReply *> intercept(QNetworkRequest & request);

    QtPromise::QPromise<QNetworkReply *> process(QNetworkRequest & request, int times);

    bool recoverable(QNetworkReply::NetworkError e);

private:
    int times_;
    int interval_;
};

#endif // QRESTRETRYINTERCEPTOR_H
