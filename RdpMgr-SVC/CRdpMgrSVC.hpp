#ifndef __HDR_RDP_MGR_SVC__
#define __HDR_RDP_MGR_SVC__

#pragma once

#include "qtservice.h"

class CRdpMgrSVC : public QtService< QCoreApplication >
{
public:
    CRdpMgrSVC( int argc, char* argv[] );
    ~CRdpMgrSVC() override {}

protected:
    void                                    start() override;
    void                                    stop() override;
    void                                    pause() override;
    void                                    resume() override;
    void                                    processCommand( int code ) override;
};
#endif