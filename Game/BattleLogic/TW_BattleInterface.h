#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <cstdint>
#include <string>
#include <memory>

class TwMain;
class TwPlayerInfo;
class TwOutput;

class TwBattleInterface final
{
public:
    TwBattleInterface();
    ~TwBattleInterface();

    bool Initialize();
    void Update();
    void SetPlayerInfo(const std::shared_ptr<TwPlayerInfo> playerInfo);
    bool OnPlayerJion(std::uint64_t playerId);
    bool OnPlayerLeave(std::uint64_t playerId);
    bool OnReceiveCommand(std::uint64_t playerId, std::string command);

private:
    std::shared_ptr<TwMain>         Main;
    unsigned int                    Frame;
    std::unique_ptr<TwOutput>       Output;
};
