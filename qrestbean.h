#ifndef QRESTBEAN_H
#define QRESTBEAN_H

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

#endif // QRESTBEAN_H
