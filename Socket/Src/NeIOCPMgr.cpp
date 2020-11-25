#include "NeIOCPMgr.h"
#ifdef WIN32

NeWinSock g_kWinSock;
void NetError(const char* pcFun,int iLine)
{
	char acString[1024];
	sprintf(acString,"%s[%d]\n",pcFun,iLine);
	OutputDebugString(acString);	
}

NeIOCPMgr::NeIOCPMgr(void)
{
	m_hIOCP = NULL;
	m_pkNetCall = NULL;

	m_iThreadNum = 0;
	m_ahThread = NULL;

	m_pkAcceptEx = NULL;
	m_pkSockaddrs = NULL;

	m_lGenID = NIK_ORIGIN_KEY + 1;
}

NeIOCPMgr::~NeIOCPMgr(void)
{
	Finialize();
}

bool NeIOCPMgr::Initialize(TeNetCall* pkNetCall,int iMaxThread)
{
	m_pkNetCall = pkNetCall;

	// 初始化完成端口
	if((m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0)) == NULL)
	{
		return false;
	}

	// 限制最大和最小线程数量
	if(iMaxThread < MIN_THREAD_NUM)
	{
		iMaxThread = MIN_THREAD_NUM;
	}
	else if(iMaxThread > MAX_THREAD_NUM)
	{
		iMaxThread = MAX_THREAD_NUM;
	}

	// 根据CPU核数决定最大线程
	SYSTEM_INFO kSysInfo;
	GetSystemInfo(&kSysInfo);
	m_iThreadNum = kSysInfo.dwNumberOfProcessors * 2;
	if(m_iThreadNum > iMaxThread)
	{
		m_iThreadNum = iMaxThread;
	}

	m_ahThread = new HANDLE[m_iThreadNum];
	memset(m_ahThread,0,sizeof(HANDLE) * m_iThreadNum);

	// 创建网络处理线程
	DWORD dwThreadID;
	for(int i = 0;i < m_iThreadNum;i++)
	{
		m_ahThread[i] = (HANDLE)CreateThread(NULL,0,NetProcThread,this,0,&dwThreadID);
	}

	return true;
}

void NeIOCPMgr::Update(void)
{
	while(true)
	{
		if(m_kPostData.empty())
		{
			break;
		}

		m_kPostLock.Lock();
		NePostData kData = m_kPostData.front();
		m_kPostData.pop();
		m_kPostLock.UnLock();

		switch(kData.eType)
		{
		case NPT_CONNECTED:
			{
				if(m_pkNetCall)
				{
					m_pkNetCall->OnNetConnected(*((HSock *)&kData.kSock),kData.acIP,kData.iPort);
				}
				break;
			}
		case NPT_ACCEPTED:
			{
				if(m_pkNetCall)
				{
					m_pkNetCall->OnNetAccept(*((HSock *)&kData.kSock),kData.iAcceptPort,kData.acIP,kData.iPort);
				}
				break;
			}
		case NPT_RECV:
			{
				char* pcData = m_kRecvBuffer.GetData(kData.iSize);
				if(m_pkNetCall && pcData)
				{
					m_pkNetCall->OnNetRecv(*((HSock *)&kData.kSock),pcData,kData.iSize);
				}
				break;
			}
		case NPT_DISCONNECT:
			{
				if(m_pkNetCall)
				{
					m_pkNetCall->OnNetDisconnect(*((HSock *)&kData.kSock),(TeDisconnectCode)kData.iCode,kData.iParam);
				}
				break;
			}
		}
	}
}

