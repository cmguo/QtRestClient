#ifndef QRESTSERVICE_H
#define QRESTSERVICE_H

#include "QtRestClient_global.h"

#include "qrestrequest.h"
#include "qrestclient.h"

#include <QObject>
#include <QtPromise>

class QTRESTCLIENT_EXPORT QRestService : public QObject
{
    Q_OBJECT
public:
    explicit QRestService(char const * baseUrl, QObject *parent = nullptr);

protected:
    void addInterceptor(QRestInterceptor * interceptor);

    void setJsonFlags(QRestJson::Flags flags);

protected:
    template<template<typename> class R, typename F>
    class Function;

    template<template<typename> class R, typename T, typename ...Args>
    class Function<R, T (Args...)>
    {
    public:
        Function(QRestService * service, QRestRequest::Method method, char const * path)
            : service_(service)
            , method_(method)
            , path_(path)
        {
        }

        QtPromise::QPromise<T> operator()(Args const & ...args) const
        {
            QRestRequest request(method_, path_, args...);
            return service_->client_->request<R, T>(request);
        }

    private:
        QRestService * service_;
        QRestRequest::Method method_;
        char const * path_;
    };

private:
    QRestClient * client_;
};

#define Q_REST_KEY(K, k) static constexpr char const K[] = k;

#define Q_REST_RESULT_WRAPPER(wrapper) \
    template <typename T> \
    using Result = wrapper<T>;

#define Q_REST_FUNCTION(method, path, func, signature) \
    Function<Result, signature> const func = {this, QRestRequest::method, path};

#define Q_REST_HEAD(path, func, signature) Q_REST_FUNCTION(Head, path, func, signature)
#define Q_REST_GET(path, func, signature) Q_REST_FUNCTION(Get, path, func, signature)
#define Q_REST_POST(path, func, signature) Q_REST_FUNCTION(Post, path, func, signature)
#define Q_REST_PUT(path, func, signature) Q_REST_FUNCTION(Put, path, func, signature)

#endif // QRESTSERVICE_H
