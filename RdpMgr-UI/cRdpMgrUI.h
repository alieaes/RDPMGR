#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cRdpMgrUI.h"

class cRdpMgrUI : public QMainWindow
{
    Q_OBJECT

public:
    cRdpMgrUI(QWidget *parent = nullptr);
    ~cRdpMgrUI();

private:
    Ui::cRdpMgrUIClass ui;
};
