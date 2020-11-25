#pragma once
#include "TeNetMgr.h"
#include "NeNetDef.h"
#include "C++Sockets/TcpSocket.h"
#include "C++Sockets/SocketHandler.h"
#include "C++Sockets/StdoutLog.h"
#include "C++Sockets/socket_include.h"
#include <queue>
#include "NeRecvBuffer.h"

#ifdef _WIN32
#pragma warning(disable:4786)
#endif

#define MAX_SEND_SIZE	32 * 1024

class NeMulitTcpMgr;

class NeMulitSock : public TcpSocket
{
public:
	NeMulitSock(ISocketHandler& h, NeMulitTcpMgr* pkNetMgr, unsigned int iAllocID, int iBufferSize = 16 * 1024) : TcpSocket(h, iBufferSize, iBufferSize), m_pkNetMgr(pkNetMgr), m_uiAllocID(iAllocID)
	{
	}

	void OnConnect();
	void OnConnectFailed();
	void OnRawData(const char *buf, size_t len);
	// ONDELTE会有一个延迟 所有SOCKET关闭都会有调用
	void OnDelete();
	// ONDELTE会有一个延迟 所有SOCKET关闭都会有调用
	void OnDisconnect();
	unsigned int GetAllocID();

	void SetConnectData(const char* pcIP, int iPort);
protected:
	NeMulitTcpMgr*	m_pkNetMgr;
	NeConnect		m_kConnect;
	unsigned int	m_uiAllocID;
};

class NeMulitTcpMgr : public SocketHandler
{
	friend class NeMulitSock;
public:
	NeMulitTcpMgr(void);
	~NeMulitTcpMgr(void);

	bool Initialize(TeNetCall* pkNetCall,int iMaxThread = 2);
	void Update(void);
	void Finialize(void);

	bool Connect(const char* pcIP, int iPort, int iTimeOut, int iBufferSize, int iFlag);
	bool Listen(int iListenPort, int iListenNum, int iBufferSize, int iFlag);
	TeSendResult SendData(const NeHMulitSock& rkSock, char* pcData, int iSize);
	void Disconnect(const NeHMulitSock& rkSock);

protected:
	// 放入消息队列
	void PushPost(NePostData& rkData);
	void PushRecv(const NeHMulitSock& rkSock, char* pcData, int iSize);

protected:
	TeNetCall*							m_pkNetCall;
	StdoutLog							m_kLog;
	volatile long						m_lGenID;

	Mutex								m_kPostMutex;
	QPostData							m_kPostData;

	NeRecvBuffer						m_kRecvBuffer;
};