#include "qrestjson.h"

QRestJson::QRestJson(Flags flags)
{
    setFlags(flags);
    serializer_.addJsonTypeConverter(
                QJsonTypeConverterStandardFactory<QJsonWrapperConverter>().createConverter());
}

void QRestJson::setFlags(Flags flags)
{
    serializer_.setAllowDefaultNull(flags & AllowNull);
    QJsonSerializer::ValidationFlags vflags;
    if (flags & AllProperties)
        vflags |= QJsonSerializer::AllProperties;
    if (flags & NoExtraProperties)
        vflags |= QJsonSerializer::NoExtraProperties;
}
