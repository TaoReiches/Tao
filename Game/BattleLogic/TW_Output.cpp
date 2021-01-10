/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Output.h"
#include "TW_Main.h"
#include "TW_UnitMgr.h"
#include "TW_Unit.h"
#include "GameCommand.pb.h"

void TwOutput::Update()
{
    OutputCommands.clear();

    // all units appear, remove and update commands
    for (auto& player : AllPlayers)
    {
        Game::TwGameUnitsUpdateSC unitUpdateCommand;
        // add new unit
        const auto& newUnits = player.second->GetOutputNewUnits();
        for (auto& newUnit : newUnits)
        {
            auto addCommand = unitUpdateCommand.add_unitdatas();
            addCommand->MergeFromString(GetUnitUpdateData(newUnit, true));
        }
        // unit update
        const auto& updateUnits = player.second->GetOutputUpdateUnits();
        for (auto& updateUnit : updateUnits)
        {
            unitUpdateCommand.add_unitdatas()->MergeFromString(GetUnitUpdateData(updateUnit, false));
        }
        // store command
        if (unitUpdateCommand.unitdatas_size() > 0)
        {
            Game::TwGameCommand gameCommand;
            gameCommand.set_commandtype(Game::TwGameCommandType::SC_UNITS_UPDATE);
            gameCommand.set_content(unitUpdateCommand.SerializeAsString());
            OutputCommands[player.first].push_back(gameCommand.SerializeAsString());
        }
        // remove units
        const auto& removeUnits = player.second->GetOutputRemoveUnits();
        if (removeUnits.size() > 0)
        {
            Game::TwGameUnitsRemoveSC removeUnitsCommand;
            for (auto& removeUnit : removeUnits)
            {
                removeUnitsCommand.add_userids(player.second->GetPlayer());
            }
            Game::TwGameCommand gameCommand;
            gameCommand.set_commandtype(Game::TwGameCommandType::SC_UNITS_REMOVE);
            gameCommand.set_content(removeUnitsCommand.SerializeAsString());
            OutputCommands[player.first].push_back(gameCommand.SerializeAsString());
        }
        // clear unit output data
        player.second->UpdateOutputDataFinished();
        player.second->ClearOutputData();
    }
}

std::string TwOutput::GetUnitUpdateData(std::shared_ptr<TwUnit> player, bool addNew)
{
    Game::TwGameUnitData unitData;
    unitData.set_userid(player->GetPlayer());

    if (addNew || player->HasOutputFlag(TwUnitOutputFlag::BSUDCF_TYPEID))
    {
        unitData.set_unittypeid(player->GetTypeID());
    }
    if (addNew || player->HasOutputFlag(TwUnitOutputFlag::BSUDCF_CURRENT_POS))
    {
        unitData.set_posx(static_cast<int>(player->GetPosX()));
        unitData.set_posy(static_cast<int>(player->GetPosY()));
    }
    if (addNew || player->HasOutputFlag(TwUnitOutputFlag::BSUDCF_TARGET_POS))
    {
        unitData.set_targetposx(static_cast<int>(player->GetTarPosX()));
        unitData.set_targetposy(static_cast<int>(player->GetTarPosY()));
    }
    if (addNew || player->HasOutputFlag(TwUnitOutputFlag::BSUDCF_CURRENT_HP))
    {
        unitData.set_hp(static_cast<int>(player->GetHP()));
    }
    if (addNew || player->HasOutputFlag(TwUnitOutputFlag::BSUDCF_CURRENT_MP))
    {
        unitData.set_mp(static_cast<int>(player->GetMP()));
    }

    return unitData.SerializeAsString();
}
