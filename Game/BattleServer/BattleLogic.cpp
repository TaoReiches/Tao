/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#pragma warning( disable: 4244)

#include "BattleLogic.h"
#include "TW_Main.h"
#include "GameCommand.pb.h"
#include "TW_Network.h"
#include "Tw_Users.h"

TwBattleLogic::TwBattleLogic()
{
    mpMain = nullptr;
    muiFrame = 0;
}

TwBattleLogic::~TwBattleLogic()
{

}

bool TwBattleLogic::Initialize()
{
    mpMain = std::unique_ptr<TwMain>(new TwMain());
    mpMain->LoadRes(888);
    mpMain->Initialize();

    return true;
}

void TwBattleLogic::UpdateLogic()
{
    ++muiFrame;
    mpMain->UpdateFrame(muiFrame);
}

// Player connect process:
// 1,CS_CONNECT client send token to battle server
// 2,battle server uses the token get user info from the world server
// 3,SC_CONNECT battle server send map info and user info to let the client begin loading
// 4,CS_LOADING_END client send loading end to the battle server
// 5,SC_FIRST_ENTER_MAP battle server send initial info to client to allow the user appear on the map
void TwBattleLogic::OnRecivedCommand(std::string command, const HSock& sock)
{
    Game::TwGameCommand cmd;
    cmd.ParseFromString(command);

    switch (cmd.commandtype())
    {
    case Game::TwGameCommandType::CS_CONNECT: OnPlayerConnect(cmd.content(), sock); break;
    default:
        break;
    }
}

void TwBattleLogic::OnPlayerConnect(std::string command, const HSock& sock)
{
    Game::TwGameConnectionCS cmd;
    cmd.ParseFromString(command);

    auto token = cmd.token();

    // http request to the world server to get user info
    auto userId = 9999999;
    TwUsers::Get()->OnUserConnect(userId, sock);

    // send back connect success and let client to start loading
    Game::TwGameConnectionSC gameConnectionSC;
    gameConnectionSC.set_herotypeid('U001');
    gameConnectionSC.set_mapid('M001');
    gameConnectionSC.set_userid(userId);

    Game::TwGameCommand sendCmd;
    sendCmd.set_commandtype(Game::TwGameCommandType::SC_CONNECT);
    sendCmd.set_content(gameConnectionSC.SerializeAsString());

    const auto sendString = sendCmd.SerializeAsString();
    static void* sendBuffer = new char[1024];
    std::memset(sendBuffer, 0, 1024);
    std::memcpy(sendBuffer, sendString.c_str(), sendString.size());
    TwNetwork::Get()->SendData(sendBuffer, sendString.size(), TwUsers::Get()->GetSock(userId));
}