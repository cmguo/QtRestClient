#ifndef QRESTARG_H
#define QRESTARG_H

#include "QtRestClient_global.h"

#include "qrestjson.h"

#include <QVariant>
#include <QIODevice>

class QRestRequest;
class QRestJson;

class QTRESTCLIENT_EXPORT QRestArg
{
public:
    QRestArg() {}

    virtual ~QRestArg() {}

public:
    virtual void apply(QRestJson & json, QRestRequest & req) const;

    virtual void apply(QRestRequest & req) const { (void) req; }

private:
    Q_DISABLE_COPY(QRestArg)
};

class QTRESTCLIENT_EXPORT QQueryBase : public QRestArg
{
public:
    QQueryBase(char const * name, QString value);

private:
    virtual void apply(QRestRequest & req) const;

private:
    char const * name_;
    QString value_;
};

template <char const * const N>
class QQuery : public QQueryBase
{
public:
    template<typename T>
    QQuery(T const & value)
        : QQueryBase(N, QVariant::fromValue(value).toString())
    {
    }
};

class QTRESTCLIENT_EXPORT QHeaderBase : public QRestArg
{
public:
    QHeaderBase(char const * name, QString value);

private:
    virtual void apply(QRestRequest & req) const;

private:
    char const * name_;
    QString value_;
};

template <char const * N>
class QHeader : public QHeaderBase
{
public:
    template<typename T>
    QHeader(T const & value)
        : QHeaderBase(N, QVariant::fromValue(value).toString())
    {
    }
};

class QTRESTCLIENT_EXPORT QPathBase : public QRestArg
{
public:
    QPathBase(char const * name, QString value);

private:
    virtual void apply(QRestRequest & req) const;

private:
    char const * name_;
    QString value_;
};

template <char const * N>
class QPath : public QPathBase
{
public:
    template<typename T>
    QPath(T const & value)
        : QPathBase(N, QVariant::fromValue(value).toString())
    {
    }
};

class QTRESTCLIENT_EXPORT QBodyBase : public QRestArg
{
public:
    QBodyBase(char const * name = nullptr);

private:
    virtual void apply(QRestJson & json, QRestRequest & req) const;

protected:
    virtual QByteArray getBody(QRestJson & json) const = 0;

private:
    char const * name_;
};


template <typename T, char const * N = nullptr>
class QBody : public QBodyBase
{
public:
    QBody(T value)
        : QBodyBase(N)
        , value_(value)
    {
    }

private:
    virtual QByteArray getBody(QRestJson & json) const
    {
        return json.toJson(value_);
    }

private:
    T value_;
};

template <char const * N>
class QBody<QIODevice*, N> : public QBodyBase
{
public:
    QBody(QIODevice* value)
        : QBodyBase(N)
        , value_(value)
    {
    }

private:
    virtual QByteArray getBody(QRestJson &) const
    {
        return value_->readAll();
    }

private:
    QIODevice* value_;
};

#endif // QRESTARG_H
