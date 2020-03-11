#include "qrestjson.h"
#include "qrestjson.hpp"

QRestJson::QRestJson(Flags flags)
{
    serializer_ = new QJsonSerializer;
    setFlags(flags);
    serializer_->addJsonTypeConverter(
                QJsonTypeConverterStandardFactory<QJsonWrapperConverter>().createConverter());
}

QRestJson::~QRestJson()
{
    delete serializer_;
}

void QRestJson::setFlags(Flags flags)
{
    serializer_->setAllowDefaultNull(flags & AllowNull);
    QJsonSerializer::ValidationFlags vflags;
    if (flags & AllProperties)
        vflags |= QJsonSerializer::AllProperties;
    if (flags & NoExtraProperties)
        vflags |= QJsonSerializer::NoExtraProperties;
}
