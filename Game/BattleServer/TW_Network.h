#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TeNetMgr.h"
#include <map>

class TwNetMgr : public TeNetCall
{
public:
    TwNetMgr();
    ~TwNetMgr();
    void	UpdateNet();

    virtual void OnNetConnected(const HSock& rkSock, const char* pcIP, int iPort) override;
    virtual void OnNetAccept(const HSock& rkSock, int iListenPort, const char* pcIP, int iPort) override;
    virtual void OnNetRecv(const HSock& rkSock, void* pkData, int iSize) override;
    virtual void OnNetDisconnect(const HSock& rkSock, TeDisconnectCode eCode, int iParam) override;

    void	SendData(void* pkData, int iSize, const	HSock& rkSock);
    void	DisConnect(const	HSock& rkSock);

protected:
    TeNetMgr			m_kNetMgr;
};

extern TwNetMgr	kNet;
