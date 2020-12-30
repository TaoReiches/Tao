#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include <string>
#include "Singleton.h"
#include <TeNetMgr.h>

class TwMain;

class TwBattleLogic final : public TwSingleton<TwBattleLogic>
{
public:
    TwBattleLogic();
    ~TwBattleLogic();

    bool Initialize();
    void UpdateLogic();
    void OnRecivedCommand(std::string command, const HSock& sock);
    void OnPlayerDisconnect(std::uint64_t userId);

private:
    void OnPlayerConnect(std::string command, const HSock& sock);
    void OnPlayerLoadend(const HSock& sock);

private:
    std::unique_ptr<TwMain>     mpMain;
    unsigned int                muiFrame;
};
