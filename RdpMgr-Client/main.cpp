#include "stdafx.h"
#include "RdpMgrClient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RdpMgrClient w;
    w.show();
    return a.exec();
}
