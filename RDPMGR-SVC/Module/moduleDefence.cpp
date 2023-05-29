#include "stdafx.h"
#include "moduleDefence.h"

#include "SharedBase.h"
#include "Windows/SharedWinTools.h"

#include <QtXml/QtXml>

#pragma comment(lib, "wevtapi.lib")

#include <winevt.h>

moduleDefence::moduleDefence()
{
    _isStop = false;
    _dtLastCheck = QDateTime::currentDateTime();

    _thEventLog = std::thread( std::bind( &moduleDefence::AbnormalDetectionThread, this ) );
    _thCheckRemoteState = std::thread( std::bind( &moduleDefence::RemoteStateCheckThread, this ) );

    _nAbnomalCount = 5;

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
        HANDLE hEventLog = NULL;
        EVT_HANDLE hEvent = NULL;

        // TEST 종료 후 제거
        _dtLastCheck = _dtLastCheck.addMonths( -1 );

        XString sDateTime = _dtLastCheck.toString( QString( "yyyy-MM-ddTHH:mm:ss.000Z" ) );

        // EventID는 추후 UI에서 받아서 추가할 수 있도록 함. 현재 목적은 brute-force attack을 막기위함이니 4625만 막아도 충분할듯함.
        DWORD dwEventID = 4625;

        // 이벤트 XPATH 쿼리 생성
        XString sQuery = Shared::Format::Format( "*[System[TimeCreated[@SystemTime > '{}'] and EventID={} ]]", sDateTime, dwEventID );

        do
        {
            hEventLog = EvtOpenLog( NULL, L"Security", EvtOpenChannelPath );

            if( hEventLog == NULL )
                break;

            hEvent = EvtQuery( NULL, L"Security", sQuery, EvtQueryChannelPath );

            if( hEvent == NULL )
                break;

            DWORD dwBufferUsed = 0;
            EVT_HANDLE hEvents[ 1024 ];

            if( EvtNext( hEvent, 100, &hEvents[ 0 ], 1000, 0, &dwBufferUsed ) == FALSE )
            {
                if( GetLastError() == ERROR_NO_MORE_ITEMS )
                    break;
            }

            for( int idx = 0; idx < dwBufferUsed; idx++ )
            {
                if( PrintEvent( hEvents[ idx ] ) == ERROR_SUCCESS )
                {
                    EvtClose( hEvents[ idx ] );
                    hEvents[ idx ] = NULL;
                }
            }

        } while( false );

        if( hEvent != NULL )
            EvtClose( hEvent );

        if( hEventLog != NULL )
            EvtClose( hEventLog );

        _dtLastCheck = QDateTime::currentDateTime();

        for( int idx = 0; idx < 100; idx++ )
        {
            if( _isStop == true )
                break;

            Sleep( 10 );
        }
    }
}

void moduleDefence::RemoteStateCheckThread()
{
    while( _isStop == false )
    {
        if( IsRemoteState() == false )
            InActiveRemoteConnection();

        for( int idx = 0; idx < 300; idx++ )
        {
            if( _isStop == true )
                break;

            Sleep( 10 );
        }
    }
}

bool moduleDefence::IsRemoteState()
{
    return GetSystemMetrics( SM_REMOTESESSION ) != 0;
}

