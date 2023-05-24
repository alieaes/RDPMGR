#include "stdafx.h"
#include "cRdpMgrSvc.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cRdpMgrSvc w;
    w.show();
    return a.exec();
}
