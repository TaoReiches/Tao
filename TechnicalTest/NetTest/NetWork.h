#pragma once

#include "TeNetMgr.h"
#include <map>

class SeNetMgr : public TeNetCall
{
public:
    SeNetMgr();
    ~SeNetMgr();

    virtual void OnNetConnected(const HSock& rkSock, const char* pcIP, int iPort) override;
    virtual void OnNetAccept(const HSock& rkSock, int iListenPort, const char* pcIP, int iPort) override;
    virtual void OnNetRecv(const HSock& rkSock, void* pkData, int iSize) override;
    virtual void OnNetDisconnect(const HSock& rkSock, TeDisconnectCode eCode, int iParam) override;

    void	UpdateNet();
    void	SendData(void* pkData, int iSize, const	HSock& rkSock);
    void	DisConnect(const	HSock& rkSock);

    void	BeginGame();

private:
    TeNetMgr			m_kNetMgr;

    HSock				m_kServerSock;
};

extern SeNetMgr	kNet;
