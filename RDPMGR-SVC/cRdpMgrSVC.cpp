#include "stdafx.h"
#include "cRdpMgrSvc.h"

#include "moduleDefence.h"

cRdpMgrSvc::cRdpMgrSvc( QWidget* parent )
    : QMainWindow( parent )
{
    ui.setupUi(this);

    auto defence = TyStDefence::GetInstance();
}

cRdpMgrSvc::~cRdpMgrSvc()
{}
