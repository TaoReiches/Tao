#pragma once

#include <string>
#include <iostream>
#include "GameCommand.pb.h"

class GameCommandExecute
{
public:
    static void OnRecvConnect(std::string command)
    {
        Game::TwGameCommand cmd;
        cmd.ParseFromString(command);

        std::cout << "Receive command: " << cmd.commandtype() << std::endl;

        switch (cmd.commandtype())
        {
        case Game::TwGameCommandType::SC_CONNECT: BeginLoading(cmd.content()); break;
        default:
            break;
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
    }
};
