#ifndef QJSONWRAPPERCONVERTOR_H
#define QJSONWRAPPERCONVERTOR_H

#include "QtRestClient_global.h"

#include <QtJsonSerializer/QJsonTypeConverter>
#include <map>

class QTRESTCLIENT_EXPORT QJsonWrapperConverter : public QJsonTypeConverter
{
public:
    bool canConvert(int metaTypeId) const override;
    QList<QJsonValue::Type> jsonTypes() const override;
    QJsonValue serialize(int propertyType, const QVariant &value, const SerializationHelper *helper) const override;
    QVariant deserialize(int propertyType, const QJsonValue &value, QObject *parent, const SerializationHelper *helper) const override;

public:
    void static registerWrapper(int wrapType, char const * dataName, int dataType);

private:
    static std::map<int, std::pair<char const *, int>> wrapTypes;

    int getSubtype(int mapType) const;
};

#endif // QJSONWRAPPERCONVERTOR_H
