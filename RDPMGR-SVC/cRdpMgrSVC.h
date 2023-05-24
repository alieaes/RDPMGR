#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cRdpMgrSvc.h"

class cRdpMgrSvc : public QMainWindow
{
    Q_OBJECT

public:
    cRdpMgrSvc(QWidget *parent = nullptr);
    ~cRdpMgrSvc();

private:
    Ui::cRdpMgrSvcClass ui;
};
