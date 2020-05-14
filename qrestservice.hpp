#ifndef QRESTSERVICE_HPP
#define QRESTSERVICE_HPP

#include "qrestservice.h"
#include "qrestclient.hpp"

namespace restpriv {

    template<typename U, typename Arg, typename ...Args>
    inline bool fillArg(QtPrivate::List<Arg, Args...>, int index, QVariantList & list)
    {
        if (index >= list.size())
            list.append(QVariant());
        QVariant & arg = list[index];
        int t = qMetaTypeId<Arg>();
        if (!arg.canConvert(t))
            return false;
        if (!arg.convert(t))
            return false;
        return fillArg<U>(QtPrivate::List<Args...>(), ++index, list);
    }

    template<typename U>
    inline bool fillArg(QtPrivate::List<>, int, QVariantList &)
    {
        return true;
    }

    template<typename T>
    inline QtPromise::QPromise<QVariant> promiseWrap(QtPromise::QPromise<T> const & p)
    {
        return p.then([](T const & t) {
            return QVariant::fromValue(t);
        });
    }

    inline QtPromise::QPromise<QVariant> promiseWrap(QtPromise::QPromise<void> const & p)
    {
        return p.then([]() { return QVariant(); });
    }

}

template<template<typename> class R, typename T, typename ...Args>
QtPromise::QPromise<QVariant> QRestService::Function<R, T (Args...)>::sinvoke(
        FunctionBase const & func, typename Args::type const & ...args)
{
    return restpriv::promiseWrap(static_cast<Function const &>(func)(args...));
}

template<template<typename> class R, typename T, typename ...Args>
QtPromise::QPromise<QVariant> QRestService::Function<R, T (Args...)>::sinvoke(
        const QRestService::FunctionBase &func, QVariant args)
{
    typedef QtPromise::QPromise<QVariant> (*Func)(FunctionBase const &, typename Args::type const & ...);
    typedef QtPrivate::FunctionPointer<Func> FuncType;
    QVariantList list;
    if (args.canConvert(qMetaTypeId<QVariantList>())
        && args.convert(qMetaTypeId<QVariantList>())) {
        list = args.toList();
    } else {
        list.append(args);
    }
    typedef QtPromise::QPromise<QVariant> (*Func2)(typename Args::type const & ...);
    typedef QtPrivate::FunctionPointer<Func2> Func2Type;
    typename Func2Type::Arguments types;
    restpriv::fillArg<int>(types, 0, list);
    QVector<void*> vec;
    QtPromise::QPromise<QVariant> ret = QtPromise::QPromise<QVariant>::resolve(QVariant());
    vec.append(&ret);
    vec.append(const_cast<FunctionBase*>(&func));
    for (QVariant & l : list)
        vec.append(l.data());
    Func f = &Function::sinvoke;
    FuncType::template call<typename FuncType::Arguments, QtPromise::QPromise<QVariant>>(f, nullptr, vec.data());
    return ret;
}

#endif // QRESTSERVICE_HPP
