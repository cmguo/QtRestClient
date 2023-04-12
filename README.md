# QtRestClient
运行于Qt的Rest-Ful协议客户端，类似 Java 语言的开源库 [Retrifit](https://github.com/square/retrofit)

# 特性：
+ 四种类型的参数：
   + 查询（query）
   + 路径（path）
   + 头域（header）
   + 数据（body）
+ 可扩展的请求拦截器，内置下列拦截器：
   + 重试
   + 网络日志
+ 数据序列化：目前仅 json [QtJsonSerializer](https://github.com/Skycoder42/QtJsonSerializer)
+ 异常通道，将带错误码的报文中的错误转换为异常
+ 异步：与[QtPromise](https://github.com/simonbrunel/qtpromise)结合，返回未来的（promise）数据
+ 简化REST服务定义：通过预编译宏简化服务接口实现

# 方案说明
[基于 Qt 的 REST 网络框架](https://blog.csdn.net/luansxx/article/details/120796569)

# 使用：
定义返回结果的报文格式（包括数据，QVariant类型）：
```cpp
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
```
将报文的数据部分泛型化
```
template <typename T>
class QTestResultT : public QTestResult
{
public:
    T const data() const
    {
        return data_.value<T>();
    }

    void setData(T const & data)
    {
        data_ = QVariant::fromValue(data);
    }
};

Q_REST_DECLARE_WRAPPER_TEMPLATE(QTestResultT)
```
定义返回数据（或者上传数据）格式
```
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
```
定义Rest服务
```
#include "qrestservice.h"
#include "qrestarg.h"

class QTRESTCLIENT_EXPORT QTestService : public QRestService
{
public:
    Q_REST_RESULT_WRAPPER(QTestResultT)

public:
    QTestService()
        : QRestService("http://www.baidu.com/")
    {
    }

public:
    Q_REST_POST("test", test, QTestData(QQuery<REST_KEY("k1")>, QHeader<REST_KEY("Ticket")>))
};
```
使用Rest服务
```cpp
QTestService service;
service.test(1, QString("ccc")).then([](QTestData d) {
   (void)d;
}).wait();
```
