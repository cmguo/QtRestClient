#ifndef QRESTJSON_H
#define QRESTJSON_H

#include "QtRestClient_global.h"

class QJsonSerializer;

class QTRESTCLIENT_EXPORT QRestJson
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

    ~QRestJson();

    void setFlags(Flags flags);

public:
    template<typename T>
    QByteArray toJson(T const * t);

    template<typename T>
    QByteArray toJson(T const & t);

    template<typename T>
    T fromJson(QByteArray const & json, T * unused = nullptr);

    void fromJson(QByteArray const & , void *)
    {
    }

private:
    Q_DISABLE_COPY(QRestJson)

    QJsonSerializer* serializer_;
};

#endif // QRESTJSON_H
