/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "BattleLogic.h"
#include "TW_Main.h"

TwBattleLogic::TwBattleLogic()
{
    mpMain = nullptr;
    muiFrame = 0;
}

TwBattleLogic::~TwBattleLogic()
{

}

bool TwBattleLogic::Initialize()
{
    mpMain = std::unique_ptr<TwMain>(new TwMain());
    mpMain->LoadRes(888);
    mpMain->Initialize();

    return true;
}

void TwBattleLogic::UpdateLogic()
{
    ++muiFrame;
    mpMain->UpdateFrame(muiFrame);
}
