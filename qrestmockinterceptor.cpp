#include "qrestmockinterceptor.h"

#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>

QRestMockInterceptor::QRestMockInterceptor()
{
}

class MockReply : public QNetworkReply
{
public:
    MockReply(QFile * file) : file_(file) { open(ReadOnly); }

    // QIODevice interface
protected:
    qint64 readData(char *data, qint64 maxlen)
    {
        return file_->read(data, maxlen);
    }

    // QNetworkReply interface
public slots:
    void abort() {}

private:
    QFile * file_;
};

QtPromise::QPromise<QNetworkReply *> QRestMockInterceptor::intercept(QNetworkRequest & request)
{
    QFile * file = new QFile("restful" + request.url().path());
    if (file->open(QFile::ReadOnly)) {
        QNetworkReply * reply = new MockReply(file);
        return QtPromise::resolve(reply);
    } else {
        delete file;
        return processNext(request);
    }
}
