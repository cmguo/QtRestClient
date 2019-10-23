#include "qrestjson.h"

QRestJson::QRestJson(Flags flags)
{
    if (flags & AllowNull)
        serializer_.setAllowDefaultNull(true);
    QJsonSerializer::ValidationFlags vflags;
    if (flags & AllProperties)
        vflags |= QJsonSerializer::AllProperties;
    if (flags & NoExtraProperties)
        vflags |= QJsonSerializer::NoExtraProperties;
    serializer_.setValidationFlags(vflags);
    serializer_.addJsonTypeConverter(
                QJsonTypeConverterStandardFactory<QJsonWrapperConverter>().createConverter());
}
