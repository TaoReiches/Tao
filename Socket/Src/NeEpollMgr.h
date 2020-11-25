#pragma once

#include "TeInclude.h"
#include "NeNetDef.h"
#include "NeBuffer.h"
#include "TeNetMgr.h"
#include "NeRecvBuffer.h"

#ifdef __LINUX__

void Printf(const char* files,const char* func,int iLine,const char *format,...);
#define PRINT(format,...)   Printf(__FILE__,__FUNCTION__,__LINE__, format,##__VA_ARGS__)

class NeEpollMgr
{
public:
	NeEpollMgr(void);
	~NeEpollMgr(void);

	bool Initialize(TeNetCall* pkNetCall,int iMaxThread);
	void Update(void);
	void Finialize(void);

	bool Connect(const char* pcIP,int iPort,int iTimeOut,int iBufferSize,int iFlag);
	bool Listen(int iListenPort,int iListenNum,int iBufferSize,int iFlag);
	TeSendResult SendData(const NeHSock& rkSock,char* pcData,int iSize);
	void Disconnect(const NeHSock& rkSock);


public:
	static void*  NetProcThread(void* pkParam);
	void NetProc();

	// 客户端使用
	NeSock* Connect(NeConnect* pkConnect);

public:
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

	int	 GetEpollFD(){ return m_iEpollFD;}
private:
	void AddSocket(SOCKET sFD);
	void RemoveSocket(SOCKET sFD);
    void PostEvent(SOCKET sFD,int iEvents);

    void Accept(SOCKET sFd,NeAcceptOV* pkOV);
    int  Write(SOCKET sFd,const void *data, int len);
    int  Read(SOCKET sFd,void *data, int len);

    int  ReadData(SOCKET sFd,NeBuffer& rkBuf);
    int  WriteData(SOCKET sFd,NeBuffer& rkBuf);
    void KeepAlive(SOCKET fd);
	void SetLinger(SOCKET fd);
private:
	TeNetCall*		m_pkNetCall;
	pthread_t		m_ahThread;

	SeLock			m_kSockLock;
	MSock			m_kAllSock;
	//NeSock*			m_kAllSock[SOCKET_HOLDER_SIZE];

	SeLock			m_kPostLock;
	QPostData		m_kPostData;

	int				m_iThreadNum;

	VListen			m_akListen;


	volatile DWORD	m_lGenID;
    volatile bool   m_bIsRun;
	volatile bool   m_bExit;
	SeLock			m_kDelSockLock;
	LDelSock		m_kDelSocks;

	NeRecvBuffer	m_kRecvBuffer;

	int				m_iEpollFD;
	struct epoll_event	m_kEvents[THREAD_EVENT_SIZE];
};
#endif