#pragma once

#include "stdafx.h"

#include <QObject>
#include <winevt.h>

#include "Singleton.h"

class moduleDefence : public QObject
{
    Q_OBJECT

public:
    moduleDefence();
    ~moduleDefence();

    void                                             AbnormalDetectionThread();
    void                                             RemoteStateCheckThread();
    bool                                             IsRemoteState();

    DWORD PrintEvent( EVT_HANDLE hEvent );
    void                                             FilterEventLog();

    bool                                             ActiveRemoteConnection();
    bool                                             InActiveRemoteConnection();
    
private:
    std::thread                                      _thEventLog;
    std::thread                                      _thCheckRemoteState;

    bool                                             _isStop;
};

typedef Shared::Singletons::Singleton<moduleDefence> TyStDefence;
