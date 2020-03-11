#include "qrestservice.h"
#include "qrestretryinterceptor.h"
#include "qrestloginterceptor.h"
#include "qresttimeoutinterceptor.h"

QRestService::QRestService(char const * baseUrl, QObject *parent)
    : QObject(parent)
    , client_(new QRestClient(baseUrl))
{
    client_->addInterceptor(new QRestTimeoutInterceptor());
    client_->addInterceptor(new QRestLogInterceptor());
    client_->addInterceptor(new QRestRetryInterceptor());
}

QVector<QRestInterceptor *> QRestService::interceptors()
{
    return client_->interceptors();
}

void QRestService::addInterceptor(QRestInterceptor * interceptor)
{
    client_->addInterceptor(interceptor);
}

void QRestService::setJsonFlags(QRestJson::Flags flags)
{
    client_->json().setFlags(flags);
}
