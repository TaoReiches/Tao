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
    void OnUserDisconnect(std::uint64_t userId);

    const HSock& GetSock(std::uint64_t userId) const
    {
        const auto& result = UserIdToSock.find(userId);
        if (result != UserIdToSock.end())
        {
            return result->second;
        }
        return InvalidSock;
    }
    const std::uint64_t& GetUserId(const HSock& sock) const
    {
        const auto& result = SockToUserId.find(sock);
        if (result != SockToUserId.end())
        {
            return result->second;
        }
        return InvalidUserId;
    }
    const std::map<std::uint64_t, HSock>& GetAllUsers() const
    {
        return UserIdToSock;
    }

private:
    std::map<std::uint64_t, HSock>      UserIdToSock;
    std::map<HSock, std::uint64_t>      SockToUserId;

    HSock                               InvalidSock;
    std::uint64_t                       InvalidUserId;
};
