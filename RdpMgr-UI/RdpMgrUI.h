#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RdpMgrUI.h"

class RdpMgrUI : public QMainWindow
{
    Q_OBJECT

public:
    RdpMgrUI(QWidget *parent = nullptr);
    ~RdpMgrUI();

private:
    Ui::RdpMgrUIClass ui;
};
