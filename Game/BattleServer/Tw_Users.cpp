/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "Tw_Users.h"

TwUsers::TwUsers()
{

}

TwUsers::~TwUsers()
{

}

void TwUsers::OnUserConnect(std::uint64_t userId, const HSock& sock)
{
    UserIdToSock[userId] = sock;
    SockToUserId[sock] = userId;
}
