#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CRdpMgrClient.h"

class CRdpMgrClient : public QMainWindow
{
    Q_OBJECT

public:
    CRdpMgrClient(QWidget *parent = nullptr);
    ~CRdpMgrClient();

private:
    Ui::CRdpMgrClientClass ui;
};
