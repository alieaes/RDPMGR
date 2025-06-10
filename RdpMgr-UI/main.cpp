#include "cRdpMgrUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cRdpMgrUI w;
    w.show();
    return a.exec();
}
