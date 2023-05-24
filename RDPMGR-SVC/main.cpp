#include "stdafx.h"
#include "cRdpMgrSVC.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cRdpMgrSVC w;
    w.show();
    return a.exec();
}
