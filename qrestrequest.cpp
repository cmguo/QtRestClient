#include "qrestrequest.h"
#include "qrestarg.h"
#include "qrestclient.h"

#include <QNetworkRequest>

QRestRequest::QRestRequest()
{

}

QRestRequest::QRestRequest(Method method, char const * path, ...)
    : method_(method)
    , url_(path)
    , query_(url_.query())
{
    va_list vl;
    va_start(vl, path);
    while (true) {
        QRestArg const * arg = va_arg(vl, QRestArg const *);
        if (arg == nullptr)
            break;
        args_.push_back(arg);
    }
}

void QRestRequest::toRequest(QRestClient & client, QNetworkRequest &req)
{
    for (auto arg : args_) {
        arg->apply(client.json(), *this);
    }
    args_.clear();
    QUrl url;
    if (url_.scheme() == nullptr)
        url.setUrl(client.baseUrl() + url_.toString());
    else
        url = url_;
    url.setQuery(query_);
    req.setUrl(url);
    for (auto h : headers_)
        req.setRawHeader(h.first, h.second.toUtf8());
}