DWORD moduleDefence::PrintEvent( EVT_HANDLE hEvent )
{
    DWORD status = ERROR_SUCCESS;
    DWORD dwBufferSize = 0;
    DWORD dwBufferUsed = 0;
    DWORD dwPropertyCount = 0;
    LPWSTR pRenderedContent = NULL;

    do
    {
        if( EvtRender( NULL, hEvent, EvtRenderEventXml, dwBufferSize, pRenderedContent, &dwBufferUsed, &dwPropertyCount ) == FALSE )
        {
            status = GetLastError();
            if( status == ERROR_INSUFFICIENT_BUFFER )
            {
                dwBufferSize = dwBufferUsed;
                pRenderedContent = ( LPWSTR )malloc( dwBufferSize );
                if( pRenderedContent )
                {
                    EvtRender( NULL, hEvent, EvtRenderEventXml, dwBufferSize, pRenderedContent, &dwBufferUsed, &dwPropertyCount );
                }
                else
                {
                    status = ERROR_OUTOFMEMORY;
                    break;
                }
            }

            if( status != ERROR_SUCCESS )
                break;
        }

    }
    while( false );

    XString s = pRenderedContent;
    QString sIpAddress;
    int nPID = -1;

    QXmlStreamReader xmlReader( s );

    while( !xmlReader.atEnd() && !xmlReader.hasError() )
    {
        if( xmlReader.readNextStartElement() )
        {
            if( xmlReader.name().toString().compare( "Data", Qt::CaseInsensitive ) == 0 )
            {
                QXmlStreamAttributes attributes = xmlReader.attributes();
                QString name = attributes.value( "Name" ).toString();

                if( name.compare( "IpAddress", Qt::CaseInsensitive ) == 0 )
                {
                    xmlReader.readNext();
                    sIpAddress = xmlReader.text().toString();
                    break;
                }

                // RDP로 연결한 경우 PID가 0으로 나옴, 확인 후에 0이 맞다면 같이 체크하도록 함.
                if( name.compare( "ProcessId", Qt::CaseInsensitive ) == 0 )
                {
                    xmlReader.readNext();
                    nPID = xmlReader.text().toInt();
                    break;
                }
            }
        }
    }

    // TODO : 값이 존재하고, 예외 IP가 아니라면 해당 IP의 접근 횟수를 확인해 일정 횟수 이상이면 방화벽 차단을 넣도록 함.

    do
    {
        if( sIpAddress.isEmpty() == true )
            break;

        // RDP 연결은 PID가 0으로 표기됨. 이 부분은 확인 후 주석 해제 필요
        // if( nPID != 0 )
        //     break;

        //if( sIpAddress.compare( "localhost", Qt::CaseInsensitive ) )

    }
    while (false);

    if( pRenderedContent != NULLPTR )
        free( pRenderedContent );

    return status;
}

void moduleDefence::FilterEventLog()
{
    HANDLE hEventLog = NULL;
    EVT_HANDLE hEvent = NULL;

#ifdef _DEBUG
    _dtLastCheck = _dtLastCheck.addMonths( -1 );
#endif

    XString sDateTime = _dtLastCheck.toString( QString( "yyyy-MM-ddTHH:mm:ss.000Z" ) );
    
    DWORD dwEventID = 4648;

    // 이벤트 XPATH 쿼리 생성
    XString sQuery = Shared::Format::Format( "*[System[TimeCreated[@SystemTime > '{}'] and EventID={} ]]", sDateTime.toWString(), dwEventID );

    do
    {
        hEventLog = EvtOpenLog( NULL, L"Security", EvtOpenChannelPath );

        if( hEventLog == NULL )
            break;

        hEvent = EvtQuery( NULL, L"Security", sQuery, EvtQueryChannelPath );

        if( hEvent == NULL )
            break;

        DWORD dwBufferUsed = 0;
        EVT_HANDLE hEvents[ 1024 ];

        if( EvtNext( hEvent, 100, &hEvents[ 0 ], 1000, 0, &dwBufferUsed ) == FALSE )
        {
            if( GetLastError() == ERROR_NO_MORE_ITEMS )
                continue;
        }

        for( int idx = 0; idx < dwBufferUsed; idx++ )
        {
            if( PrintEvent( hEvents[ idx ] ) == ERROR_SUCCESS )
            {
                EvtClose( hEvents[ idx ] );
                hEvents[ idx ] = NULL;
            }
        }
    }
    while( false );

    if( hEvent != NULL )
        EvtClose( hEvent );

    if( hEventLog != NULL )
        EvtClose( hEventLog );
}

bool moduleDefence::ActiveRemoteConnection()
{
    return Shared::Windows::SetRegDWORDValue( HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Terminal Server", L"fDenyTSConnections", 0, true );
}

bool moduleDefence::InActiveRemoteConnection()
{
    return Shared::Windows::SetRegDWORDValue( HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Terminal Server", L"fDenyTSConnections", 1, true );
}

SYSTEMTIME moduleDefence::getLocalTime()
{
    SYSTEMTIME ret;
    GetLocalTime( &ret );

    return ret;
}
