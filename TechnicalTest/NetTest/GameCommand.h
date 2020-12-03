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
        send.set_content(cmd.SerializeAsString());

        out = send.SerializeAsString();
        return out;
    }
};
