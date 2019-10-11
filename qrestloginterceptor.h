#ifndef QRESTLOGINTERCEPTOR_H
#define QRESTLOGINTERCEPTOR_H

#include "qrestinterceptor.h"

class QRestLogInterceptor : public QRestInterceptor
{
public:
    QRestLogInterceptor();

private:
    virtual QtPromise::QPromise<QByteArray> intercept(QRestRequest & request);
};

#endif // QRESTLOGINTERCEPTOR_H
