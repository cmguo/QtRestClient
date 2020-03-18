#include "qrestarg.h"
#include "qrestrequest.h"

#include <QUrl>
#include <QUrlQuery>

void QRestArg::apply(QRestJson & json, QRestRequest & req) const
{
    (void)json;
    apply(req);
}

QQueryBase::QQueryBase(char const * name, QString value)
    : name_(name)
    , value_(value)
{
}

void QQueryBase::apply(QRestRequest &req) const
{
    req.url_query().addQueryItem(name_, value_);
}

QHeaderBase::QHeaderBase(char const * name, QString value)
    : name_(name)
    , value_(value)
{
}

void QHeaderBase::apply(QRestRequest &req) const
{
    req.setHeader(name_, value_.toUtf8());
}

QPathBase::QPathBase(char const * name, QString value)
    : name_(name)
    , value_(value)
{
}

void QPathBase::apply(QRestRequest &req) const
{
    QString name = QString("{") + name_ + "}";
    req.setUrlPath(req.url().path().replace(name, value_));
}

QBodyBase::QBodyBase(char const * name)
    : name_(name)
{
}

void QBodyBase::apply(QRestJson &json, QRestRequest &req) const
{
    req.setHeader("Content-Type", "application/json");
    req.setBody(getBody(json));
}
