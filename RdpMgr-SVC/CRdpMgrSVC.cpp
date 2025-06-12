#include "stdafx.h"

#include "CRdpMgrSVC.hpp"

#include "EXIPC.hpp"

CRdpMgrSVC::CRdpMgrSVC( int argc, char* argv[] )
    : QtService<QCoreApplication>( argc, argv, "SERVICE" )
{
    Ext::IPC::CreateIPC( L"RDPMGR", nullptr, this );
}

void CRdpMgrSVC::start()
{
}

void CRdpMgrSVC::stop()
{
    QtService<QCoreApplication>::stop();
}

void CRdpMgrSVC::pause()
{
    QtService<QCoreApplication>::pause();
}

void CRdpMgrSVC::resume()
{
    QtService<QCoreApplication>::resume();
}

void CRdpMgrSVC::processCommand(int code)
{
    QtService<QCoreApplication>::processCommand(code);
}
