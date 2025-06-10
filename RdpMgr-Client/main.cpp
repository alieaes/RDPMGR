#include "CRdpMgrClient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CRdpMgrClient w;
    w.show();
    return a.exec();
}
