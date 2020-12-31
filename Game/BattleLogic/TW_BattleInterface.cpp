/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_BattleInterface.h"
#include "TW_Main.h"

TwBattleInterface::TwBattleInterface()
{
    mpMain = nullptr;
}

TwBattleInterface::~TwBattleInterface()
{

}

bool TwBattleInterface::Initialize()
{
    return true;
}

void TwBattleInterface::Update()
{

}

void TwBattleInterface::SetPlayerInfo(const std::shared_ptr<TwPlayerInfo> playerInfo)
{
    mpMain->SetPlayerInfo(playerInfo);
}

bool TwBattleInterface::OnPlayerJion(std::uint64_t playerId)
{


    return true;
}

bool TwBattleInterface::OnPlayerLeave(std::uint64_t playerId)
{
    return true;
}

bool TwBattleInterface::OnReceiveCommand(std::uint64_t playerId, std::string command)
{
    return true;
}
