#include "stdafx.h"
#include "cRdpMgrSvc.h"

#include "moduleDefence.h"
#include "Network/SharedNetwork.h"
#include "QT/SharedQtSwitch.h"

cRdpMgrSvc::cRdpMgrSvc( QWidget* parent )
    : QMainWindow( parent )
{
    ui.setupUi( this );
    setWindowFlags( Qt::FramelessWindowHint );

    QVBoxLayout mainLayout;
    ui.frmSwitch->setLayout( &mainLayout );

    _wdgMainSwitch = new Switch();
    mainLayout.addWidget( _wdgMainSwitch );

    // Switch의 기본 값을 지정해야함

    connect( _wdgMainSwitch, SIGNAL( toggled( bool ) ), this, SLOT( OnMainSwitchToggled( bool ) ) );
}

cRdpMgrSvc::~cRdpMgrSvc()
{
    
}

bool cRdpMgrSvc::ConnectValidation( bool isUseMsg /*= false*/ )
{
    bool isValidate = false;
    do
    {
        if( ui.edtConnectIP->text().isEmpty() == true || ui.edtConnectPORT->text().isEmpty() == true )
        {
            QMessageBox::critical( this, "ERROR", "IP/PORT를 입력해야 합니다." );
            break;
        }

        if( ui.edtConnectPORT->text().toInt() < 1 || ui.edtConnectPORT->text().toInt() > PORT_RANGE_MAX )
        {
            QMessageBox::critical( this, "ERROR", "PORT 범위가 유효하지 않습니다." );
            break;
        }

        if( Shared::Network::ValidateIP( ui.edtConnectIP->text().toStdString() ).empty() == true )
        {
            QMessageBox::critical( this, "ERROR", "IP가 유효하지 않습니다." );
            break;
        }

        isValidate = true;
    }
    while( false );

    return isValidate;
}

void cRdpMgrSvc::OnMainSwitchToggled( bool isChecked )
{
    if( isChecked == true )
    {
        toggleDefence( true );
    }
    else
    {
        toggleDefence( false );
    }
}

void cRdpMgrSvc::on_btnConnectTEST_clicked()
{
    Shared::Network::CNetworkMgr* networkMgr = TyStNetworkMgr::GetInstance();

    do
    {
        if( ConnectValidation( true ) == false )
            break;

        auto prConnection = networkMgr->GetConnection( "SERVER" );
        if( prConnection.first == false )
        {
            QMessageBox::critical( this, "ERROR", "연결 테스트는 적용 이후 가능합니다." );
            break;
        }

        tyStNetworkInfo network;
        network.sName = "TEST";
        network.eType = NETWORK_CLIENT;
        network.sIP = ui.edtConnectIP->text();
        network.sPort = ui.edtConnectPORT->text().toInt();

        bool isSuccess = networkMgr->NewConnection( network );

        if( isSuccess == false )
        {
            QMessageBox::critical( this, "ERROR", "연결 정보 확인이 필요합니다." );
            break;
        }

        auto prConnect = networkMgr->GetConnection( "TEST" );

        if( prConnect.first == false )
        {
            QMessageBox::critical( this, "ERROR", "내부 모듈 에러" );
            break;
        }

        if( prConnect.second->Connect() == false )
        {
            XString sErr = Shared::Format::Format( "연결 실패, 에러코드 : {}", GetLastError() );
            QMessageBox::critical( this, "ERROR", sErr );
            break;
        }

        QMessageBox::information( this, "SUCCESS", "연결 테스트 성공!" );
    }
    while( false );

    auto prConnect = networkMgr->GetConnection( "TEST" );

    if( prConnect.first == true )
        prConnect.second->DisConnect();
}

void cRdpMgrSvc::on_btnConnectApply_clicked()
{
    Shared::Network::CNetworkMgr* networkMgr = TyStNetworkMgr::GetInstance();

    do
    {
        if( ConnectValidation( true ) == false )
            break;

        tyStNetworkInfo network;
        network.sName = "SERVER";
        network.eType = NETWORK_SERVER;
        network.sIP = ui.edtConnectIP->text();
        network.sPort = ui.edtConnectPORT->text().toInt();

        bool isSuccess = networkMgr->NewConnection( network );

        if( isSuccess == false )
        {
            QMessageBox::critical( this, "ERROR", "연결 정보 확인이 필요합니다." );
            break;
        }

        auto prConnect = networkMgr->GetConnection( "SERVER" );

        if( prConnect.first == false )
        {
            QMessageBox::critical( this, "ERROR", "내부 모듈 에러" );
            break;
        }

        if( prConnect.second->Connect() == false )
        {
            XString sErr = Shared::Format::Format( "연결 실패, 에러코드 : {}", GetLastError() );
            QMessageBox::critical( this, "ERROR", sErr );
            break;
        }

        QMessageBox::information( this, "SUCCESS", "연결 성립!" );
    }
    while( false );
}

void cRdpMgrSvc::on_btnAbnormalApply_clicked()
{
}

void cRdpMgrSvc::toggleDefence( bool isUse )
{
    if( isUse == true )
    {
        TyStDefence::GetInstance();
    }
    else
    {
        TyStDefence::DestroyInstance();
    }
}
