#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cRdpMgrSVC.h"

class cRdpMgrSVC : public QMainWindow
{
    Q_OBJECT

public:
    cRdpMgrSVC(QWidget *parent = nullptr);
    ~cRdpMgrSVC();

private:
    Ui::cRdpMgrClass ui;
};
