#ifndef QRESTINTERCEPTOR_H
#define QRESTINTERCEPTOR_H

#include "QtRestClient_global.h"

#include <QObject>
#include <QtPromise>
#include <QByteArray>

class QNetworkReply;
class QNetworkRequest;

class QNetworkResponse : QObject
{
    Q_OBJECT

public:
    QNetworkResponse(QNetworkReply * reply);

private:
    QNetworkReply * reply;
};

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
    virtual QtPromise::QPromise<QNetworkReply *> intercept(QNetworkRequest & request) = 0;

protected:
    QtPromise::QPromise<QNetworkReply *> processNext(QNetworkRequest & request);

private:
    QRestInterceptor * next_;
};

#endif // QRESTINTERCEPTOR_H
