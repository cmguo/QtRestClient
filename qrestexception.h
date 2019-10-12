#ifndef QRESTERROR_H
#define QRESTERROR_H

#include <QException>

class QRestException : public QException
{
public:
    QRestException(int code, QString const & msg)
        : code_(code)
        , msg_(msg)
    {
    }

private:
    int code_;
    QString msg_;
};

#endif // QRESTERROR_H