void NeIOCPMgr::Finialize(void)
{
	// 等待所有线程结束
	if(m_ahThread)
	{
		for(int i = 0;i < m_iThreadNum;i++)
		{
			PostQueuedCompletionStatus(m_hIOCP,0,NIK_ALL_EXIT,NULL);
		}
		WaitForMultipleObjects(m_iThreadNum,m_ahThread,true,WAIT_THREAD_TIME);
		SAFE_ARRAY_DELETE(m_ahThread);

		m_iThreadNum = 0;
		m_ahThread = NULL;
	}

	// 关闭监听socket
	m_pkAcceptEx = NULL;
	m_pkSockaddrs = NULL;
	for(int i = 0;i < (int)m_akListen.size();i++)
	{
		NeListen& rkListen = m_akListen[i];
		ReleaseSocket(rkListen.hSock);
		SAFE_ARRAY_DELETE(rkListen.apkAcceptOV);
	}
	m_akListen.clear();

	// 关闭所有socket连接
	m_kSockLock.Lock();
	for(MSock::iterator itr = m_kAllSock.begin();itr != m_kAllSock.end();++itr)
	{
		NeSock* pkSock = itr->second;
		ReleaseSocket(pkSock->hSocket);
		SAFE_DELETE(pkSock);
	}
	m_kAllSock.clear();
	m_kSockLock.UnLock();

	// 清除连接缓冲
	m_kDelSockLock.Lock();
	for(LDelSock::iterator itr = m_kDelSocks.begin();itr != m_kDelSocks.end();++itr)
	{
		NeDelSock& rkDelSock = *itr;
		SAFE_DELETE(rkDelSock.pkSock);
	}
	m_kDelSocks.clear();
	m_kDelSockLock.UnLock();

	// 清除接收缓冲
	m_kRecvBuffer.Clear();

	// 关闭完成端口
	CloseHandle(m_hIOCP);
	m_hIOCP = NULL;

	m_pkNetCall = NULL;
}

bool NeIOCPMgr::Connect(const char* pcIP,int iPort,int iTimeOut,int iBufferSize,int iFlag)
{
	if(!m_hIOCP || m_iThreadNum <= 0 || !m_ahThread || !pcIP || iPort < 0)
	{
		return false;
	}

	NeConnect* pkConnect = new NeConnect;
	memcpy(pkConnect->acIP, pcIP, sizeof(pkConnect->acIP));
	pkConnect->iPort = iPort;
	pkConnect->iTimeOut = iTimeOut;
	pkConnect->iBufferSize = iBufferSize;
	pkConnect->iFlag = iFlag;

	PostQueuedCompletionStatus(m_hIOCP,0,NIK_CONNECT_KEY,(OVERLAPPED *)pkConnect);
	return true;
}

