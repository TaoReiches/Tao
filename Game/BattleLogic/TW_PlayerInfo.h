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
        Clear();
    }

    void Clear()
    {
        eType = TwPlayerType::PT_NULL;
        iUserID = 0;
        Name = "";
        iTypeID = 0;
    }

    TwPlayerType    eType;
    std::uint64_t   iUserID;
    std::string     Name;
    unsigned int    iTypeID;
};
