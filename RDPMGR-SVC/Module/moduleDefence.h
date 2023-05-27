#pragma once

#include "stdafx.h"

#include <QObject>
#include <winevt.h>

#include "Singleton.h"
#include "String/SharedXString.h"

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

    SYSTEMTIME                                       getLocalTime();

    std::thread                                      _thEventLog;
    std::thread                                      _thCheckRemoteState;

    bool                                             _isStop;

    QDateTime                                        _dtLastCheck;

    std::map< XString, DWORD >                       _mapAbnormalIP;
};

typedef Shared::Singletons::Singleton<moduleDefence> TyStDefence;
