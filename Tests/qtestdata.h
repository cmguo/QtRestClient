#ifndef QTESTDATA_H
#define QTESTDATA_H

#include "QtRestClient_global.h"
#include "qrestjson.h"
#include "qrestbean.h"

#include <QObject>
#include <QVariant>

class QTestSubData : QRestBean<QTestSubData>
{
    Q_GADGET

public:
    Q_PROPERTY(int value1 MEMBER value1_)
    Q_PROPERTY(QString value2 MEMBER value2_)

public:
    QTestSubData();

private:
    int value1_;
    QString value2_;
};

Q_DECLARE_METATYPE(QTestSubData)

class QTestData : QRestBean<QTestData>
{
    Q_GADGET

public:
    Q_PROPERTY(int value1 MEMBER value1_)
    Q_PROPERTY(QString value2 MEMBER value2_)
    Q_PROPERTY(QTestSubData subData MEMBER subData_)
    Q_PROPERTY(QList<QTestSubData> subDatas MEMBER subDatas_)

public:
    QTestData();

    friend QDebug operator<<(QDebug debug, QTestData const & data);

private:
    int value1_ = 0;
    QString value2_;
    QTestSubData subData_;
    QList<QTestSubData> subDatas_;
};

Q_DECLARE_METATYPE(QTestData)

class QTestResult
{
    Q_GADGET

public:
    Q_PROPERTY(int result MEMBER result_)
    Q_PROPERTY(QString message MEMBER message_)
    Q_PROPERTY(QVariant data MEMBER data_)

public:
    static constexpr char const * dataName = "data";

public:
    QTestResult();

    void check() {
        if (result_ != 0)
            throw message_;
    }

private:
    int result_;
    QString message_;

protected:
    QVariant data_;
};

Q_DECLARE_METATYPE(QTestResult)

template <typename T>
class QTestResultT : public QTestResult
{
public:
    T && data() const
    {
        return std::move(*data_.value<T*>());
    }

    void setData(T && data)
    {
        data_ = QVariant::fromValue(std::forward<T>(data));
    }
};

Q_REST_DECLARE_WRAPPER_TEMPLATE(QTestResultT)

#include "qrestservice.hpp"
#include "qrestarg.h"

class QTestService : public QRestService
{
public:
    Q_REST_RESULT_WRAPPER(QTestResultT)

public:
    QTestService()
        : QRestService("http://www.baidu.com/")
    {
    }

    static void stest();

public:
    Q_RESTD_POST("test", test, QTestData(QQuery<REST_KEY("key")>, QHeader<REST_KEY("Ticket")>))
    Q_REST_POST("test2", test2, QTestData())
};

#endif // QTESTDATA_H
