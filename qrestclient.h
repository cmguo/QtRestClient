#ifndef QTRESTCLIENT_H
#define QTRESTCLIENT_H

#include "QtRestClient_global.h"
#include "qrestrequest.h"
#include "qrestinterceptor.h"
#include "qrestjson.h"

#include <QByteArray>

namespace QtPromise {
    template <typename T> class QPromise;
}

class QNetworkAccessManager;

class QTRESTCLIENT_EXPORT QRestClient : public QRestInterceptor
{
public:
    QRestClient(char const * baseUrl, QRestJson::Flags jsonFlags = QRestJson::None);

public:
    void addInterceptor(QRestInterceptor * interceptor);

public:
    QtPromise::QPromise<QByteArray> request(QRestRequest & req);

    template<template<typename> class R, typename T>
    QtPromise::QPromise<T> request(QRestRequest & req);

    template<typename T>
    QtPromise::QPromise<T> request(QRestRequest & req);

    QString const & baseUrl() const
    {
        return base_url_;
    }

    QRestJson & json()
    {
        return json_;
    }

public:
    static void test();

private:
    virtual QtPromise::QPromise<QNetworkReply *> intercept(QNetworkRequest & request);

private:
    QNetworkAccessManager * http_;
    QString base_url_;
    QRestInterceptor * interceptors_;
    QRestJson json_;
};

#endif // QTRESTCLIENT_H
