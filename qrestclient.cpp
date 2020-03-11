#include "qrestclient.h"
#include "qrestarg.h"
#include "qrestexception.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

using namespace QtPromise;

QRestClient::QRestClient(char const * baseUrl, QRestJson::Flags jsonFlags)
    : http_(new QNetworkAccessManager())
    , base_url_(baseUrl)
    , interceptors_(this)
    , json_(jsonFlags)
{
    //http_->setRedirectPolicy(QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy);
}

void QRestClient::addInterceptor(QRestInterceptor *interceptor)
{
    interceptor->setNext(interceptors_);
    interceptor->setParent(this);
    interceptors_ = interceptor;
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
    request.setAttribute(QNetworkRequest::User, static_cast<int>(req.method()));
    request.setAttribute(QNetworkRequest::UserMax, req.body());
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

QtPromise::QPromise<QNetworkReply *> QRestClient::intercept(QNetworkRequest & request)
{
    QNetworkReply * reply = nullptr;
    int method = request.attribute(QNetworkRequest::User).toInt();
    QByteArray body = request.attribute(QNetworkRequest::UserMax).toByteArray();
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
    return QPromise<QNetworkReply *>([reply](
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
}
