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

public:
    QtPromise::QPromise<QVariant> invoke(QByteArray const & func, QVariant const & args);

protected:
    void setJsonFlags(QRestJson::Flags flags);

protected:
    class QTRESTCLIENT_EXPORT FunctionBase
    {
    public:
        QtPromise::QPromise<QVariant> invoke(QVariant const & args) const;
    protected:
        typedef QtPromise::QPromise<QVariant> (*invoke_t)(
                FunctionBase const & func, QVariant args);
        FunctionBase(QRestService * service, QRestRequest::Method method, char const * path,
                     char const * func, invoke_t invoke);
        FunctionBase(QRestService * service, QRestRequest::Method method, char const * path);
    protected:
        QRestService * service_;
        QRestRequest::Method method_;
        char const * path_;
        invoke_t invoke_;
    };

    template<template<typename> class R, typename F>
    class Function;

    template<template<typename> class R, typename T, typename ...Args>
    class Function<R, T (Args...)> : FunctionBase
    {
    public:
        Function(QRestService * service, QRestRequest::Method method, char const * path)
            : FunctionBase(service, method, path)
        {
        }
        Function(QRestService * service, QRestRequest::Method method, char const * path, char const * funcName)
            : FunctionBase(service, method, path, funcName, sinvoke)
        {
        }
        QtPromise::QPromise<T> operator()(Args const & ...args) const
        {
            QRestRequest request(method_, path_, args...);
            return service_->client_->request<R, T>(request);
        }
    private:
        static QtPromise::QPromise<QVariant> sinvoke(FunctionBase const & func, typename Args::type const & ...args);
        static QtPromise::QPromise<QVariant> sinvoke(FunctionBase const & func, QVariant args);
    };

protected:
    QRestClient * client_;

private:
    QMap<QByteArray, FunctionBase*> functions_;
};

#define Q_REST_KEY(K, k) static constexpr char const K[] = k;

#define Q_REST_RESULT_WRAPPER(wrapper) \
    template <typename T> \
    using Result = wrapper<T>;

#define Q_REST_FUNCTION(method, path, func, signature) \
    Function<Result, signature> const func = {this, QRestRequest::method, path};

#define Q_RESTD_FUNCTION(method, path, func, signature) \
    Function<Result, signature> const func = {this, QRestRequest::method, path, #func};

#define Q_REST_HEAD(path, func, signature) Q_REST_FUNCTION(Head, path, func, signature)
#define Q_REST_GET(path, func, signature) Q_REST_FUNCTION(Get, path, func, signature)
#define Q_REST_POST(path, func, signature) Q_REST_FUNCTION(Post, path, func, signature)
#define Q_REST_PUT(path, func, signature) Q_REST_FUNCTION(Put, path, func, signature)

#define Q_RESTD_HEAD(path, func, signature) Q_RESTD_FUNCTION(Head, path, func, signature)
#define Q_RESTD_GET(path, func, signature) Q_RESTD_FUNCTION(Get, path, func, signature)
#define Q_RESTD_POST(path, func, signature) Q_RESTD_FUNCTION(Post, path, func, signature)
#define Q_RESTD_PUT(path, func, signature) Q_RESTD_FUNCTION(Put, path, func, signature)

#endif // QRESTSERVICE_H
