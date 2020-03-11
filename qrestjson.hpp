#ifndef QRESTJSON_HPP
#define QRESTJSON_HPP

#include "qrestjson.h"
#include "qjsonwrapperconvertor.h"

#include <variant>
#include <QtJsonSerializer/QJsonSerializer>

template<typename T>
QByteArray QRestJson::toJson(T const * t)
{
    return toJson(*t);
}

template<typename T>
QByteArray QRestJson::toJson(T const & t)
{
    return serializer_->serializeTo(t);
}

template<typename T>
T QRestJson::fromJson(QByteArray json)
{
    return serializer_->deserializeFrom<T>(json);
}


#endif // QRESTJSON_HPP
