#ifndef QRESTLOG4QTINTERCEPTOR_H
#define QRESTLOG4QTINTERCEPTOR_H

#include "qrestloginterceptor.h"

class QTRESTCLIENT_EXPORT QRestLog4QtInterceptor : public QRestLogInterceptor
{
private:
    bool logBody() const override;

    void log(int level, QString const & msg) const override;
};

#endif // QRESTLOG4QTINTERCEPTOR_H
