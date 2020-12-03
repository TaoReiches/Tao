#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <map>
#include "TeNetMgr.h"
#include "Singleton.h"

class TwNetwork final : public TwSingleton<TwNetwork>, public TeNetCall
{
public:
    TwNetwork();
    ~TwNetwork();
    void	UpdateNet();

    void OnNetConnected(const HSock& rkSock, const char* pcIP, int iPort) override;
    void OnNetAccept(const HSock& rkSock, int iListenPort, const char* pcIP, int iPort) override;
    void OnNetRecv(const HSock& rkSock, void* pkData, int iSize) override;
    void OnNetDisconnect(const HSock& rkSock, TeDisconnectCode eCode, int iParam) override;

    void	SendData(void* pkData, int iSize, const	HSock& rkSock);
    void	DisConnect(const	HSock& rkSock);

protected:
    TeNetMgr			m_kNetMgr;
};
