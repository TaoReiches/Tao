#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

class TwBattleInterface final
{
public:
    TwBattleInterface();
    ~TwBattleInterface();

    bool Initialize();
    void Update();
    bool OnPlayerJion();
    bool OnPlayerLeave();
    bool OnReceiveCommand();
};
