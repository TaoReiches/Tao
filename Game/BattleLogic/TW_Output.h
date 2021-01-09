#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_LogicBase.h"
#include <unordered_map>
#include <map>
#include <string>
#include <memory>

class TwUnit;

class TwOutput final : public TwMainPtr
{
public:
    void Update();
    void AddPlayer(std::uint64_t playerId, std::shared_ptr<TwUnit> player)
    {
        AllPlayers.emplace(playerId, player);
    }
    std::shared_ptr<TwUnit> GetPlayer(std::uint64_t playerId)
    {
        auto iter = AllPlayers.find(playerId);
        if (iter != AllPlayers.end())
        {
            return iter->second;
        }
        return nullptr;
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
    std::string GetUnitUpdateData(std::shared_ptr<TwUnit> player, bool addNew);

private:
    std::unordered_map<std::uint64_t, std::vector<std::string>> OutputCommands;
    std::map<std::uint64_t, std::shared_ptr<TwUnit>> AllPlayers;
    std::vector<std::string> EmptyResult;
};
