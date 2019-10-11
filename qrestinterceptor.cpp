#include "qrestinterceptor.h"

QRestInterceptor::QRestInterceptor(QRestInterceptor * next)
    : next_(next)
{
}

QRestInterceptor::~QRestInterceptor()
{
}

QtPromise::QPromise<QByteArray> QRestInterceptor::processNext(QRestRequest & request)
{
    return next_->intercept(request);
}
