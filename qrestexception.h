#ifndef QRESTERROR_H
#define QRESTERROR_H

#include <QException>

class QRestException : public QException
{
public:
    QRestException(int code, QString const & msg)
        : code_(code)
        , msg_(msg.toUtf8())
    {
    }

    int code() const
    {
        return code_;
    }

public:
    virtual char const* what() const noexcept
    {
        return msg_.data();
    }

private:
    int code_;
    QByteArray msg_;
};

#endif // QRESTERROR_H
