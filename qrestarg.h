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

class QVariantEx : public QVariant
{
public:
    template<typename T>
    QVariantEx(T const & t)
    {
        setValue(t);
    }
    QVariantEx(QVariantEx const & o)
        : QVariant(o)
    {
    }
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

template <char const * const N, typename T = QVariantEx>
class QQuery : public QQueryBase
{
public:
    typedef T type;
    QQuery(T const & value)
        : QQueryBase(N, QVariantEx(value).toString())
    {
    }
};

template <char const * const N>
class QQuery<N, QVariantEx> : public QQueryBase
{
public:
    typedef QVariant type;
    template <typename T>
    QQuery(T const & value)
        : QQueryBase(N, QVariantEx(value).toString())
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

template <char const * N, typename T = QVariantEx>
class QHeader : public QHeaderBase
{
public:
    typedef T type;
    QHeader(T const & value)
        : QHeaderBase(N, QVariantEx(value).toString())
    {
    }
};

template <char const * const N>
class QHeader<N, QVariantEx> : public QHeaderBase
{
public:
    typedef QVariant type;
    template <typename T>
    QHeader(T const & value)
        : QHeaderBase(N, QVariantEx(value).toString())
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

template <char const * N, typename T = QVariantEx>
class QPath : public QPathBase
{
public:
    typedef T type;
    QPath(T const & value)
        : QPathBase(N, QVariantEx(value).toString())
    {
    }
};

template <char const * const N>
class QPath<N, QVariantEx> : public QPathBase
{
public:
    typedef QVariant type;
    template <typename T>
    QPath(T const & value)
        : QPathBase(N, QVariantEx(value).toString())
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
    virtual QVariant getBody(QRestJson & json, QMap<char const *, QByteArray> & headers) const = 0;

private:
    char const * name_;
};

template <typename T>
struct QBodyIsSingleValue
{
    static constexpr bool value = false;
};

template <>
struct QBodyIsSingleValue<QIODevice*>
{
    static constexpr bool value = true;
};

template <>
struct QBodyIsSingleValue<QByteArray>
{
    static constexpr bool value = true;
};

template <>
struct QBodyIsSingleValue<QString>
{
    static constexpr bool value = true;
};

template <typename T, char const * N = nullptr,
          bool JSON = QBodyIsSingleValue<T>::value>
class QBody : public QBodyBase
{
public:
    typedef T type;
    QBody(T value)
        : QBodyBase(N)
        , value_(value)
    {
    }

private:
    virtual QVariant getBody(QRestJson & json, QMap<char const *, QByteArray> & headers) const
    {
        headers.insert("Content-Type", "application/json");
        return json.toJson(value_);
    }

private:
    T value_;
};

template <typename T, char const * N>
class QBody<T, N, true> : public QBodyBase
{
public:
    typedef T type;
    QBody(T value)
        : QBodyBase(N)
        , value_(value)
    {
    }

private:
    virtual QVariant getBody(QRestJson &, QMap<char const *, QByteArray> & headers) const
    {
        QVariant value = QVariant::fromValue(value_);
        if (QMetaType::typeFlags(value.userType()).testFlag(QMetaType::PointerToQObject)) {
            QObject * obj = value.value<QObject*>();
            for (QByteArray k : obj->dynamicPropertyNames())
                headers.insert(k, obj->property(k).toByteArray());
        }
        return value;
    }

private:
    T value_;
};

#endif // QRESTARG_H
