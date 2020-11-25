#include "NeMulitTcpMgr.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------------------------------- Class NeMulitSock------------------------------
void NeMulitSock::OnConnect()
{
	if (m_pkNetMgr)
	{
		NePostData kPost;
		kPost.kMulitSock = NeHMulitSock(this, m_uiAllocID);
		kPost.eType = NPT_CONNECTED;
		memcpy(kPost.acIP, m_kConnect.acIP, sizeof(kPost.acIP));
		kPost.iPort = m_kConnect.iPort;
		m_pkNetMgr->PushPost(kPost);
	}
}
void NeMulitSock::OnConnectFailed()
{
	if (m_pkNetMgr)
	{
		NePostData kPost;
		kPost.kMulitSock = NeHMulitSock(NULL, 0);
		kPost.eType = NPT_CONNECTED;
		memcpy(kPost.acIP, m_kConnect.acIP, sizeof(kPost.acIP));
		kPost.iPort = m_kConnect.iPort;
		m_pkNetMgr->PushPost(kPost);
	}
}
void NeMulitSock::OnRawData(const char *buf, size_t len)
{
	// on recv Raw Data  need to process
	while (ibuf.GetLength() > 2)
	{
		int iNeedSize = 0;
		if (!ibuf.ReadButNotRemvoe((char*)&iNeedSize, 2))
		{
			return;
		}

		if (iNeedSize > (int)ibuf.GetLength())
		{
			return;
		}

		// 读取2直接大小
		ibuf.Read((char*)&iNeedSize, 2);

		// 先暂时扩大大小
		char acBuff[TCP_BUFSIZE_READ] = { 0 };
		if (!ibuf.Read(acBuff, iNeedSize))
		{
			return;
		}

		if (m_pkNetMgr)
		{			
			m_pkNetMgr->PushRecv(NeHMulitSock(this, m_uiAllocID), acBuff, iNeedSize);
		}
	}
}

void NeMulitSock::OnDelete()
{
// 	if (m_pkNetMgr)
// 	{		
// 		NePostData kPost;
// 		kPost.eType = NPT_DISCONNECT;
// 		kPost.kSock = NeHSock(this, m_uiAllocID);
// 		kPost.iCode = TDC_SELF_DISC;
// 		kPost.iParam = 0;
// 		m_pkNetMgr->PushPost(kPost);
// 	}
}

void NeMulitSock::OnDisconnect()
{
	if(m_pkNetMgr)
	{
		NePostData kPost;
		kPost.eType = NPT_DISCONNECT;
		kPost.kMulitSock = NeHMulitSock(this, m_uiAllocID);
		kPost.iCode = TDC_SELF_DISC;
		kPost.iParam = 0;
		m_pkNetMgr->PushPost(kPost);
	}	
}

unsigned int NeMulitSock::GetAllocID()
{
	return m_uiAllocID;
}

void NeMulitSock::SetConnectData(const char* pcIP, int iPort)
{
	if(pcIP)
	{
		memcpy(m_kConnect.acIP, pcIP, sizeof(m_kConnect.acIP));
	}
	
	m_kConnect.iPort = iPort;
}
// --------------------------------------- Class NeMulitSock------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------------------------------- Class NeTcpMgr------------------------------
NeMulitTcpMgr::NeMulitTcpMgr(void)
{
	m_lGenID = 4;
}

NeMulitTcpMgr::~NeMulitTcpMgr(void)
{

}

bool NeMulitTcpMgr::Initialize(TeNetCall* pkNetCall, int iMaxThread)
{
	m_pkNetCall = pkNetCall;

	SetNumberOfThreads(iMaxThread);

	return true;
}

