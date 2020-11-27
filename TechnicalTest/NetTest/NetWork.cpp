#include "NetWork.h"
#include "GameCommand.h"

SeNetMgr::SeNetMgr()
{

}

SeNetMgr::~SeNetMgr()
{

}

void	SeNetMgr::SendData(std::string Data, int iSize, const	HSock& rkSock)
{
    static void* sendBuffer = new char[1024];
    static	int	dwLastTime = 0;
    //int	dwNowTime = GetTickCount();
    //int	dwDeltaTime = dwNowTime - dwLastTime;
    //dwLastTime = dwNowTime;

    //printf("SendDataTime: %d     DeltaTime: %d \n", dwNowTime, dwDeltaTime);

    std::memset(sendBuffer, 0, 1024);
    std::memcpy(sendBuffer, Data.c_str(), iSize);
    m_kNetMgr.SendData(rkSock, sendBuffer, iSize);
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
    m_kSock = rkSock;

    // send connect command
    auto connectCmd = GameCommand::Connect();
    SendData(connectCmd.c_str(), connectCmd.size(), m_kSock);

    printf("Connect Ok!  Send Connect Command!!!  \n");
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
