#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include "Singleton.h"

class TwMain;

class BattleLogic : public Singleton<BattleLogic>
{
public:
    BattleLogic();
    ~BattleLogic();

    bool Initialize();
    void Update();

private:
    std::unique_ptr<TwMain>     mpMain;
    unsigned int                muiFrame;
};
