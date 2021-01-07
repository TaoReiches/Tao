#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_LogicBase.h"
#include <unordered_map>
#include <string>
#include <memory>
#include "GameCommand.pb.h"

class TwUnit;

class TwOutput final : public TwMainPtr
{
public:
    void Update();
    void AddPlayer(std::uint64_t playerId, std::shared_ptr<TwUnit> player)
    {
        AllPlayers.emplace(playerId, player);
    }
    const std::vector<std::string >& GetPlayerCommands(std::uint64_t playerId)
    {
        const auto& iter = OutputCommands.find(playerId);
        if (iter != OutputCommands.end())
        {
            return iter->second;
        }
        return EmptyResult;
    }

private:
    Game::TwGameUnitData GetUnitUpdateData(std::shared_ptr<TwUnit> player, bool addNew);

private:
    std::unordered_map<std::uint64_t, std::vector<std::string>> OutputCommands;
    std::unordered_map<std::uint64_t, std::shared_ptr<TwUnit>> AllPlayers;
    std::vector<std::string> EmptyResult;
};
