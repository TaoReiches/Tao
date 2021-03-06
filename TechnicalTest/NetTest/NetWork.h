#pragma once

#include "TeNetMgr.h"
#include <map>
#include <string>

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
    void	SendData(std::string Data);
    void	DisConnect(const	HSock& rkSock);

    void	BeginGame();

private:
    TeNetMgr			m_kNetMgr;

    HSock				m_kSock;
};

extern SeNetMgr	kNet;
