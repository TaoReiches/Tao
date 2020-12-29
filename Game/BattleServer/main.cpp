/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include <chrono>
#include <thread>
#include "TW_Network.h"
#include "BattleLogic.h"
#include "spdlog/spdlog.h"

int main()
{
    spdlog::info("Battle server start! int value: {0:d}; hex: {0:x}; oct: {0:o}; bin: {0:b}", 36);
    spdlog::warn("Test warning message. {1} {0}", "World", "Hello");
    spdlog::critical("Test error message. {:08.2f}", 1.23456789);

    TwBattleLogic::Get()->Initialize();

    unsigned int frame = 0;
    while (true)
    {
        TwNetwork::Get()->UpdateNet();
        TwBattleLogic::Get()->UpdateLogic();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
