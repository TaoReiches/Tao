#pragma once

#include "GameCommand.h"

class GameCommandExecute
{
public:
    static void OnRecvConnect(std::string command)
    {
        Game::TwGameCommandsSC baseCmd;
        baseCmd.ParseFromString(command);

        for (auto i = 0; i < baseCmd.commands_size(); ++i)
        {
            auto& cmd = baseCmd.commands(i);

            std::cout << "Receive command: " << cmd.commandtype() << std::endl;

            switch (cmd.commandtype())
            {
            case Game::TwGameCommandType::SC_CONNECT: BeginLoading(cmd.content()); break;
            case Game::TwGameCommandType::SC_UNITS_UPDATE: UnitUpdate(cmd.content()); break;
            default:
                break;
            }
        }
    }

private:
    static void BeginLoading(std::string command)
    {
        Game::TwGameConnectionSC gameConnectionSC;
        gameConnectionSC.ParseFromString(command);

        std::cout << "Begin Loading: mapid - " << gameConnectionSC.mapid() <<
            " unitTypeId - " << gameConnectionSC.herotypeid() <<
            " userId - " << gameConnectionSC.userid() << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        // send load end
        std::cout << "After 3 seconds, send load end!" << std::endl;
        Game::TwGameCommand loadendCmd;
        loadendCmd.set_commandtype(Game::TwGameCommandType::CS_LOADING_END);
        kNet.SendData(loadendCmd.SerializeAsString());
    }

    static void UnitUpdate(std::string command)
    {
        Game::TwGameUnitsUpdateSC gameUnitUpdateSC;
        gameUnitUpdateSC.ParseFromString(command);

        std::cout << "Recived unit update, all units :" << gameUnitUpdateSC.unitdatas_size() << std::endl;
        for (auto i = 0; i < gameUnitUpdateSC.unitdatas_size(); ++i)
        {
            auto& unitData = gameUnitUpdateSC.unitdatas(i);
            std::cout << "Player id : " << unitData.userid() << std::endl;
            if (unitData.has_hp())
            {
                std::cout << "HP : " << unitData.hp() << std::endl;
            }
            if (unitData.has_mp())
            {
                std::cout << "MP : " << unitData.mp() << std::endl;
            }
            if (unitData.has_posx())
            {
                GameCommand::PosX = unitData.posx();
                std::cout << "posX : " << unitData.posx() << std::endl;
            }
            if (unitData.has_posy())
            {
                GameCommand::PosY = unitData.posy();
                std::cout << "posY : " << unitData.posy() << std::endl;
            }
            if (unitData.has_targetposx())
            {
                std::cout << "target posX : " << unitData.targetposx() << std::endl;
            }
            if (unitData.has_targetposy())
            {
                std::cout << "target posY : " << unitData.targetposy() << std::endl;
            }
            if (unitData.has_unittypeid())
            {
                std::cout << "type id : " << unitData.unittypeid() << std::endl;
            }
        }
    }
};
