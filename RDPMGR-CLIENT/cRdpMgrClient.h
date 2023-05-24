#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cRdpMgrClient.h"

class cRdpMgrClient : public QMainWindow
{
    Q_OBJECT

public:
    cRdpMgrClient(QWidget *parent = nullptr);
    ~cRdpMgrClient();

private:
    Ui::cRdpMgrClientClass ui;
};
