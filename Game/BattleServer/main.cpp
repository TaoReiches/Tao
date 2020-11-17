/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include <chrono>
#include <thread>
#include "TW_GameLog.h"
#include "TW_Main.h"

int main()
{
    TW_LOG("Battle server start!");
    TW_WARN("Test warning message.");
    TW_ERROR("Test error message.");

    auto pkMain = std::unique_ptr<BeMain>(new BeMain());
    pkMain->Initialize();

    while (true)
    {
        pkMain->UpdateFrame(1);

        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    getchar();
}
