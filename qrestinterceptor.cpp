#include "qrestinterceptor.h"

QRestInterceptor::QRestInterceptor(QRestInterceptor * next)
    : next_(next)
{
}

QRestInterceptor::~QRestInterceptor()
{
}

QtPromise::QPromise<QNetworkReply *> QRestInterceptor::processNext(QNetworkRequest & request)
{
    return next_->intercept(request);
}
