#ifndef QRESTJSON_H
#define QRESTJSON_H

#include "qjsonwrapperconvertor.h"

#include <variant>
#include <QtJsonSerializer/QJsonSerializer>

class QRestJson
{
public:
    enum Flag {
        None = 0,
        AllowNull = 1,
        AllProperties = 2,
        NoExtraProperties = 4,
    };

    Q_DECLARE_FLAGS(Flags, Flag)

public:
    QRestJson(Flags flags = None);

public:
    template<typename T>
    QByteArray toJson(T const * t)
    {
        return toJson(*t);
    }

    template<typename T>
    QByteArray toJson(T const & t)
    {
        return serializer_.serializeTo(t);
    }

    template<typename T>
    T fromJson(QByteArray json)
    {
        return serializer_.deserializeFrom<T>(json);
    }

private:
    QJsonSerializer serializer_;
};

#define Q_REST_DECLARE_WRAPPER_TEMPLATE(SINGLE_ARG_TEMPLATE) \
QT_BEGIN_NAMESPACE \
template <typename T> \
struct QMetaTypeId< SINGLE_ARG_TEMPLATE<T> > \
{ \
    enum { \
        Defined = QMetaTypeId2<T>::Defined \
    }; \
    static int qt_metatype_id() \
    { \
        static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0); \
        if (const int id = metatype_id.load()) \
            return id; \
        const char *tName = QMetaType::typeName(qMetaTypeId<T>()); \
        Q_ASSERT(tName); \
        const int tNameLen = int(qstrlen(tName)); \
        QByteArray typeName; \
        typeName.reserve(int(sizeof(#SINGLE_ARG_TEMPLATE)) + 1 + tNameLen + 1 + 1); \
        typeName.append(#SINGLE_ARG_TEMPLATE, int(sizeof(#SINGLE_ARG_TEMPLATE)) - 1) \
            .append('<').append(tName, tNameLen); \
        if (typeName.endsWith('>')) \
            typeName.append(' '); \
        typeName.append('>'); \
        const int newId = qRegisterNormalizedMetaType< SINGLE_ARG_TEMPLATE<T> >( \
                        typeName, \
                        reinterpret_cast< SINGLE_ARG_TEMPLATE<T> *>(quintptr(-1))); \
        QJsonWrapperConverter::registerWrapper(newId, SINGLE_ARG_TEMPLATE<T>::dataName, qMetaTypeId<T>()); \
        metatype_id.storeRelease(newId); \
        return newId; \
    } \
};

#endif // QRESTJSON_H