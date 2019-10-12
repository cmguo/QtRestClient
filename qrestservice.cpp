#include "qrestservice.h"
#include "qrestretryinterceptor.h"
#include "qrestloginterceptor.h"

QRestService::QRestService(char const * baseUrl, QObject *parent)
    : QObject(parent)
    , client_(new QRestClient(baseUrl))
{
    client_->addInterceptor(new QRestLogInterceptor());
    client_->addInterceptor(new QRestRetryInterceptor());
}
