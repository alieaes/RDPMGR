#include "stdafx.h"
#include "cRdpMgrClient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cRdpMgrClient w;
    w.show();
    return a.exec();
}
