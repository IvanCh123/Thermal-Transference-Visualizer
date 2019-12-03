#include <QCoreApplication>

#include "HeatMapTester.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    HeatMapTester application(argc, argv);
    return application.run();
}
