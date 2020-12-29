/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "Tw_Users.h"

TwUsers::TwUsers()
{
    InvalidSock.uiAllocID = 0;
    InvalidUserId = 0;
}

TwUsers::~TwUsers()
{

}

void TwUsers::OnUserConnect(std::uint64_t userId, const HSock& sock)
{
    UserIdToSock[userId] = sock;
    SockToUserId[sock] = userId;
}

void TwUsers::OnUserDisconnect(std::uint64_t userId)
{
    const auto& sock = GetSock(userId);

    auto removeUserId = UserIdToSock.find(userId);
    if (removeUserId != UserIdToSock.end())
    {
        UserIdToSock.erase(removeUserId);
    }
    auto removeSock = SockToUserId.find(sock);
    if (removeSock != SockToUserId.end())
    {
        SockToUserId.erase(removeSock);
    }
}
