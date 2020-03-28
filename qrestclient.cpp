#include "qrestclient.h"
#include "qrestarg.h"
#include "qrestexception.h"

#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QNetworkReply>

using namespace QtPromise;

extern QNetworkRequest::Attribute AttributeMethod;
extern QNetworkRequest::Attribute AttributeBody;
extern QNetworkRequest::Attribute AttributeTimeout;

QNetworkRequest::Attribute AttributeMethod =
        static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User);
QNetworkRequest::Attribute AttributeBody =
        static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 1);
QNetworkRequest::Attribute AttributeTimeout =
        static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 16);

QRestClient::QRestClient(char const * baseUrl, QRestJson::Flags jsonFlags)
    : http_(new QNetworkAccessManager())
    , baseUrl_(baseUrl)
    , interceptors_(this)
    , json_(jsonFlags)
{
    //http_->setRedirectPolicy(QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy);
    //http_->setNetworkAccessible(QNetworkAccessManager::Accessible);
}

void QRestClient::addInterceptor(QRestInterceptor *interceptor)
{
    interceptor->setNext(interceptors_);
    interceptor->setParent(this);
    interceptors_ = interceptor;
    interceptor->attached(this);
}

void QRestClient::removeInterceptor(QRestInterceptor *interceptor)
{
    if (interceptors_ == interceptor) {
        interceptors_ = interceptor->next();
        interceptor->setNext(nullptr);
        interceptor->setParent(nullptr);
        interceptor->detached(this);
        return;
    }
    QRestInterceptor* i = interceptors_;
    while (i && i->next() != interceptor) {
        i = i->next();
    }
    if (i) {
        interceptor->setParent(nullptr);
        i->setNext(i->next()->next());
        interceptor->setNext(nullptr);
        interceptor->setParent(nullptr);
        interceptor->detached(this);
    }
}

QRestInterceptor *QRestClient::interceptor(const QMetaObject &meta)
{
    QRestInterceptor* i = interceptors_;
    while (i && i->metaObject() != &meta) {
        i = i->next();
    }
    return i;
}

QRestInterceptor *QRestClient::interceptor(const QByteArray &className)
{
    QRestInterceptor* i = interceptors_;
    while (i && className != i->metaObject()->className()) {
        i = i->next();
    }
    return i;
}

QVector<QRestInterceptor *> QRestClient::interceptors()
{
    QVector<QRestInterceptor *> vec;
    QRestInterceptor* i = interceptors_;
    while (i) {
        vec.prepend(i);
        i = i->next();
    }
    return vec;
}

QPromise<QByteArray> QRestClient::request(QRestRequest & req)
{
    QNetworkRequest request;
    req.toRequest(*this, request);
    request.setAttribute(AttributeMethod, static_cast<int>(req.method()));
    request.setAttribute(AttributeBody, req.body());
    QtPromise::QPromise<QNetworkReply *> reply = interceptors_->intercept(request);
    return reply.then([=](QNetworkReply * reply) {
        reply->deleteLater();
        if (reply->error() == QNetworkReply::NoError) {
            return reply->readAll();
        } else {
            throw QRestException(reply->error(), reply->errorString());
        }
    });
}

void QRestClient::setBaseUrl(QByteArray const & url)
{
    baseUrl_ = url;
}

void QRestClient::setBaseHeader(const char *key, const QByteArray &value)
{
    baseHeaders_.insert(key, value);
}

QByteArray QRestClient::proxyUrl() const
{
    QNetworkProxy proxy = http_->proxy();
    QUrl proxyUrl;
    if (proxy.type() == QNetworkProxy::HttpProxy)
       proxyUrl.setScheme("http");
    else if (proxy.type() == QNetworkProxy::Socks5Proxy)
       proxyUrl.setScheme("sock5");
    else
        return nullptr;
    proxyUrl.setHost(proxy.hostName());
    proxyUrl.setPort(proxy.port());
    return proxyUrl.toEncoded();
}

void QRestClient::setProxyUrl(const QByteArray &url)
{
    QUrl proxyUrl(url);
    QNetworkProxy proxy;
    if (proxyUrl.scheme().compare("http", Qt::CaseInsensitive) == 0)
        proxy.setType(QNetworkProxy::HttpProxy);
    else if (proxyUrl.scheme().compare("sock5", Qt::CaseInsensitive) == 0)
        proxy.setType(QNetworkProxy::Socks5Proxy);
    proxy.setHostName(proxyUrl.host());
    proxy.setPort(static_cast<quint16>(proxyUrl.port()));
    http_->setProxy(proxy);
}

QtPromise::QPromise<QNetworkReply *> QRestClient::intercept(QNetworkRequest & request)
{
    QNetworkReply * reply = nullptr;
    int method = request.attribute(AttributeMethod).toInt();
    QByteArray body = request.attribute(AttributeBody).toByteArray();
    switch (method) {
    case QRestRequest::Head:
        reply = http_->head(request);
        break;
    case QRestRequest::Get:
        reply = http_->get(request);
        break;
    case QRestRequest::Put:
        reply = http_->put(request, body);
        break;
    case QRestRequest::Post:
        reply = http_->post(request, body);
        break;
    case QRestRequest::Delete:
        reply = http_->deleteResource(request);
        break;
    }
    QPromise<QNetworkReply *> result([reply](
                                  const QPromiseResolve<QNetworkReply *>& resolve) {
        auto callback = [=]() {
            resolve(reply);
        };
        if (reply->isFinished()) {
            callback();
            return;
        }
        QObject::connect(reply, &QNetworkReply::finished, reply, callback, Qt::QueuedConnection);
    });
    QVariant timeout = request.attribute(AttributeTimeout);
    if (timeout.isValid()) {
        return result.timeout(timeout.toInt()).fail([reply](QtPromise::QPromiseTimeoutException const &) {
            reply->disconnect(SIGNAL(finished()), reply);
            reply->abort();
            return reply;
        });
    } else {
        return result;
    }
}
