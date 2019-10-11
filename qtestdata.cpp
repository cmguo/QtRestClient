#include "qtestdata.h"

QTestData::QTestData()
    : value1_(0)
    , value2_("ccc")
{
    staticMetaObject.propertyCount();
}

QTestResult::QTestResult()
    : result_(0)
    , message_("ccc")
{
    staticMetaObject.propertyCount();
}

void QTestService::stest()
{
    QTestService service;
    service.test(1, QString("ccc")).then([](QTestData d) {
        (void)d;
    }).wait();
}
