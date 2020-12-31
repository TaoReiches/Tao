/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#pragma warning( disable: 4244)

#include "BattleLogic.h"
#include "TW_BattleInterface.h"
#include "GameCommand.pb.h"
#include "TW_Network.h"
#include "TW_Users.h"
#include "spdlog/spdlog.h"
#include "TW_PlayerInfo.h"

TwBattleLogic::TwBattleLogic()
{
    mpMain = nullptr;
}

TwBattleLogic::~TwBattleLogic()
{

}

bool TwBattleLogic::Initialize()
{
    mpMain = std::unique_ptr<TwBattleInterface>(new TwBattleInterface());
    mpMain->Initialize();

    return true;
}

void TwBattleLogic::UpdateLogic()
{
    mpMain->Update();
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
    spdlog::info("Received command: {}", cmd.commandtype());

    switch (cmd.commandtype())
    {
    case Game::TwGameCommandType::CS_CONNECT: OnPlayerConnect(cmd.content(), sock); break;
    case Game::TwGameCommandType::CS_LOADING_END: OnPlayerLoadend(sock); break;
    default:
        break;
    }
}

void TwBattleLogic::OnPlayerConnect(std::string command, const HSock& sock)
{
    Game::TwGameConnectionCS cmd;
    cmd.ParseFromString(command);

    auto token = cmd.token();
    spdlog::info("Received token: {}", token);

    // http request to the world server to get user info
    TwPlayerInfo playerInfo;
    playerInfo.Level = 3;
    playerInfo.UserID = 9999999;
    playerInfo.TypeID = 'U001';
    playerInfo.Name = "FirstPlayer";
    playerInfo.PosX = 888;
    playerInfo.PosY = 888;

    TwUsers::Get()->OnUserConnect(playerInfo.UserID, sock);
    mpMain->SetPlayerInfo(std::make_shared<TwPlayerInfo>(playerInfo));

    // send back connect success and let client to start loading
    Game::TwGameConnectionSC gameConnectionSC;
    gameConnectionSC.set_herotypeid(playerInfo.TypeID);
    gameConnectionSC.set_mapid('M001');
    gameConnectionSC.set_userid(playerInfo.UserID);

    Game::TwGameCommand sendCmd;
    sendCmd.set_commandtype(Game::TwGameCommandType::SC_CONNECT);
    sendCmd.set_content(gameConnectionSC.SerializeAsString());

    const auto sendString = sendCmd.SerializeAsString();
    static void* sendBuffer = new char[1024];
    std::memset(sendBuffer, 0, 1024);
    std::memcpy(sendBuffer, sendString.c_str(), sendString.size());
    TwNetwork::Get()->SendData(sendBuffer, sendString.size(), TwUsers::Get()->GetSock(playerInfo.UserID));
}

void TwBattleLogic::OnPlayerLoadend(const HSock& sock)
{
    // add the player into the battle
    auto playerId = TwUsers::Get()->GetUserId(sock);
    if (playerId == 0)
    {
        spdlog::critical("Can not find user id, sock id: {}", sock.uiAllocID);
        return;
    }

    spdlog::info("Player loading end, player id: {}", playerId);

    mpMain->OnPlayerJion(playerId);
}

void TwBattleLogic::OnPlayerDisconnect(std::uint64_t userId)
{
    mpMain->OnPlayerLeave(userId);
}
