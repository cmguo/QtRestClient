#include "qrestrequest.h"
#include "qrestarg.h"
#include "qrestclient.h"

#include <QNetworkRequest>

QRestRequest::QRestRequest(QRestRequest::Method method, const char *url)
    : method_(method)
    , url_(url)
    , query_(url_.query())
{
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
    for (auto i = headers_.keyValueBegin(); i != headers_.keyValueEnd(); ++i)
        req.setRawHeader((*i).first, (*i).second.toUtf8());
}

