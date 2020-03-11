#include "qresttimeoutinterceptor.h"

#include <qpromiseexceptions.h>

#include <QNetworkRequest>

QRestTimeoutInterceptor::QRestTimeoutInterceptor()
    : timeout_(10000)
{
}

QtPromise::QPromise<QNetworkReply *> QRestTimeoutInterceptor::intercept(QNetworkRequest &request)
{
    if (timeout_)
        request.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 1), timeout_);
    return processNext(request);
}
