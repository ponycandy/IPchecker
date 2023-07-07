#include "ConfigIpCheck.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    ConfigIpCheck c;
    c.show();
    return a.exec();
}
