#include <QCoreApplication>

#include "qtestdata.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTestService::stest();
    return a.exec();
}
