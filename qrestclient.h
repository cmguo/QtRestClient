#ifndef QTRESTCLIENT_H
#define QTRESTCLIENT_H

#include "QtRestClient_global.h"
#include "qrestrequest.h"
#include "qrestinterceptor.h"
#include "qrestjson.h"

#include <QtPromise>
#include <QByteArray>

class QNetworkAccessManager;

class QTRESTCLIENT_EXPORT QRestClient : QRestInterceptor
{
public:
    QRestClient(char const * baseUrl);

public:
    void addInterceptor(QRestInterceptor * interceptor);

public:
    QtPromise::QPromise<QByteArray> request(QRestRequest & req);

    template<template<typename> class R, typename T>
    QtPromise::QPromise<T> request(QRestRequest & req)
    {
        typedef R<T> RT;
        return request(req).then([this](QByteArray data){
            RT rt = json_.fromJson<RT>(data);
            rt.check();
            return rt.data();
        });
    }

    template<typename T>
    QtPromise::QPromise<T> request(QRestRequest & req)
    {
        return request(req).then([this](QByteArray data){
            return json_.fromJson<T>(data);
        });
    }

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
    virtual QtPromise::QPromise<QByteArray> intercept(QRestRequest & request);

private:
    QNetworkAccessManager * http_;
    QString base_url_;
    QRestInterceptor * interceptors_;
    QRestJson json_;
};

#endif // QTRESTCLIENT_H
