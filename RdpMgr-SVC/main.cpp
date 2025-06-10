#include "stdafx.h"

#include <QtCore/QCoreApplication>

#include "CRdpMgrSVC.hpp"

int main( int argc, char* argv[] )
{
    CRdpMgrSVC a( argc, argv );

    return a.exec();
}
