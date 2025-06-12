#include "stdafx.h"
#include "RdpMgrUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RdpMgrUI w;
    w.show();
    return a.exec();
}
