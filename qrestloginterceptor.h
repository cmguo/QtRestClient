#ifndef QRESTLOGINTERCEPTOR_H
#define QRESTLOGINTERCEPTOR_H

#include "qrestinterceptor.h"

class QTRESTCLIENT_EXPORT QRestLogInterceptor : public QRestInterceptor
{
public:
    QRestLogInterceptor();

private:
    virtual QtPromise::QPromise<QNetworkReply *> intercept(QNetworkRequest & request) override;

private:
    static void log(int seq, QNetworkRequest & request);

    static void log(int seq, QNetworkReply & reply);
};

#endif // QRESTLOGINTERCEPTOR_H
