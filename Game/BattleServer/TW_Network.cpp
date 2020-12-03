/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Network.h"
#include "BattleLogic.h"

TwNetMgr::TwNetMgr()
{
    m_kNetMgr.Initialize(this);

    m_kNetMgr.Listen(8899, 128, 64 * 1024, static_cast<TeSockFlag>(TeSockFlag::TSF_16BIT_WIN | TeSockFlag::TSF_NO_DELAY));
}

TwNetMgr::~TwNetMgr()
{

}

void	TwNetMgr::SendData(void* pkData, int iSize, const	HSock& rkSock)
{
    m_kNetMgr.SendData(rkSock, pkData, iSize);
}

void	TwNetMgr::DisConnect(const	HSock& rkSock)
{
    m_kNetMgr.Disconnect(rkSock);
}

void	TwNetMgr::UpdateNet()
{
    m_kNetMgr.Update();
}

void TwNetMgr::OnNetConnected(const HSock& rkSock, const char* pcIP, int iPort)
{
    int a = 0;
}
void TwNetMgr::OnNetAccept(const HSock& rkSock, int iListenPort, const char* pcIP, int iPort)
{

}
void TwNetMgr::OnNetRecv(const HSock& rkSock, void* pkData, int iSize)
{
    char	acTemp[1024];
    memcpy(acTemp, pkData, iSize);
    acTemp[iSize + 1] = '\0';

    TwBattleLogic::Get()->Initialize();
}
void TwNetMgr::OnNetDisconnect(const HSock& rkSock, TeDisconnectCode eCode, int iParam)
{

}
