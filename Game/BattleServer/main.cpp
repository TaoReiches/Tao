/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include <chrono>
#include <thread>
#include "TW_GameLog.h"
#include "TW_Main.h"
#include "TW_Network.h"

SeNetMgr	kNet;

int main()
{
    TW_LOG("Battle server start!");
    TW_WARN("Test warning message.");
    TW_ERROR("Test error message.");

    auto pkMain = std::unique_ptr<BeMain>(new BeMain());
    pkMain->LoadRes(888);
    pkMain->Initialize();

    unsigned int frame = 0;
    while (true)
    {
        kNet.UpdateNet();

        ++frame;
        pkMain->UpdateFrame(frame);

        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    getchar();
}
