#ifndef QRESTRETRYINTERCEPTOR_H
#define QRESTRETRYINTERCEPTOR_H

#include "qrestinterceptor.h"

class QRestRetryInterceptor : public QRestInterceptor
{
    Q_OBJECT

public:
    QRestRetryInterceptor();

public:
    Q_PROPERTY(int times MEMBER times_)
    Q_PROPERTY(QString interval MEMBER interval_)

private:
    virtual QtPromise::QPromise<QByteArray> intercept(QRestRequest & request);

private:
    int times_;
    int interval_;
};

#endif // QRESTRETRYINTERCEPTOR_H
