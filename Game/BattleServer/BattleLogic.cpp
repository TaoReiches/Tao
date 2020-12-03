/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "BattleLogic.h"
#include "TW_Main.h"

BattleLogic::BattleLogic()
{
    mpMain = nullptr;
    muiFrame = 0;
}

BattleLogic::~BattleLogic()
{

}

bool BattleLogic::Initialize()
{
    mpMain = std::unique_ptr<TwMain>(new TwMain());
    mpMain->LoadRes(888);
    mpMain->Initialize();

    return true;
}

void BattleLogic::Update()
{
    ++muiFrame;
    mpMain->UpdateFrame(muiFrame);
}
