#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_LogicBase.h"
#include <unordered_map>
#include <string>
#include <memory>

class TwUnit;

class TwOutput final : public BeMainPtr
{
public:
    void Update();
    void AddPlayer(std::uint64_t playerId, std::shared_ptr<TwUnit> player)
    {
        AllPlayers.emplace(playerId, player);
    }

private:
    std::string GetUnitUpdateData(std::shared_ptr<TwUnit> player, bool addNew);

private:
    std::unordered_map<std::uint64_t, std::string> OutputCommands;
    std::unordered_map<std::uint64_t, std::shared_ptr<TwUnit>> AllPlayers;
};
