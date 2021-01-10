#pragma once

#include "GameCommand.pb.h"
#include <iostream>

class GameCommand
{
public:
    static std::string Connect()
    {
        static std::string out;
        Game::TwGameConnectionCS cmd;
        cmd.set_token("fakeToken");

        Game::TwGameCommand send;
        const auto sendBytes = cmd.SerializeAsString();
        send.set_content(sendBytes);
        send.set_commandtype(Game::TwGameCommandType::CS_CONNECT);

        out = send.SerializeAsString();
        std::cout << "Send Connect Token: " << out << std::endl;
        return out;
    }

    static int PosX;
    static int PosY;
    static std::string Move()
    {
        static std::string out;
        Game::TwGameUnitMoveCS moveCmd;
        moveCmd.set_posx(PosX + 300);
        moveCmd.set_posy(PosY + 500);

        Game::TwGameCommand send;
        send.set_commandtype(Game::TwGameCommandType::CS_UNIT_MOVE);
        send.set_content(moveCmd.SerializeAsString());

        out = send.SerializeAsString();
        std::cout << "Send Move Command: " << out << std::endl;
        return out;
    }
};
