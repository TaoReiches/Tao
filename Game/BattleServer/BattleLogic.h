#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "Singleton.h"

class BattleLogic : public Singleton<BattleLogic>
{
public:
    BattleLogic();
    ~BattleLogic();

    bool Initialize();
};
