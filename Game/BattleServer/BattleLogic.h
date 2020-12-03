#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include "Singleton.h"

class TwMain;

class TwBattleLogic : public TwSingleton<TwBattleLogic>
{
public:
    TwBattleLogic();
    ~TwBattleLogic();

    bool Initialize();
    void UpdateLogic();

private:
    std::unique_ptr<TwMain>     mpMain;
    unsigned int                muiFrame;
};
