#include "qrestrequest.h"
#include "qrestarg.h"
#include "qrestclient.h"

#include <QNetworkRequest>

QRestRequest::QRestRequest()
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
    for (auto h : headers_)
        req.setRawHeader(h.first, h.second.toUtf8());
}

