#pragma once
#include "Enet/include/enet/enet.h"
#include "C++Sockets/Thread.h"
#include "C++Sockets/Mutex.h"
#include "TeNetMgr.h"
#include "NeNetDef.h"
#include "NeRecvBuffer.h"
#include "NeSendBuffer.h"
#include <map>

#ifndef _WIN32
typedef unsigned char BYTE;
#endif

struct SeNetPeer
{
	SeNetPeer()
	{
		pkNetPeer = NULL;
	}

	Mutex			kSendMutex;
	NeBuffer		kSendBuf;	
	ENetPeer*		pkNetPeer;
};
typedef std::map<int, ENetPeer*> MapAllNetPeer;

class NeUdpMgr
{
public:
	NeUdpMgr(void);
	~NeUdpMgr(void);

	bool Initialize(TeNetCall* pkNetCall);
	void Update(void);
	void Finialize(void);

	bool Listen(int iPort,int iMaxCount,int iBufferSize);
	bool Connect(const char* pcIP, int iPort, int iTimeOut, int iBufferSize);
	bool SendData(int iConnect, char* pcData, int iSize,int iFlag);
	bool Disconnect(int iConnect);

protected:
	void ThreadUpdate();

	// 放入消息队列(FROM THREAD TO MAIN THREAD)
	void PushConnected(int iID, BYTE abyIP[], int iPort);
	void PushAccept(int iID, int iAcceptPort, BYTE abyIP[], int iPort);
	void PushDisconnect(int iID);
	void PushPost(NePostData& rkData);
	void PushRecv(int iID, char* pcData, int iSize);

	// 放入消息队列
	bool PushSend(int iID, char* pcData, int iSize,int iFlag);
	void PushActiveDisconnect(int iID);

	void AddPeer(int iID, ENetPeer* pkPeer);
	void DelPeer(int iID);

protected:
	TeNetCall*			m_pkNetCall;
	volatile long		m_lGenID;

	// 接收线程消息队列
	Mutex				m_kPostMutex;
	QPostData			m_kPostData;
	NeRecvBuffer		m_kRecvBuffer;

	// 发送队列
	Mutex				m_kSendMutex;
	QPostData			m_kSendData;
	NeRecvBuffer		m_kSendBuffer;

	// 线程使用变量
	ENetHost*			m_pkNetClient;
	ENetHost*			m_pkNetHost;
	MapAllNetPeer		m_kMapAllPeer;		// not thread safe	

	// multi thread
	class  NeUdpMgrThread : public Thread 
	{ 
	protected: 
		virtual void Run(); 
	public:
		NeUdpMgrThread()
		{
			m_pkData = NULL;
		}
		void SetVoid(void* pkData)
		{
			m_pkData = pkData; 
		}  
	private:  
		void* m_pkData; 
	};
	NeUdpMgrThread		m_kUpdateThread;
};