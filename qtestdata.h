#ifndef QTESTDATA_H
#define QTESTDATA_H

#include "QtRestClient_global.h"
#include "qrestjson.h"

#include <QObject>

class QTestData
{
    Q_GADGET

public:
    Q_PROPERTY(int value1 MEMBER value1_)
    Q_PROPERTY(QString value2 MEMBER value2_)

public:
    QTestData();

private:
    int value1_;
    QString value2_;
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

#include "qrestservice.h"
#include "qrestarg.h"

class QTRESTCLIENT_EXPORT QTestService : public QRestService
{
public:
    Q_REST_RESULT_WRAPPER(QTestResultT)

    Q_REST_KEY(K1, "k1")
    Q_REST_KEY(Ticket, "ticket")

public:
    QTestService()
        : QRestService("http://www.baidu.com/")
    {
    }

    static void stest();

public:
    Q_REST_POST("test", test, QTestData(QQuery<K1>, QHeader<Ticket>))
    Q_REST_POST("test2", test2, QTestData())
};

#endif // QTESTDATA_H
