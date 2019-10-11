#ifndef QRESTINTERCEPTOR_H
#define QRESTINTERCEPTOR_H

#include "QtRestClient_global.h"

#include <QObject>
#include <QtPromise>
#include <QByteArray>

class QRestClient;
class QRestRequest;

class QTRESTCLIENT_EXPORT QRestInterceptor : public QObject
{
    Q_OBJECT
public:
    QRestInterceptor(QRestInterceptor *next = nullptr);

    virtual ~QRestInterceptor();

    void setNext(QRestInterceptor *next)
    {
        next_ = next;
    }

public:
    virtual QtPromise::QPromise<QByteArray> intercept(QRestRequest & request) = 0;

protected:
    QtPromise::QPromise<QByteArray> processNext(QRestRequest & request);

private:
    QRestInterceptor * next_;
};

#endif // QRESTINTERCEPTOR_H
