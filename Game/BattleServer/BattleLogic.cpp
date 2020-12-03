/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#pragma warning( disable: 4244)

#include "BattleLogic.h"
#include "TW_Main.h"
#include "GameCommand.pb.h"

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

void TwBattleLogic::OnRecivedCommand(std::string command)
{
    Game::TwGameCommand cmd;
    cmd.ParseFromString(command);

    switch (cmd.commandtype())
    {
    case Game::TwGameCommandType::CS_CONNECT: OnPlayerConnect(cmd.content()); break;
    default:
        break;
    }
}

void TwBattleLogic::OnPlayerConnect(std::string command)
{
    Game::TwGameConnectionCS cmd;
    cmd.ParseFromString(command);


}
