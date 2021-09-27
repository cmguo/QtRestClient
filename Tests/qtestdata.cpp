#include "qtestdata.h"
#include "qrestservice.hpp"

QTestSubData::QTestSubData()
    : value1_(0)
    , value2_("ccc")
{
}

QTestData::QTestData()
    : value1_(0)
    , value2_("ccc")
{
}

QTestResult::QTestResult()
    : result_(0)
    , message_("ccc")
{
}

QDebug operator<<(QDebug debug, const QTestData &data)
{
    debug << data.value1_ << data.value2_;
    return debug;
}

void QTestService::stest()
{
    QTestService * service = new QTestService();

    service->test(QByteArray{"k1"}, QByteArray{"ticket"}).then([] (QTestData data) {
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
