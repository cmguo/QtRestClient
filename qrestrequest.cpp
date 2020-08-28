#include "qrestrequest.h"
#include "qrestarg.h"
#include "qrestclient.h"

#include <QHttpMultiPart>
#include <QNetworkRequest>

QRestRequest::QRestRequest(QRestRequest::Method method, const char *url)
    : method_(method)
    , url_(url)
    , query_(url_.query())
{
}

void QRestRequest::setUrlPath(const QString &path)
{
    url_.setPath(path);
}

void QRestRequest::setHeader(const char *key, const QByteArray &value)
{
    headers_.insert(key, value);
}

void QRestRequest::setBody(QByteArray const & body)
{
    body_ = body;
}

void QRestRequest::setBody(QIODevice *body)
{
    for (QByteArray k : body->dynamicPropertyNames())
        headers_.insert(k, body->property(k).toByteArray());
    body_ = QVariant::fromValue(body);
}

void QRestRequest::setBody(QMap<char const *, QByteArray> const & headers, const QVariant &body)
{
    assert(!body_.isValid());
    headers_.unite(headers);
    body_ = body;
}

void QRestRequest::setBody(const QByteArray &key, QMap<char const *, QByteArray> const & headers, const QVariant &body)
{
    if (key.isNull())
        return setBody(headers, body);
    if (!body_.isValid()) {
        body_ = QVariant::fromValue(new QHttpMultiPart());
    }
    QHttpMultiPart * multiPart = body_.value<QHttpMultiPart*>();
    assert(multiPart != nullptr);
    QHttpPart part;
    for (auto iter = headers.begin(); iter != headers.end(); ++iter)
        part.setRawHeader(iter.key(), iter.value());
    if (body.value<QIODevice*>())
        part.setBodyDevice(body.value<QIODevice*>());
    else
        part.setBody(body.toByteArray());
    multiPart->append(part);
}

void QRestRequest::toRequest(QRestClient & client, QNetworkRequest &req)
{
    if (url_.scheme() == nullptr) {
        for (auto i = client.baseHeaders().keyValueBegin();
                i != client.baseHeaders().keyValueEnd(); ++i)
            req.setRawHeader((*i).first, (*i).second);
    }
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
        req.setRawHeader((*i).first, (*i).second);
}