bool NeIOCPMgr::Listen(int iListenPort,int iListenNum,int iBufferSize,int iFlag)
{
	if(iListenPort <= 0)
	{
		return false;
	}

	// 查看该端口是否已经在监听了
	for(int i = 0;i < (int)m_akListen.size();i++)
	{
		if(m_akListen[i].iPort == iListenPort)
		{
			return false;
		}
	}

	// 参数合法化
	if(iBufferSize < 8 * 1024)
	{
		iBufferSize = 8 * 1024;
	}
	if(iListenNum < 4)
	{
		iListenNum = 4;
	}

	// 创建监听网络
	WSADATA	kData;
	WSAStartup(MAKEWORD(2, 2), &kData);
	SOCKET hListenSock = (int)socket(PF_INET,SOCK_STREAM,IPPROTO_IP);
	if(hListenSock == INVALID_SOCKET)
	{
		return false;
	}

	////	设置tcp属性
	//int	optval = 1;                    
	//setsockopt(hListenSock, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));

	// 绑定端口
	SOCKADDR_IN kLocal;
	kLocal.sin_family = AF_INET;
	kLocal.sin_addr.s_addr = htonl(INADDR_ANY);
	kLocal.sin_port = htons(iListenPort);
	if(bind(hListenSock,(PSOCKADDR)&kLocal,sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		return false;
	}

	// 使用完成端口
	if(m_hIOCP != CreateIoCompletionPort((HANDLE)hListenSock,m_hIOCP,NIK_ACCEPT_KEY,0))
	{
		return false;
	}

	// 由于使用了AcceptEx，所以等待队列的长度可以设置的比较大一些
	if(listen(hListenSock,4) == SOCKET_ERROR)
	{
		return false;
	}

	// 取得AcceptEx和GetAcceptExSockaddrs函数指针
	DWORD dwbytes = 0;
	GUID kGuidAcceptEx = WSAID_ACCEPTEX;
	GUID kGuidSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
	WSAIoctl(hListenSock,SIO_GET_EXTENSION_FUNCTION_POINTER,&kGuidAcceptEx,sizeof(kGuidAcceptEx),&m_pkAcceptEx,sizeof(FAcceptEx),&dwbytes,NULL,NULL);
	WSAIoctl(hListenSock,SIO_GET_EXTENSION_FUNCTION_POINTER,&kGuidSockaddrs,sizeof(kGuidSockaddrs),&m_pkSockaddrs,sizeof(FSockaddrs),&dwbytes,NULL,NULL);

	NeListen kListen;
	kListen.hSock = hListenSock;
	kListen.iPort = iListenPort;
	kListen.iBufferSize = iBufferSize;
	kListen.iFlag = iFlag;
	kListen.iListenNum = iListenNum;

	// 提交监听给完成端口
	kListen.apkAcceptOV = new NeAcceptOV[iListenNum];
	for(int i = 0;i < iListenNum;i++)
	{
		NeAcceptOV& rkAcceptOV = kListen.apkAcceptOV[i];
		rkAcceptOV.hListenSock = hListenSock;
		rkAcceptOV.iPort = iListenPort;
		rkAcceptOV.iBufferSize = iBufferSize;
		rkAcceptOV.iFlag = iFlag;
		PostAccept(rkAcceptOV);
	}

	m_akListen.push_back(kListen);

	return true;
}

TeSendResult NeIOCPMgr::SendData(const NeHSock& rkSock,char* pcData,int iSize)
{
	// 判断是否已经关闭
	if(rkSock.dwAllocID != rkSock.pkSock->dwAllocID)
	{
		return TSR_NET_CLOSED;
	}

	// 确认网络句柄是有效的
	{
		bool bFind = false;
		m_kSockLock.Lock();
		MSock::iterator itr = m_kAllSock.find(rkSock.dwAllocID);
		if(itr != m_kAllSock.end() && rkSock.pkSock == itr->second)
		{
			bFind = true;
		}
		m_kSockLock.UnLock();
		if(!bFind)
		{
			return TSR_NET_CLOSED;
		}
	}

	SeLock& rkLock = rkSock.pkSock->kSendLock;
	NeBuffer& rkSendBuf = rkSock.pkSock->kSendBuf;
	NeSendBuffer& rkExtraBuf = rkSock.pkSock->kExtraBuf;

	bool bNeedSend = false;
	TeSendResult eRet = TSR_SUCCESS;

	rkLock.Lock();
	if(rkExtraBuf.GetData() == NULL)
	{
		if(rkSendBuf.GetSize() <= 0)
		{
			bNeedSend = true;
		}

		if(!rkSendBuf.PushData(pcData,iSize))
		{
			if(bNeedSend)
			{
				eRet = TSR_NET_BUSY;
			}
			else if(!rkExtraBuf.PushData(pcData,iSize))
			{
				eRet = TSR_NET_BUSY;
			}
		}
	}
	else
	{
		if(!rkExtraBuf.PushData(pcData,iSize))
		{
			eRet = TSR_NET_BUSY;
		}
	}

	if(bNeedSend)
	{
		NeOverlap& rkSendOV = rkSock.pkSock->kSendOV;
		rkSendOV.eType = NOV_SEND;
		rkSendOV.kWSABuf.buf = rkSendBuf.GetBuffer();
		rkSendOV.kWSABuf.len = rkSendBuf.GetSize();

		DWORD dwIOSize = 0;
		memset(&rkSendOV,0,sizeof(OVERLAPPED));
		if(WSASend(rkSock.pkSock->hSocket,&rkSendOV.kWSABuf,1,&dwIOSize,0,&rkSendOV,NULL) == SOCKET_ERROR)
		{
			int iSendError = WSAGetLastError();
			if(iSendError != ERROR_IO_PENDING)
			{
				PushDisconnect(rkSock,TDC_SEND_ERROR1,iSendError);
				eRet = TSR_NET_ERROR;
			}
		}
	}
	rkLock.UnLock();

	return eRet;
}

void NeIOCPMgr::Disconnect(const NeHSock& rkSock)
{
	PushDisconnect(rkSock,TDC_SELF_DISC,0);
}

DWORD WINAPI NeIOCPMgr::NetProcThread(LPVOID pkParam)
{
	NeIOCPMgr* pkThis = (NeIOCPMgr *)pkParam;
	return pkThis->NetProc();
}

DWORD NeIOCPMgr::NetProc(void)
{
	int	iLastError = 0;
	DWORD dwIOSize = 0;
	DWORD dwIOCPKey = NIK_ORIGIN_KEY;
	OVERLAPPED* pkOV = NULL;
	while(true)
	{
		iLastError = 0;
		dwIOSize = 0;
		pkOV = NULL;

		ULONG_PTR ulIOKey = NIK_ORIGIN_KEY;
		bool bRet = GetQueuedCompletionStatus(m_hIOCP,&dwIOSize,&ulIOKey,&pkOV,INFINITE);
		if(!bRet)
		{
			iLastError = GetLastError();
		}
		dwIOCPKey = (DWORD)ulIOKey;

		if(dwIOCPKey == NIK_ALL_EXIT || (!bRet && dwIOCPKey == NIK_ORIGIN_KEY))
		{
			NET_ERROR();
			return 0;
		}

		// Connect相关操作
		if(dwIOCPKey == NIK_CONNECT_KEY)
		{
			// 连接服务器
			NeConnect* pkConnect = (NeConnect *)pkOV;
			NeSock* pkSock = Connect(pkConnect);
			if(!pkSock)
			{
				PushConnected(NULL,(BYTE*)pkConnect->acIP,pkConnect->iPort);
			}
			delete pkConnect;
			continue;
		}

		// AcceptEx相关的操作
		if(dwIOCPKey == NIK_ACCEPT_KEY)
		{
			NeAcceptOV* pkAcceptOV = (NeAcceptOV *)pkOV;
			if(!bRet)
			{
				NET_ERROR();
				closesocket(pkAcceptOV->hSocket);
				continue;
			}

			NeSock* pkSock = CreateSock(pkAcceptOV->hSocket,pkAcceptOV,NULL);
			if(!pkSock)
			{
				NET_ERROR();
				closesocket(pkAcceptOV->hSocket);
				continue;
			}

			PostAccept(*pkAcceptOV);
			continue;
		}

		NeOverlap* pkOverlap = (NeOverlap *)pkOV;
		if(dwIOCPKey != pkOverlap->kSock.dwAllocID)
		{
			// 该套接字逻辑层已经关闭了
			continue;
		}

		// 关闭一个连接
		NeSock* pkSock = pkOverlap->kSock.pkSock;
		if(!bRet || dwIOSize == 0)
		{
			PushDisconnect(NeHSock(pkSock,dwIOCPKey),TDC_SYSTEM_ERROR,iLastError);
			continue;
		}

		if(pkOverlap->eType == NOV_RECV)
		{
			SeLock& rkLock = pkSock->kRecvLock;
			NeBuffer& rkRecvBuf = pkSock->kRecvBuf;

			// 检查接收之后的数据是否构成了一个完整包
			rkLock.Lock();
			if(rkRecvBuf.Append(dwIOSize))
			{
				int iPos = 0;
				NePack kPack;
				while(rkRecvBuf.GetPack(iPos,kPack))
				{
					PushRecv(pkOverlap->kSock,kPack.pcData,kPack.iSize);
					iPos += kPack.iAddSize;
				}
				if(iPos > 0)
				{
					rkRecvBuf.Retract(iPos);
				}
			}
			rkLock.UnLock();

			// 进行下一次接收
			DWORD dwFlag = 0;
			memset(pkOverlap,0,sizeof(OVERLAPPED));
			pkOverlap->kWSABuf.buf = rkRecvBuf.GetFreeBuffer();
			pkOverlap->kWSABuf.len = rkRecvBuf.GetFreeSize();
			if(WSARecv(pkSock->hSocket,&pkOverlap->kWSABuf,1,&dwIOSize,&dwFlag,pkOV,NULL) == SOCKET_ERROR)
			{
				int iRecvError = WSAGetLastError();
				if(iRecvError != ERROR_IO_PENDING)
				{
					PushDisconnect(NeHSock(pkSock,dwIOCPKey),TDC_RECV_ERROR2,iRecvError);
				}
			}
		}
		else if(pkOverlap->eType == NOV_SEND && pkSock->dwAllocID > 0)
		{
			NeBuffer& rkSendBuf = pkSock->kSendBuf;
			NeSendBuffer& rkExtraBuf = pkSock->kExtraBuf;

			bool bNeedSend = false;
			pkSock->kSendLock.Lock();
			rkSendBuf.Retract(dwIOSize);

			//////////////////////////////////////////////////////////////////////////
			// 假如额外的发送缓冲里面也有数据的话就把它放入正式的发送缓冲
			while(true)
			{
				NeSendPack* pkPack = rkExtraBuf.GetData();
				if(!pkPack)
				{
					break;
				}
				if(!rkSendBuf.PushData(pkPack->acData,pkPack->wSize))
				{
					break;
				}
				rkExtraBuf.PopData();
			}
			rkExtraBuf.Retract();
			//////////////////////////////////////////////////////////////////////////

			if(rkSendBuf.GetSize() > 0)
			{
				pkOverlap->kWSABuf.buf = rkSendBuf.GetBuffer();
				pkOverlap->kWSABuf.len = rkSendBuf.GetSize();
				bNeedSend = true;
			}
			pkSock->kSendLock.UnLock();

			if(bNeedSend)
			{
				memset(pkOverlap,0,sizeof(OVERLAPPED));
				if(WSASend(pkSock->hSocket,&pkOverlap->kWSABuf,1,&dwIOSize,0,pkOV,NULL) == SOCKET_ERROR)
				{
					int iSendError = WSAGetLastError();
					if(iSendError != ERROR_IO_PENDING)
					{
						PushDisconnect(NeHSock(pkSock,dwIOCPKey),TDC_SEND_ERROR2,iSendError);
					}
				}
			}
		}
	}

	return 1;
}

NeSock* NeIOCPMgr::Connect(NeConnect* pkConnect)
{
	// 创建Socket
	SOCKET hSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(hSocket == INVALID_SOCKET)
	{
		return NULL;
	}

	// 设置连接服务器
	sockaddr_in kSockAddr;
	memset(&kSockAddr,0,sizeof(kSockAddr));
	kSockAddr.sin_addr.s_addr =  inet_addr(pkConnect->acIP);
	kSockAddr.sin_family		= AF_INET;
	kSockAddr.sin_port			= htons(pkConnect->iPort);

	// 是否启用超时参数
	if(pkConnect->iTimeOut > 0)
	{
		// 修改该socket为非阻塞模式
		ULONG ulValue = 1;
		int iRet = ioctlsocket(hSocket,FIONBIO,&ulValue);
		if(iRet == SOCKET_ERROR)
		{
			closesocket(hSocket);
			return NULL;
		}

		// 连接服务器
		if(connect(hSocket,(SOCKADDR*)&kSockAddr,sizeof(kSockAddr)) == -1)
		{
			// 设置连接超时时间
			fd_set kSet;
			FD_ZERO(&kSet);
			FD_SET(hSocket,&kSet);
			timeval kTimeOut;
			kTimeOut.tv_sec  = pkConnect->iTimeOut / 1000;
			kTimeOut.tv_usec = pkConnect->iTimeOut % 1000;
			iRet = select((int)hSocket + 1,NULL,&kSet,NULL,&kTimeOut);
			if(iRet <= 0)
			{
				closesocket(hSocket);
				return NULL;
			}
			else
			{
				int iError = -1;
				int iLen = sizeof(int);
				getsockopt(hSocket,SOL_SOCKET,SO_ERROR,(char *)&iError,&iLen);
				if(iError != 0)
				{
					closesocket(hSocket);
					return NULL;
				}
			}
		}

		// 还原为阻塞模式
		ulValue = 0;
		iRet = ioctlsocket(hSocket,FIONBIO,&ulValue);
		if(iRet == SOCKET_ERROR)
		{
			closesocket(hSocket);
			return NULL;
		}
	}
	else
	{
		// 连接服务器
		if(connect(hSocket,(SOCKADDR*)&kSockAddr,sizeof(kSockAddr)) != 0)
		{
			closesocket(hSocket);
			return NULL;
		}
	}

	// 绑定该连接到完成端口
	NeSock* pkSock = CreateSock(hSocket,NULL,pkConnect);
	if(!pkSock)
	{
		closesocket(hSocket);
		return NULL;
	}

	return pkSock;
}

bool NeIOCPMgr::PostAccept(NeAcceptOV& rkOV)
{
	rkOV.hSocket = (int)WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if(rkOV.hSocket == INVALID_SOCKET)
	{
		NET_ERROR();
		return false;
	}
	memset(&rkOV,0,sizeof(OVERLAPPED));

	DWORD dwBytes = 0;
	if(!m_pkAcceptEx(rkOV.hListenSock,rkOV.hSocket,rkOV.acBuffer,0,NET_ADDR_LEN,NET_ADDR_LEN,&dwBytes,&rkOV))
	{
		if(WSA_IO_PENDING != WSAGetLastError())
		{
			NET_ERROR();
			return false;
		}
	}

	return true;
}

void NeIOCPMgr::PushConnected(const NeHSock& rkSock, BYTE abyIP[], int iPort)
{
	NePostData kPost;
	kPost.kSock = rkSock;
	kPost.eType = NPT_CONNECTED;
	memcpy(kPost.acIP, abyIP, sizeof(kPost.acIP));
	kPost.iPort = iPort;
	PushPost(kPost);
}

void NeIOCPMgr::PushAccept(const NeHSock& rkSock,int iAcceptPort,BYTE abyIP[],int iPort)
{
	NePostData kPost;
	kPost.kSock = rkSock;
	kPost.eType = NPT_ACCEPTED;
	kPost.iAcceptPort = iAcceptPort;
	sprintf(kPost.acIP, "%d.%d.%d.%d", abyIP[0], abyIP[1], abyIP[2], abyIP[3]);
	kPost.iPort = iPort;
	PushPost(kPost);
}

void NeIOCPMgr::PushDisconnect(const NeHSock& rkSock,TeDisconnectCode eCode,int iParam)
{
	bool bFind = false;
	NeSock* pkSock = NULL;

	m_kSockLock.Lock();
	MSock::iterator itr = m_kAllSock.find(rkSock.dwAllocID);
	if(itr != m_kAllSock.end())
	{
		bFind = true;
		pkSock = itr->second;
		m_kAllSock.erase(itr);
	}
	m_kSockLock.UnLock();

	if(bFind)
	{
		ReleaseSocket(rkSock.pkSock->hSocket);

		NeHSock kSock = rkSock;
		FreeSock(rkSock.pkSock);

		NePostData kPost;
		kPost.eType = NPT_DISCONNECT;
		kPost.kSock = kSock;
		kPost.iCode = eCode;
		kPost.iParam = iParam;
		PushPost(kPost);
	}
}

void NeIOCPMgr::PushPost(NePostData& rkData)
{
	m_kPostLock.Lock();
	m_kPostData.push(rkData);
	m_kPostLock.UnLock();
}

void NeIOCPMgr::PushRecv(const NeHSock& rkSock,char* pcData,int iSize)
{
	m_kPostLock.Lock();

	// 该连接已经断开丢弃接收到的数据
	if(rkSock.dwAllocID == 0)
	{
		m_kPostLock.UnLock();
		return;
	}

	int iPushSize = m_kRecvBuffer.PushData(pcData,iSize);
	if(iPushSize == iSize && iPushSize > 0)
	{
		NePostData kPost;
		kPost.eType = NPT_RECV;
		kPost.kSock = rkSock;
		kPost.iSize = iPushSize;
		m_kPostData.push(kPost);
	}

	m_kPostLock.UnLock();
}

NeSock* NeIOCPMgr::CreateSock(SOCKET hSocket,NeAcceptOV* pkAcceptOV,NeConnect* pkConnect)
{
	if(!pkAcceptOV && !pkConnect)
	{
		return NULL;
	}

	int iBufferSize = 0;
	int iFlag = 0;
	if(pkAcceptOV)
	{
		// 继承hListenSock属性
		setsockopt(hSocket,SOL_SOCKET,SO_UPDATE_ACCEPT_CONTEXT,(char *)&pkAcceptOV->hListenSock,sizeof(SOCKET));
		iBufferSize = pkAcceptOV->iBufferSize;
		iFlag = pkAcceptOV->iFlag;
	}
	else
	{
		iBufferSize = pkConnect->iBufferSize;
		iFlag = pkConnect->iFlag;
	}

	if(iFlag & TSF_NO_DELAY)
	{
		bool bNoDelay = true;
		setsockopt(hSocket,IPPROTO_TCP,TCP_NODELAY,(char*)&bNoDelay,sizeof(bNoDelay));
	}

	NeSock* pkSock = AllocSock(hSocket,iBufferSize,iFlag);
	if(!pkSock)
	{
		NET_ERROR();
		return NULL;
	}

	m_kSockLock.Lock();
	m_kAllSock[pkSock->dwAllocID] = pkSock;
	m_kSockLock.UnLock();

	if(pkAcceptOV)
	{
		SOCKADDR*	pkLocaladdr = NULL;
		SOCKADDR*	pkRemoteSockaddr = NULL;
		int			iLocaladdrLen = 0;
		int			iRemoteaddrLen = 0;

		m_pkSockaddrs(pkAcceptOV->acBuffer,0,NET_ADDR_LEN,NET_ADDR_LEN,&pkLocaladdr,&iLocaladdrLen,&pkRemoteSockaddr,&iRemoteaddrLen);

		BYTE abyIP[4];
		memcpy(abyIP,pkRemoteSockaddr->sa_data + 2,sizeof(abyIP));
		int iPort = ntohs(((sockaddr_in *)pkRemoteSockaddr)->sin_port);
		PushAccept(NeHSock(pkSock,pkSock->dwAllocID),pkAcceptOV->iPort,abyIP,iPort);
	}
	else
	{
		NeHSock kSock(pkSock,pkSock->dwAllocID);
		PushConnected(kSock,(BYTE*)pkConnect->acIP,pkConnect->iPort);
	}

	// 准备接收数据
	DWORD dwIOSize = 0,dwFlag = 0;
	NeOverlap& rkOV = pkSock->kRecvOV;
	memset(&rkOV,0,sizeof(OVERLAPPED));
	rkOV.eType = NOV_RECV;
	rkOV.kWSABuf.buf = pkSock->kRecvBuf.GetFreeBuffer();
	rkOV.kWSABuf.len = pkSock->kRecvBuf.GetFreeSize();
	if(WSARecv(hSocket,&rkOV.kWSABuf,1,&dwIOSize,&dwFlag,&rkOV,NULL) == SOCKET_ERROR)
	{
		int iRecvError = WSAGetLastError();
		if(iRecvError != ERROR_IO_PENDING)
		{
			PushDisconnect(NeHSock(pkSock,pkSock->dwAllocID),TDC_RECV_ERROR1,iRecvError);
		}
	}

	return pkSock;
}

void NeIOCPMgr::ReleaseSocket(SOCKET hSocket)
{
	CancelIo((HANDLE)hSocket);
	shutdown(hSocket,SD_BOTH);
	closesocket(hSocket);
}

NeSock*	NeIOCPMgr::AllocSock(SOCKET hSocket,int iBufferSize,int iFlag)
{
	// 确定socket是否可以绑定到完成端口
	DWORD dwAllocID = (DWORD)InterlockedIncrement(&m_lGenID);
	if(m_hIOCP != CreateIoCompletionPort((HANDLE)hSocket,m_hIOCP,dwAllocID,0))
	{
		NET_ERROR();
		return NULL;
	}

	// 查看缓冲池里面是否有空闲的自定义sock
	NeSock* pkSock = NULL;
	m_kDelSockLock.Lock();
	if(!m_kDelSocks.empty())
	{
		NeDelSock& rkDelSock = m_kDelSocks.front();
		pkSock = rkDelSock.pkSock;
		m_kDelSocks.pop_front();
	}
	m_kDelSockLock.UnLock();

	// 假如没有空闲的就分配一个
	if(!pkSock)
	{
		pkSock = new NeSock;
	}

	// 合法化发送和接受数据大小
	if(iBufferSize < MIN_BUFFER_SIZE)
	{
		iBufferSize = MIN_BUFFER_SIZE;
	}
	if(iBufferSize > MAX_BUFFER_SZIE)
	{
		iBufferSize = MAX_BUFFER_SZIE;
	}

	pkSock->hSocket = hSocket;
	pkSock->dwAllocID = dwAllocID;
	pkSock->kRecvBuf.InitBuffer(iBufferSize,iFlag);
	pkSock->kSendBuf.InitBuffer(iBufferSize,iFlag);
	pkSock->kRecvOV.kSock = NeHSock(pkSock,pkSock->dwAllocID);
	pkSock->kSendOV.kSock = NeHSock(pkSock,pkSock->dwAllocID);

	return pkSock;
}

void NeIOCPMgr::FreeSock(NeSock* pkSock)
{
	pkSock->hSocket = INVALID_SOCKET;
	pkSock->dwAllocID = 0;

	pkSock->kRecvLock.Lock();
	pkSock->kRecvBuf.ReleaseBuffer();
	pkSock->kRecvLock.UnLock();

	pkSock->kSendLock.Lock();
	pkSock->kSendBuf.ReleaseBuffer();
	pkSock->kExtraBuf.ReleaseBuffer();
	pkSock->kSendLock.UnLock();

	m_kDelSockLock.Lock();
	m_kDelSocks.push_back(NeDelSock(GetTickCount(),pkSock));
	m_kDelSockLock.UnLock();
}
#endif