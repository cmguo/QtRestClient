#ifndef QRESTREQUEST_H
#define QRESTREQUEST_H

#include "QtRestClient_global.h"

#include <QUrl>
#include <QUrlQuery>
#include <map>
#include <vector>

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
    QRestRequest();

    template<typename ...Args>
    QRestRequest(Method method, char const * path, Args const & ...args)
        : method_(method)
        , url_(path)
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

    QByteArray const & body() const
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
    void setUrlPath(QString const & path)
    {
        url_.setPath(path);
    }

    void addHeader(char const * key, QString const & value)
    {
        headers_.insert(std::make_pair(key, value));
    }

    void setBody(QByteArray const & body)
    {
        body_ = body;
    }

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
    std::vector<QRestArg const *> args_;
    QUrlQuery query_;
    std::map<char const *, QString> headers_;
    QByteArray body_;
};

#endif // QRESTREQUEST_H
