#include "qresttimeoutinterceptor.h"

#include <QNetworkRequest>

extern QNetworkRequest::Attribute AttributeTimeout;

QRestTimeoutInterceptor::QRestTimeoutInterceptor()
    : timeout_(10000)
{
}

QtPromise::QPromise<QNetworkReply *> QRestTimeoutInterceptor::intercept(QNetworkRequest &request)
{
    if (timeout_)
        request.setAttribute(AttributeTimeout, timeout_);
    return processNext(request);
}
