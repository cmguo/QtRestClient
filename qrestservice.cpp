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

QtPromise::QPromise<QVariant> QRestService::invoke(QByteArray const & func, const QVariant &args)
{
    FunctionBase * f = functions_.value(func);
    if (f == nullptr) {
        return QtPromise::QPromise<QVariant>::reject(
                    std::runtime_error("function not found"));
    }
    return f->invoke(args);
}

void QRestService::setJsonFlags(QRestJson::Flags flags)
{
    client_->json().setFlags(flags);
}

QRestService::FunctionBase::FunctionBase(QRestService *service, QRestRequest::Method method, const char *path, const char *func, QRestService::FunctionBase::invoke_t invoke)
    : service_(service)
    , method_(method)
    , path_(path)
    , invoke_(invoke)
{
    service_->functions_.insert(func, this);
}

QtPromise::QPromise<QVariant> QRestService::FunctionBase::invoke(const QVariant &args) const
{
    return invoke_(*this, args);
}
