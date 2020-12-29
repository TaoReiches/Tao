/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Network.h"
#include "BattleLogic.h"
#include "Tw_Users.h"

TwNetwork::TwNetwork()
{
    m_kNetMgr.Initialize(this);

    m_kNetMgr.Listen(8899, 128, 64 * 1024, static_cast<TeSockFlag>(TeSockFlag::TSF_16BIT_WIN | TeSockFlag::TSF_NO_DELAY));
}

TwNetwork::~TwNetwork()
{

}

void	TwNetwork::SendData(void* pkData, int iSize, const	HSock& rkSock)
{
    m_kNetMgr.SendData(rkSock, pkData, iSize);
}

void	TwNetwork::DisConnect(const	HSock& rkSock)
{
    m_kNetMgr.Disconnect(rkSock);
}

void	TwNetwork::UpdateNet()
{
    m_kNetMgr.Update();
}

void TwNetwork::OnNetConnected(const HSock& rkSock, const char* pcIP, int iPort)
{
    int a = 0;
}
void TwNetwork::OnNetAccept(const HSock& rkSock, int iListenPort, const char* pcIP, int iPort)
{

}
void TwNetwork::OnNetRecv(const HSock& rkSock, void* pkData, int iSize)
{
    char	acTemp[1024];
    memcpy(acTemp, pkData, iSize);
    acTemp[iSize] = '\0';

    TwBattleLogic::Get()->OnRecivedCommand(acTemp, rkSock);
}
void TwNetwork::OnNetDisconnect(const HSock& rkSock, TeDisconnectCode eCode, int iParam)
{
    auto userId = TwUsers::Get()->GetUserId(rkSock);

    TwBattleLogic::Get()->OnPlayerDisconnect(userId);
    TwUsers::Get()->OnUserDisconnect(userId);
}
