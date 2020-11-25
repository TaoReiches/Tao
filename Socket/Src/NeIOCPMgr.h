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

	bool Connect(const char* pcIP,int iPort,int iTimeOut,int iBufferSize,int iFlag);
	bool Listen(int iListenPort,int iListenNum,int iBufferSize,int iFlag);
	TeSendResult SendData(const NeHSock& rkSock,char* pcData,int iSize);
	void Disconnect(const NeHSock& rkSock);

protected:
	static DWORD WINAPI NetProcThread(LPVOID pkParam);
	DWORD NetProc(void);

	// 客户端使用
	NeSock* Connect(NeConnect* pkConnect);

	// 服务器端需要使用
	bool PostAccept(NeAcceptOV& rkOV);

	// 放入消息队列
	void PushConnected(const NeHSock& rkSock,BYTE abyIP[],int iPort);
	void PushAccept(const NeHSock& rkSock,int iAcceptPort,BYTE abyIP[],int iPort);
	void PushDisconnect(const NeHSock& rkSock,TeDisconnectCode eCode,int iParam = 0);
	void PushPost(NePostData& rkData);
	void PushRecv(const NeHSock& rkSock,char* pcData,int iSize);

	// 连接管理
	NeSock* CreateSock(SOCKET hSocket,NeAcceptOV* pkAcceptOV,NeConnect* pkConnect);
	void ReleaseSocket(SOCKET hSocket);

	// Sock池
	NeSock*	AllocSock(SOCKET hSocket,int iBufferSize,int iFlag);
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

	// 服务器端需要(使用AcceptEx)
	FAcceptEx		m_pkAcceptEx;
	FSockaddrs		m_pkSockaddrs;
	VListen			m_akListen;

	volatile LONG	m_lGenID;
	SeLock			m_kDelSockLock;
	LDelSock		m_kDelSocks;

	NeRecvBuffer	m_kRecvBuffer;
};
#endif