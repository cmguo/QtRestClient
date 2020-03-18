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
    Q_OBJECT

    Q_PROPERTY(QByteArray baseUrl READ baseUrl WRITE setBaseUrl)

public:
    QRestClient(char const * baseUrl, QRestJson::Flags jsonFlags = QRestJson::None);

public:
    void addInterceptor(QRestInterceptor * interceptor);

    void removeInterceptor(QRestInterceptor * interceptor);

    QRestInterceptor* interceptor(QMetaObject const & meta);

    QRestInterceptor* interceptor(QByteArray const & className);

    QVector<QRestInterceptor *> interceptors();

public:
    QtPromise::QPromise<QByteArray> request(QRestRequest & req);

    template<template<typename> class R, typename T>
    QtPromise::QPromise<T> request(QRestRequest & req);

    template<typename T>
    QtPromise::QPromise<T> request(QRestRequest & req);

    QByteArray const & baseUrl() const
    {
        return baseUrl_;
    }

    void setBaseUrl(QByteArray url);

    void setBaseHeader(char const * key, QString const & value);

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
    QByteArray baseUrl_;
    QMap<char const *, QString> baseHeaders_;
    QRestInterceptor * interceptors_;
    QRestJson json_;
};

#endif // QTRESTCLIENT_H
