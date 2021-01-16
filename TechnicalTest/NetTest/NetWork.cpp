#include "NetWork.h"
#include "GameCommand.h"
#include "GameCommandExecute.h"

int GameCommand::PosX = 0;
int GameCommand::PosY = 0;

SeNetMgr::SeNetMgr()
{

}

SeNetMgr::~SeNetMgr()
{

}

void	SeNetMgr::SendData(std::string Data)
{
    static void* sendBuffer = new char[1024];
    static	int	dwLastTime = 0;

    std::memset(sendBuffer, 0, 1024);
    std::memcpy(sendBuffer, Data.c_str(), Data.size());
    m_kNetMgr.SendData(m_kSock, sendBuffer, Data.size());
}

void	SeNetMgr::DisConnect(const	HSock& rkSock)
{
    m_kNetMgr.Disconnect(rkSock);
}

void	SeNetMgr::UpdateNet()
{
    m_kNetMgr.Update();

    static int count = 0;
    ++count;
    if (count % 10 == 0)
    {
        SendData(GameCommand::Move());
    }
}

void SeNetMgr::OnNetConnected(const HSock& rkSock, const char* pcIP, int iPort)
{
    m_kSock = rkSock;

    // send connect command
    auto connectCmd = GameCommand::Connect();

    Game::TwGameCommand cmd;
    cmd.ParseFromString(connectCmd);

    Game::TwGameConnectionCS cmd1;
    cmd1.ParseFromString(cmd.content());

    SendData(connectCmd);

    printf("Connect Ok!  Send Connect Command!!!  \n");
}

void SeNetMgr::OnNetAccept(const HSock& rkSock, int iListenPort, const char* pcIP, int iPort)
{

}

void SeNetMgr::OnNetRecv(const HSock& rkSock, void* pkData, int iSize)
{
    char	acTemp[1024];
    memcpy(acTemp, pkData, iSize);
    acTemp[iSize] = '\0';
    std::string command(acTemp, iSize);

    GameCommandExecute::OnRecvConnect(command);
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
