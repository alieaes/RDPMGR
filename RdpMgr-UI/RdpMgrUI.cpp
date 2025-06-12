#include "stdafx.h"
#include "RdpMgrUI.h"

#include "EXIPC.hpp"

RdpMgrUI::RdpMgrUI(QWidget *parent)
    : QMainWindow(parent)
{
    int64_t x = 0xF1F2F3F4F5F6F7F8;
    size_t t = sizeof( x );
    int64_t x2 = 0;
    Ext::IPC::SendIPC( L"RDPMGR", ( PVOID ) &x, t, ( void* ) &x2, t, 10, true );
    ui.setupUi( this );
}

RdpMgrUI::~RdpMgrUI()
{}
