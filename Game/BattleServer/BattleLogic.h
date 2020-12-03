#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include <string>
#include "Singleton.h"

class TwMain;

class TwBattleLogic : public TwSingleton<TwBattleLogic>
{
public:
    TwBattleLogic();
    ~TwBattleLogic();

    bool Initialize();
    void UpdateLogic();
    void OnRecivedCommand(std::string command);

private:
    void OnPlayerConnect(std::string command);

private:
    std::unique_ptr<TwMain>     mpMain;
    unsigned int                muiFrame;
};
