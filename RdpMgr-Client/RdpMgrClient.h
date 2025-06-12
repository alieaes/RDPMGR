#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RdpMgrClient.h"

class RdpMgrClient : public QMainWindow
{
    Q_OBJECT

public:
    RdpMgrClient(QWidget *parent = nullptr);
    ~RdpMgrClient();

private:
    Ui::RdpMgrClientClass ui;
};
