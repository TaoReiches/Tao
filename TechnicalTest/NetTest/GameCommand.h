#pragma once

#include "GameCommand.pb.h"

class GameCommand
{
public:
    static std::string Connect()
    {
        static std::string out;
        Game::TwGameCommand* baseCmd = new Game::TwGameCommand();
        baseCmd->set_userid(888);
        baseCmd->set_commandtype(Game::TwGameCommandType::CS_CONNECT);
        Game::TwGameConnectionCS cmd;
        cmd.unsafe_arena_set_allocated_gamecommand(baseCmd);

        out = cmd.SerializeAsString();
        return out;
    }
};
