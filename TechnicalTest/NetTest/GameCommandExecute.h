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
    }
};
