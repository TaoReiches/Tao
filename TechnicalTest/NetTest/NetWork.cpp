#include "NetWork.h"

SeNetMgr::SeNetMgr()
{

}

SeNetMgr::~SeNetMgr()
{

}

void	SeNetMgr::SendData(void* pkData, int iSize, const	HSock& rkSock)
{
    static	int	dwLastTime = 0;
    //int	dwNowTime = GetTickCount();
    //int	dwDeltaTime = dwNowTime - dwLastTime;
    //dwLastTime = dwNowTime;

    //printf("SendDataTime: %d     DeltaTime: %d \n", dwNowTime, dwDeltaTime);

    m_kNetMgr.SendData(rkSock, pkData, iSize);
}

void	SeNetMgr::DisConnect(const	HSock& rkSock)
{
    m_kNetMgr.Disconnect(rkSock);
}

void	SeNetMgr::UpdateNet()
{
    m_kNetMgr.Update();
}


void SeNetMgr::OnNetConnected(const HSock& rkSock, const char* pcIP, int iPort)
{
    m_kServerSock = rkSock;



    //SendData(acSendBuffer, 16, m_kServerSock);

    printf("Connect Ok!  Send MapInfo!!!  \n");

    //Sleep(500);
}
void SeNetMgr::OnNetAccept(const HSock& rkSock, int iListenPort, const char* pcIP, int iPort)
{

}
void SeNetMgr::OnNetRecv(const HSock& rkSock, void* pkData, int iSize)
{

}
void SeNetMgr::OnNetDisconnect(const HSock& rkSock, TeDisconnectCode eCode, int iParam)
{

}

void	SeNetMgr::BeginGame()
{
    m_kNetMgr.Initialize(this);
    if (m_kNetMgr.Connect("127.0.0.1", 8899))
    {
        printf("Send Connect Handle Ok!!!!  \n");
    }
}
