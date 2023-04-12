#include "qrestlog4qtinterceptor.h"

#include <log4qt/logger.h>

bool QRestLog4QtInterceptor::logBody() const
{
    static Log4Qt::Logger& log = *Log4Qt::Logger::logger("QRestLogInterceptor");
    return log.isEnabledFor(Log4Qt::Level::DEBUG_INT);
}

void QRestLog4QtInterceptor::log(int level, const QString &msg) const
{
    static Log4Qt::Logger& log = *Log4Qt::Logger::logger("QRestLogInterceptor");
    if (level == QtMsgType::QtInfoMsg)
        log.info(msg);
    else
        log.warn(msg);
}
