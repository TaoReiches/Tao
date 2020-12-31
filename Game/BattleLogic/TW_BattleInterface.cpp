/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_BattleInterface.h"
#include "TW_Main.h"
#include "TW_UnitMgr.h"
#include "TW_PlayerInfo.h"
#include "TW_Unit.h"

TwBattleInterface::TwBattleInterface()
{
    Main = nullptr;
    Frame = 0;
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
    Main->UpdateFrame(Frame);
}

void TwBattleInterface::SetPlayerInfo(const std::shared_ptr<TwPlayerInfo> playerInfo)
{
    Main->SetPlayerInfo(playerInfo);
}

bool TwBattleInterface::OnPlayerJion(std::uint64_t playerId)
{
    // get player info
    const auto& playerInfo = Main->GetPlayerInfo(playerId);
    if (playerInfo == nullptr)
    {
        return false;
    }

    // add unit
    auto unit = Main->m_pkUnitMgr->AddUnit(playerInfo->TypeID, playerInfo->Level, playerInfo->UserID);
    if (unit == nullptr)
    {
        return false;
    }
    unit->SetPosition(playerInfo->PosX, playerInfo->PosY);

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
