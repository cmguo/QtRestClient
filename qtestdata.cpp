#include "qtestdata.h"

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
    service->test2().then([](QTestData d) {
        (void)d;
    }).tapFail([](std::exception & e) {
        qDebug() << e.what();
    }).wait();
}
