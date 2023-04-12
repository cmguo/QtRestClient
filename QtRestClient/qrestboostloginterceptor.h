#ifndef QRESTBOOSTLOGINTERCEPTOR_H
#define QRESTBOOSTLOGINTERCEPTOR_H

#include "qrestloginterceptor.h"

class QTRESTCLIENT_EXPORT QRestBoostLogInterceptor : public QRestLogInterceptor
{
private:
    bool logBody() const override;

    void log(int level, QString const & msg) const override;
};

#endif // QRESTBOOSTLOGINTERCEPTOR_H
