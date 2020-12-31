#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <cstdint>
#include <string>
#include <memory>

class TwMain;

class TwBattleInterface final
{
public:
    TwBattleInterface();
    ~TwBattleInterface();

    bool Initialize();
    void Update();
    bool OnPlayerJion(std::uint64_t playerId);
    bool OnPlayerLeave(std::uint64_t playerId);
    bool OnReceiveCommand(std::uint64_t playerId, std::string command);

private:
    std::unique_ptr<TwMain>         mpMain;
};
