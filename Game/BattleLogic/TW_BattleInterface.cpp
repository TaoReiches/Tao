/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_BattleInterface.h"
#include "TW_Main.h"
#include "TW_UnitMgr.h"
#include "TW_PlayerInfo.h"
#include "TW_Unit.h"
#include "TW_Output.h"
#include "TW_Command.h"
#include "GameCommand.pb.h"

TwBattleInterface::TwBattleInterface()
{
    Main = nullptr;
    Frame = 0;
    Output = nullptr;
}

TwBattleInterface::~TwBattleInterface()
{
    Main->Finialize();
}

bool TwBattleInterface::Initialize()
{
    Main = std::shared_ptr<TwMain>(new TwMain());
    if (!Main->Initialize(888))
    {
        return false;
    }

    Output = std::unique_ptr<TwOutput>(new TwOutput());
    Output->AttachMain(Main);

    return true;
}

void TwBattleInterface::Update()
{
    ++Frame;
    Main->UpdateFrame(Frame);
    Output->Update();
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

    Output->AddPlayer(playerId, unit);

    // add testing units
    auto testUserId = 10000;
    for (auto i = 0; i < 32; ++i)
    {
        for (auto j = 0; j < 32; ++j)
        {
            ++testUserId;
            auto unitTest = Main->m_pkUnitMgr->AddUnit(playerInfo->TypeID, playerInfo->Level, testUserId);
            if (unitTest == nullptr)
            {
                return false;
            }
            unitTest->SetPosition(i * 1000, j * 1000);
            Output->AddPlayer(testUserId, unitTest);
        }
    }

    return true;
}

bool TwBattleInterface::OnPlayerLeave(std::uint64_t playerId)
{
    return true;
}

bool TwBattleInterface::OnPlayerMove(std::uint64_t playerId, std::string command)
{
    auto player = Output->GetPlayer(playerId);
    if (player == nullptr)
    {
        return false;
    }

    Game::TwGameUnitMoveCS moveCommand;
    moveCommand.ParseFromString(command);
    if (moveCommand.has_direction())
    {

    }
    else
    {
        auto posX = static_cast<float>(moveCommand.posx());
        auto posY = static_cast<float>(moveCommand.posy());

        TwCommand unitCommand(TwCommandType::BCT_MOVE, 0, posX, posY);
        player->GiveCommand(unitCommand, TwGiveCmdType::BCT_IMMEDIATE, true);
    }

    return true;
}

std::string TwBattleInterface::GetOutputCommand(std::uint64_t playerId)
{
    const auto& commands = Output->GetPlayerCommands(playerId);
    if (commands.size() == 0)
    {
        return "";
    }
    Game::TwGameCommandsSC outputCommand;
    for (auto& cmd : commands)
    {
        outputCommand.add_commands()->MergeFromString(cmd);
    }
    return outputCommand.SerializeAsString();
}
