#include "qrestinterceptor.h"

QRestInterceptor::QRestInterceptor(QRestInterceptor * next)
    : next_(next)
{
}

QRestInterceptor::~QRestInterceptor()
{
}

void QRestInterceptor::attached(QRestClient *)
{
}

void QRestInterceptor::detached(QRestClient *)
{
}

QtPromise::QPromise<QNetworkReply *> QRestInterceptor::processNext(QNetworkRequest & request)
{
    return next_->intercept(request);
}
