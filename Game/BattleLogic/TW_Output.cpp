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
            unitUpdateCommand.MergeFromString(GetUnitUpdateData(newUnit, true));
        }
    }
}

std::string TwOutput::GetUnitUpdateData(std::shared_ptr<TwUnit> player, bool addNew)
{
    Game::TwGameUnitData unitData;
    unitData.set_userid(player->GetPlayer());

    if (addNew || player->HasOutputFlag(1))
    {
        unitData.set_unittypeid(player->GetTypeID());
    }

    return unitData.SerializeAsString();
}
