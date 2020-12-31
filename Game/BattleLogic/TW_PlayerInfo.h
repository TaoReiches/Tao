#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <string>

enum class TwPlayerType
{
    PT_NULL = 0,
    PT_OFFLINE_PLAYER,
    PT_ROBOT,
    PT_ONLINE_PLAYER,
};

class TwPlayerInfo
{
public:
    TwPlayerInfo()
    {
        Type = TwPlayerType::PT_NULL;
        UserID = 0;
        Name = "";
        TypeID = 0;
        Level = 0;
        PosX = 0;
        PosY = 0;
    }

    TwPlayerType    Type;
    std::uint64_t   UserID;
    std::string     Name;
    unsigned int    TypeID;
    int             Level;
    float           PosX;
    float           PosY;
};
