#include "stdafx.h"

#include "CRdpMgrSVC.hpp"

CRdpMgrSVC::CRdpMgrSVC( int argc, char* argv[] )
    : QtService<QCoreApplication>( argc, argv, "SERVICE" )
{
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
