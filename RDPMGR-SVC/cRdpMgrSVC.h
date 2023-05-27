#pragma once

#include <QtWidgets/QMainWindow>

#include "Shared_Defs.h"
#include "ui_cRdpMgrSvc.h"
#include "QT/SharedQtSwitch.h"

class cRdpMgrSvc : public QMainWindow
{
    Q_OBJECT

public:
    cRdpMgrSvc( QWidget* parent = nullptr );
    ~cRdpMgrSvc();

    bool                                     ConnectValidation( bool isUseMsg = false );

public slots:
    void                                     OnMainSwitchToggled( bool isChecked );
    void                                     on_btnConnectTEST_clicked();
    void                                     on_btnConnectApply_clicked();
    void                                     on_btnAbnormalApply_clicked();

private:
    void                                     toggleDefence( bool isUse );

    Ui::cRdpMgrSvcClass                      ui;
    Switch*                                  _wdgMainSwitch = NULLPTR;
};
