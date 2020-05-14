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

void QTestService::stest()
{
    QTestService * service = new QTestService();
    service->invoke("test", QVariantList{1, 2}).then([](QVariant const & d) {
        (void)d;
    }).tapFail([](std::exception & e) {
        qDebug() << e.what();
    }).wait();
}
