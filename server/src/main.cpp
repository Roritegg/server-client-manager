#include "server.h"
#include <QCoreApplication>

int main(int argc, char** argv)
{
    QCoreApplication a(argc, argv);

    Server s;
    s.open("9999");

    return a.exec();
}
