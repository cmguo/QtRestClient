#ifndef QRESTBEAN_H
#define QRESTBEAN_H

#include "qjsonwrapperconvertor.h"

#include <QtJsonSerializer/QJsonSerializer>

#include <QMetaType>

template <typename T>
class QRestAsList
{
    static constexpr bool value = false;
};

template <typename T>
class QRestBean
{
public:
    inline friend bool operator==(T const & l, T const & r)
    {
        (void) l;
        (void) r;
        return false;
    }

    inline friend bool operator!=(T const & l, T const & r)
    {
        (void) l;
        (void) r;
        return !(l == r);
    }

public:
    static void registerList()
    {
        if (!QMetaType::hasRegisteredConverterFunction<QVariantList, QList<T>>())
            QJsonSerializer::registerListConverters<T>();
    }
};

template <typename T>
class QRestBeanInitializer {
public:
    QRestBeanInitializer() {
        static bool init = false;
        if (init)
            return;
        T::doInit();
        init = true;
    }
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

#endif // QRESTBEAN_H
