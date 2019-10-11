#include "qrestclient.h"
#include "qrestarg.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

using namespace QtPromise;

QRestClient::QRestClient(char const * baseUrl)
    : http_(new QNetworkAccessManager())
    , base_url_(baseUrl)
    , interceptors_(this)
{
}

void QRestClient::addInterceptor(QRestInterceptor *interceptor)
{
    interceptor->setNext(interceptors_);
    interceptor->setParent(this);
    interceptors_ = interceptor;
}

QPromise<QByteArray> QRestClient::request(QRestRequest & req)
{
    return interceptors_->intercept(req);
}

QPromise<QByteArray> QRestClient::intercept(QRestRequest & req)
{
    QNetworkReply * reply = nullptr;
    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::BackgroundRequestAttribute, true);
    req.toRequest(*this, request);
    switch (req.method()) {
    case QRestRequest::Head:
        reply = http_->head(request);
        break;
    case QRestRequest::Get:
        reply = http_->get(request);
        break;
    case QRestRequest::Put:
        reply = http_->put(request, req.body());
        break;
    case QRestRequest::Post:
        reply = http_->post(request, req.body());
        break;
    case QRestRequest::Delete:
        reply = http_->deleteResource(request);
        break;
    }
    return QPromise<QByteArray>([reply](
                                  const QPromiseResolve<QByteArray>& resolve,
                                  const QPromiseReject<QByteArray>& reject) {
        auto callback = [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                //if (statusCode.toInt() >= 300) {
                //    auto reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute);
                //    reject(reinterpret_cast<QNetworkReply::NetworkError>(statusCode.toInt()));
                //}
                resolve(reply->readAll());
                //resolve("{\"result\": 2, \"message\": \"ddd\", \"data\": {\"value1\": 5, \"value2\": \"abc\"}}");
            } else {
                reject(reply->error());
            }
            reply->deleteLater();
        };
        if (reply->isFinished()) {
            callback();
            return;
        }
        QObject::connect(reply, &QNetworkReply::finished, callback);
    });
}
