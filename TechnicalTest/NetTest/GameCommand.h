#pragma once

#include "GameCommand.pb.h"

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
        return out;
    }
};