void NeMulitTcpMgr::Update(void)
{
	while (true)
	{
		if (m_kPostData.empty())
		{
			break;
		}

		m_kPostMutex.Lock();
		NePostData kData = m_kPostData.front();
		m_kPostData.pop();
		m_kPostMutex.Unlock();

		switch (kData.eType)
		{
		case NPT_CONNECTED:
		{
			if (m_pkNetCall)
			{				
				m_pkNetCall->OnNetConnected(*((HSock *)&kData.kMulitSock), kData.acIP, kData.iPort);
			}
			break;
		}		
		case NPT_RECV:
		{
			char* pcData = m_kRecvBuffer.GetData(kData.iSize);
			if (m_pkNetCall && pcData)
			{
				m_pkNetCall->OnNetRecv(*((HSock *)&kData.kMulitSock), pcData, kData.iSize);
			}
			break;
		}
		case NPT_DISCONNECT:
		{
			if (m_pkNetCall)
			{
				m_pkNetCall->OnNetDisconnect(*((HSock *)&kData.kMulitSock), (TeDisconnectCode)kData.iCode, kData.iParam);
			}
			break;
		}
		case NPT_ACCEPTED:
		{
			// 服务器端 功能 暂时不支持
// 			if (m_pkNetCall)
// 			{
// 				char acIP[32];
// 				sprintf(acIP, "%d.%d.%d.%d", kData.abyIP[0], kData.abyIP[1], kData.abyIP[2], kData.abyIP[3]);
// 				m_pkNetCall->OnNetAccept(*((HSock *)&kData.kSock), kData.iAcceptPort, acIP, kData.iPort);
// 			}
//			break;
		}
		}
	}
}

void NeMulitTcpMgr::Finialize(void)
{
	// 清除接收缓冲
	m_kRecvBuffer.Clear();
}

bool NeMulitTcpMgr::Connect(const char* pcIP, int iPort, int iTimeOut, int iBufferSize, int iFlag)
{
	ISocketHandler& rkScoketHandler = GetRandomHandler();
	NeMulitSock* pkSock = new NeMulitSock(rkScoketHandler, this, m_lGenID++, iBufferSize);
	if(!pkSock)
	{
		return false;
	}

	// 设置连接的IP 端口
	pkSock->SetConnectData(pcIP, iPort);
	
	// 设置连接超时时间
	pkSock->SetConnectTimeout(iTimeOut);

	if(!pkSock->Open(pcIP, iPort))
	{
		delete pkSock;
		return false;
	}

	// 设置由handler自动销毁
	pkSock->SetDeleteByHandler(true);
	
	rkScoketHandler.Add(pkSock);
	
	return true;
}

bool NeMulitTcpMgr::Listen(int iListenPort, int iListenNum, int iBufferSize, int iFlag)
{
	return true;
}

TeSendResult NeMulitTcpMgr::SendData(const NeHMulitSock& rkSock, char* pcData, int iSize)
{
	if(rkSock.pkSock == NULL || rkSock.dwAllocID != rkSock.pkSock->GetAllocID())
	{
		return TSR_NET_CLOSED;
	}

	if(!rkSock.pkSock->Ready())
	{
		return TSR_NET_CLOSED;
	}

	char acSendBuf[MAX_SEND_SIZE] = { 0 };
	memcpy(acSendBuf, &iSize, 2);
	memcpy(acSendBuf + 2, pcData, iSize);

	rkSock.pkSock->SendBuf(acSendBuf, iSize + 2);

	return TSR_SUCCESS;
}

void NeMulitTcpMgr::Disconnect(const NeHMulitSock& rkSock)
{
	if (rkSock.dwAllocID != rkSock.pkSock->GetAllocID())
	{
		return;
	}

	rkSock.pkSock->Close();
	DeleteSocket(rkSock.pkSock);
}

void NeMulitTcpMgr::PushPost(NePostData& rkData)
{
	m_kPostMutex.Lock();
	m_kPostData.push(rkData);
	m_kPostMutex.Unlock();
}

void NeMulitTcpMgr::PushRecv(const NeHMulitSock& rkSock, char* pcData, int iSize)
{
	m_kPostMutex.Lock();

	// 该连接已经断开丢弃接收到的数据
	if (rkSock.dwAllocID == 0)
	{
		m_kPostMutex.Unlock();
		return;
	}

	int iPushSize = m_kRecvBuffer.PushData(pcData, iSize);
	if (iPushSize == iSize && iPushSize > 0)
	{
		NePostData kPost;
		kPost.eType = NPT_RECV;
		kPost.kMulitSock = rkSock;
		kPost.iSize = iPushSize;
		m_kPostData.push(kPost);
	}

	m_kPostMutex.Unlock();
}

// --------------------------------------- Class NeTcpMgr------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////