#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "Singleton.h"
#include <cstdint>
#include <TeNetMgr.h>
#include <map>

class TwUsers final : public TwSingleton<TwUsers>
{
public:
    TwUsers();
    ~TwUsers();

    void OnUserConnect(std::uint64_t userId, const HSock& sock);

    const HSock& GetSock(std::uint64_t userId)
    {
        return UserIdToSock[userId];
    }

private:
    std::map<std::uint64_t, HSock>      UserIdToSock;
    std::map<HSock, std::uint64_t>      SockToUserId;
};
