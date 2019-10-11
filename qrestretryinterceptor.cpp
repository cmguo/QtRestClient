#include "qrestretryinterceptor.h"

QRestRetryInterceptor::QRestRetryInterceptor()
{

}

QtPromise::QPromise<QByteArray> QRestRetryInterceptor::intercept(QRestRequest & request)
{
    processNext(request).tapFail()
}
