#ifndef QRESTARG_H
#define QRESTARG_H

#include "QtRestClient_global.h"

#include "qrestjson.h"

#include <QVariant>
#include <QIODevice>

#include <type_traits>

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

#define MACRO_GET_1(str, i) \
    (sizeof(str) > (i) ? str[(i)] : 0)

#define MACRO_GET_2(str, i) \
    MACRO_GET_1(str, i+0),  \
    MACRO_GET_1(str, i+1)

#define MACRO_GET_4(str, i) \
    MACRO_GET_2(str, i+0),  \
    MACRO_GET_2(str, i+2)

#define MACRO_GET_8(str, i) \
    MACRO_GET_4(str, i+0),  \
    MACRO_GET_4(str, i+4)

#define MACRO_GET_16(str, i) \
    MACRO_GET_8(str, i+0),   \
    MACRO_GET_8(str, i+8)

#define MACRO_GET_32(str, i) \
    MACRO_GET_16(str, i+0),  \
    MACRO_GET_16(str, i+16)

#define MACRO_GET_64(str, i) \
    MACRO_GET_32(str, i+0),  \
    MACRO_GET_32(str, i+32)

#define REST_KEY(str) std::conditional_t<(sizeof(str) > 32), \
    std::integer_sequence<char, MACRO_GET_64(str, 0)>, \
    std::conditional_t<(sizeof(str) > 16), \
        std::integer_sequence<char, MACRO_GET_32(str, 0)>, \
        std::conditional_t<(sizeof(str) > 8), \
            std::integer_sequence<char, MACRO_GET_16(str, 0)>, \
            std::conditional_t<(sizeof(str) > 4), \
                std::integer_sequence<char, MACRO_GET_8(str, 0)>, \
                std::integer_sequence<char, MACRO_GET_4(str, 0)>>>>>

template <char... chars>
char const * rest_key(std::integer_sequence<char, chars...>)
{
    static constexpr char str[sizeof...(chars) + 1] = { chars..., '\0' };
    return str;
}

class QTRESTCLIENT_EXPORT QQueryBase : public QRestArg
{
public:
    QQueryBase(char const * name, QString value);

private:
    void apply(QRestRequest & req) const override;

private:
    char const * name_;
    QString value_;
};

template <typename K, typename T = QVariantEx>
class QQuery : public QQueryBase
{
public:
    typedef T type;
    QQuery(T const & value)
        : QQueryBase(rest_key(K()), QVariantEx(value).toString())
    {
    }
};

template <typename K>
class QQuery<K, QVariantEx> : public QQueryBase
{
public:
    typedef QVariant type;
    template <typename T>
    QQuery(T const & value)
        : QQueryBase(rest_key(K()), QVariantEx(value).toString())
    {
    }
};

class QTRESTCLIENT_EXPORT QHeaderBase : public QRestArg
{
public:
    QHeaderBase(char const * name, QString value);

private:
    void apply(QRestRequest & req) const override;

private:
    char const * name_;
    QString value_;
};

template <typename K, typename T = QVariantEx>
class QHeader : public QHeaderBase
{
public:
    typedef T type;
    QHeader(T const & value)
        : QHeaderBase(rest_key(K()), QVariantEx(value).toString())
    {
    }
};

template <typename K>
class QHeader<K, QVariantEx> : public QHeaderBase
{
public:
    typedef QVariant type;
    template <typename T>
    QHeader(T const & value)
        : QHeaderBase(rest_key(K()), QVariantEx(value).toString())
    {
    }
};

class QTRESTCLIENT_EXPORT QPathBase : public QRestArg
{
public:
    QPathBase(char const * name, QString value);

private:
    void apply(QRestRequest & req) const override;

private:
    char const * name_;
    QString value_;
};

template <typename K, typename T = QVariantEx>
class QPath : public QPathBase
{
public:
    typedef T type;
    QPath(T const & value)
        : QPathBase(rest_key(K()), QVariantEx(value).toString())
    {
    }
};

template <typename K>
class QPath<K, QVariantEx> : public QPathBase
{
public:
    typedef QVariant type;
    template <typename T>
    QPath(T const & value)
        : QPathBase(rest_key(K()), QVariantEx(value).toString())
    {
    }
};

class QTRESTCLIENT_EXPORT QBodyBase : public QRestArg
{
public:
    QBodyBase(char const * name = nullptr);

private:
    void apply(QRestJson & json, QRestRequest & req) const override;

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
