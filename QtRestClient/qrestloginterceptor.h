#ifndef QRESTLOGINTERCEPTOR_H
#define QRESTLOGINTERCEPTOR_H

#include "qrestinterceptor.h"

class QTRESTCLIENT_EXPORT QRestLogInterceptor : public QRestInterceptor
{
public:
    static QRestLogInterceptor *create();

private:
    virtual QtPromise::QPromise<QNetworkReply *> intercept(QNetworkRequest & request) override;

private:
    void log(int seq, QNetworkRequest & request);

    void log(int seq, QNetworkReply & reply);

    virtual bool logBody() const;

    virtual void log(int level, QString const & msg) const;
};

#endif // QRESTLOGINTERCEPTOR_H
