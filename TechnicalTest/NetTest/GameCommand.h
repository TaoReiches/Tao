#pragma once

#include "GameCommand.pb.h"

class GameCommand
{
public:
    static std::string Connect()
    {
        Game::TwGameCommand baseCmd;
        baseCmd.set_userid(888);
        baseCmd.set_commandtype(Game::TwGameCommandType::CS_CONNECT);
        Game::TwGameConnectionCS cmd;
        cmd.unsafe_arena_set_allocated_gamecommand(&baseCmd);

        return cmd.SerializeAsString();
    }
};
