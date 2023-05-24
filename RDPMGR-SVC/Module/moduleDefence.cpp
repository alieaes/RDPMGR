#include "stdafx.h"
#include "moduleDefence.h"

#include "SharedBase.h"
#include "Windows/SharedWinTools.h"

#pragma comment(lib, "wevtapi.lib")

#include <winevt.h>

#define ARRAY_SIZE 10
#define TIMEOUT 1000  // 1 second; Set and use in place of INFINITE in EvtNext call

moduleDefence::moduleDefence()
{
    _isStop = false;

    _thEventLog = std::thread( std::bind( &moduleDefence::AbnormalDetectionThread, this ) );
    _thCheckRemoteState = std::thread( std::bind( &moduleDefence::RemoteStateCheckThread, this ) );

    FilterEventLog();
}

moduleDefence::~moduleDefence()
{
    _isStop = true;

    if( _thCheckRemoteState.joinable() == true )
        _thCheckRemoteState.join();

    if( _thEventLog.joinable() == true )
        _thEventLog.join();
}

void moduleDefence::AbnormalDetectionThread()
{
    while( _isStop == false )
    {
        
    }
}

void moduleDefence::RemoteStateCheckThread()
{
    while( _isStop == false )
    {
        if( IsRemoteState() == false )
            InActiveRemoteConnection();

        Sleep( 3000 );
    }
}

bool moduleDefence::IsRemoteState()
{
    return GetSystemMetrics( SM_REMOTESESSION ) != 0;
}

DWORD moduleDefence::PrintEvent(EVT_HANDLE hEvent)
{
    DWORD status = ERROR_SUCCESS;
    DWORD dwBufferSize = 0;
    DWORD dwBufferUsed = 0;
    DWORD dwPropertyCount = 0;
    LPWSTR pRenderedContent = NULL;

    // The EvtRenderEventXml flag tells EvtRender to render the event as an XML string.
    if( !EvtRender( NULL, hEvent, EvtRenderEventXml, dwBufferSize, pRenderedContent, &dwBufferUsed, &dwPropertyCount ) )
    {
        if( ERROR_INSUFFICIENT_BUFFER == ( status = GetLastError() ) )
        {
            dwBufferSize = dwBufferUsed;
            pRenderedContent = ( LPWSTR )malloc( dwBufferSize );
            if( pRenderedContent )
            {
                EvtRender( NULL, hEvent, EvtRenderEventXml, dwBufferSize, pRenderedContent, &dwBufferUsed, &dwPropertyCount );
            }
            else
            {
                wprintf( L"malloc failed\n" );
                status = ERROR_OUTOFMEMORY;
                goto cleanup;
            }
        }

        if( ERROR_SUCCESS != ( status = GetLastError() ) )
        {
            wprintf( L"EvtRender failed with %d\n", GetLastError() );
            goto cleanup;
        }
    }

    wprintf( L"\n\n%s", pRenderedContent );

cleanup:

    if( pRenderedContent )
        free( pRenderedContent );

    return status;
}

void moduleDefence::FilterEventLog()
{
    HANDLE hEventLog = NULL;
    EVT_HANDLE hEvent = NULL;

    // 시작 시간과 종료 시간 설정
    SYSTEMTIME startTime;
    SYSTEMTIME endTime;
    memset( &startTime, 0, sizeof( startTime ) );
    memset( &endTime, 0, sizeof( endTime ) );
    startTime.wYear = 2023;
    startTime.wMonth = 5;
    startTime.wDay = 1;
    endTime.wYear = 2023;
    endTime.wMonth = 5;
    endTime.wDay = 10;

    // 시작 시간과 종료 시간을 FILETIME 형식으로 변환
    FILETIME ftStartTime;
    FILETIME ftEndTime;
    SystemTimeToFileTime( &startTime, &ftStartTime );
    SystemTimeToFileTime( &endTime, &ftEndTime );

    // 이벤트 XPATH 쿼리 생성
    WCHAR query[ 256 ];
    DWORD eventID = 1000;  // 이벤트 ID
    swprintf_s( query, L"*[System[TimeCreated[@SystemTime&gt;='%llu' and @SystemTime&lt;='%llu'] and EventID=%u]]",
                *( ULONGLONG* )&ftStartTime, *( ULONGLONG* )&ftEndTime, eventID );

    hEventLog = EvtOpenLog( NULL, L"Application", EvtOpenChannelPath );
    if( NULL == hEventLog )
    {
        wprintf( L"EvtOpenLog failed with %lu\n", GetLastError() );
        goto cleanup;
    }

    hEvent = EvtQuery( NULL, L"Application", query, EvtQueryChannelPath );
    if( NULL == hEvent )
    {
        wprintf( L"EvtQuery failed with %lu\n", GetLastError() );
        goto cleanup;
    }

    for( ;;)
    {
        DWORD dwBufferSize = 0;
        DWORD dwBufferUsed = 0;
        DWORD dwPropertyCount = 0;
        EVT_HANDLE hEvents[ ARRAY_SIZE ];

        if( !EvtNext( hEvent, 100, &hEvents[ 0 ], 1000, 0, &dwBufferUsed ) )
        {
            if( ERROR_NO_MORE_ITEMS != GetLastError() )
            {
                wprintf( L"EvtNext failed with %lu\n", GetLastError() );
            }
            goto cleanup;
        }

        for( DWORD i = 0; i < dwBufferUsed; i++ )
        {
            if( ERROR_SUCCESS == ( PrintEvent( hEvents[ i ] ) ) )
            {
                EvtClose( hEvents[ i ] );
                hEvents[ i ] = NULL;
            }
            else
            {
                goto cleanup;
            }
        }

        //EvtClearLog( hEventLog, NULL );
    }

cleanup:

    if( hEvent )
    {
        EvtClose( hEvent );
    }

    if( hEventLog )
    {
        EvtClose( hEventLog );
    }
}

bool moduleDefence::ActiveRemoteConnection()
{
    return Shared::Windows::SetRegDwordValue( HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Terminal Server", L"fDenyTSConnections", 0, true );
}

bool moduleDefence::InActiveRemoteConnection()
{
    return Shared::Windows::SetRegDwordValue( HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Terminal Server", L"fDenyTSConnections", 1, true );
}
