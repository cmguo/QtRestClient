#include "qrestservice.h"

QRestService::QRestService(char const * baseUrl, QObject *parent)
    : QObject(parent)
    , client_(new QRestClient(baseUrl))
{

}
