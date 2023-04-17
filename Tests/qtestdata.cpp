#include "qtestdata.h"
#include "qrestservice.hpp"

QTestSubData::QTestSubData()
    : value1_(0)
    , value2_("ccc")
{
}

QTestData::QTestData()
    : value1(0)
    , value2("ccc")
{
}

QTestResult::QTestResult()
    : result_(0)
    , message_("ccc")
{
}

QDebug operator<<(QDebug debug, const QTestData &data)
{
    debug << data.value1 << data.value2;
    return debug;
}

void QTestService::stest()
{
    QTestService * service = new QTestService();

    service->test("k1", "ticket").then([] (QTestData data) {
        qDebug() << data;
    }).tapFail([](std::exception & e) {
        qDebug() << e.what();
    }).wait();

    service->invoke("test", QVariantList{1, 2}).then([](QVariant const & d) {
        (void)d;
    }).tapFail([](std::exception & e) {
        qDebug() << e.what();
    }).wait();
}
