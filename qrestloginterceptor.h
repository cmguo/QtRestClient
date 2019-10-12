#ifndef QRESTLOGINTERCEPTOR_H
#define QRESTLOGINTERCEPTOR_H

#include "qrestinterceptor.h"

class QRestLogInterceptor : public QRestInterceptor
{
public:
    QRestLogInterceptor();

private:
    virtual QtPromise::QPromise<QNetworkReply *> intercept(QNetworkRequest & request);
};

#endif // QRESTLOGINTERCEPTOR_H
