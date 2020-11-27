#pragma once
#include "TeInclude.h"
#include "NeNetDef.h"
#include "NeBuffer.h"
#include "TeNetMgr.h"
#include "NeRecvBuffer.h"

#ifdef WIN32
class NeIOCPMgr
{
public:
	NeIOCPMgr(void);
	~NeIOCPMgr(void);

	bool Initialize(TeNetCall* pkNetCall,int iMaxThread);
	void Update(void);
	void Finialize(void);

	bool Connect(const char* pcIP,int iPort,int iTimeOut,int iBufferSize, TeSockFlag iFlag);
	bool Listen(int iListenPort,int iListenNum,int iBufferSize, TeSockFlag iFlag);
	TeSendResult SendData(const NeHSock& rkSock,char* pcData,int iSize);
	void Disconnect(const NeHSock& rkSock);

protected:
	static DWORD WINAPI NetProcThread(LPVOID pkParam);
	DWORD NetProc(void);

	NeSock* Connect(NeConnect* pkConnect);
	bool PostAccept(NeAcceptOV& rkOV);
	void PushConnected(const NeHSock& rkSock,BYTE abyIP[],int iPort);
	void PushAccept(const NeHSock& rkSock,int iAcceptPort,BYTE abyIP[],int iPort);
	void PushDisconnect(const NeHSock& rkSock,TeDisconnectCode eCode,int iParam = 0);
	void PushPost(NePostData& rkData);
	void PushRecv(const NeHSock& rkSock,char* pcData,int iSize);

	NeSock* CreateSock(SOCKET hSocket,NeAcceptOV* pkAcceptOV,NeConnect* pkConnect);
	void ReleaseSocket(SOCKET hSocket);

	NeSock*	AllocSock(SOCKET hSocket,int iBufferSize, TeSockFlag iFlag);
	void	FreeSock(NeSock* pkSock);

protected:
	HANDLE			m_hIOCP;
	TeNetCall*		m_pkNetCall;

	int				m_iThreadNum;
	HANDLE*			m_ahThread;

	SeLock			m_kSockLock;
	MSock			m_kAllSock;

	SeLock			m_kPostLock;
	QPostData		m_kPostData;

	FAcceptEx		m_pkAcceptEx;
	FSockaddrs		m_pkSockaddrs;
	VListen			m_akListen;

	volatile LONG	m_lGenID;
	SeLock			m_kDelSockLock;
	LDelSock		m_kDelSocks;

	NeRecvBuffer	m_kRecvBuffer;
};
#endif