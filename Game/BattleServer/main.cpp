/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include <chrono>
#include <thread>
#include "TW_GameLog.h"
#include "TW_Network.h"
#include "BattleLogic.h"

TwNetMgr	kNet;

int main()
{
    TW_LOG("Battle server start!");
    TW_WARN("Test warning message.");
    TW_ERROR("Test error message.");

    TwBattleLogic::Get()->Initialize();

    unsigned int frame = 0;
    while (true)
    {
        kNet.UpdateNet();
        TwBattleLogic::Get()->Update();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
