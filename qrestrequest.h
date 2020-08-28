#ifndef QRESTREQUEST_H
#define QRESTREQUEST_H

#include "QtRestClient_global.h"

#include <QUrl>
#include <QUrlQuery>
#include <QMap>
#include <QVector>
#include <QVariant>
#include <QHttpPart>

class QNetworkRequest;
class QRestArg;
class QRestClient;
class QIODevice;

class QTRESTCLIENT_EXPORT QRestRequest
{
public:
    enum Method {
        Head,
        Get,
        Put,
        Post,
        Delete
    };

public:
    QRestRequest(Method method, char const * url);

    template<typename ...Args>
    QRestRequest(Method method, char const * url, Args const & ...args)
        : QRestRequest(method, url)
    {
        storeArgs(args...);
    }


public:
    Method method() const
    {
        return method_;
    }

    QUrl const & url() const
    {
        return url_;
    }

    QVariant const & body() const
    {
        return body_;
    }

public:
    QUrl & url()
    {
        return url_;
    }

    QUrlQuery & url_query()
    {
        return query_;
    }

public:
    void setUrlPath(QString const & path);

    void setHeader(char const * key, QByteArray const & value);

    void setBody(QByteArray const & body);

    void setBody(QIODevice * body);

    void setBody(QMap<char const *, QByteArray> const & headers, QVariant const & body);

    // multi-part
    void setBody(QByteArray const & key, QMap<char const *, QByteArray> const & headers, QVariant const & body);

public:
    void toRequest(QRestClient & client, QNetworkRequest & req);

private:
    void storeArgs() {}

    template<typename Arg, typename ...Args>
    void storeArgs(Arg const & arg, Args const & ...args)
    {
        args_.push_back(&arg);
        storeArgs(args...);
    }

private:
    Method method_;
    QUrl url_;
    QVector<QRestArg const *> args_;
    QUrlQuery query_;
    QMap<char const *, QByteArray> headers_;
    QVariant body_;
};

#endif // QRESTREQUEST_H
