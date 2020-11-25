#include "NeEpollMgr.h"

#ifdef __LINUX__
class IgnSigpipe
{
public:
	IgnSigpipe()
	{
		signal(SIGPIPE,SIG_IGN);
	}
};
IgnSigpipe gIgnSigpipe;

NeEpollMgr::NeEpollMgr(void)
{
    m_bIsRun = true;
	m_pkNetCall = NULL;
	m_lGenID = 1;
	m_iThreadNum = 0;
	m_bExit = false;
}

NeEpollMgr::~NeEpollMgr(void)
{
	Finialize();
}



bool NeEpollMgr::Initialize(TeNetCall* pkNetCall,int iMaxThread)
{
	m_pkNetCall = pkNetCall;

	m_iEpollFD = epoll_create(SOCKET_HOLDER_SIZE);
	if(m_iEpollFD == -1)
	{
		//PRINT("Could not create epoll fd (/dev/epoll).");
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
	m_iThreadNum = iMaxThread;
	pthread_attr_t type;
	pthread_attr_init(&type);
	pthread_attr_setdetachstate(&type, PTHREAD_CREATE_JOINABLE);
	pthread_create(&m_ahThread,&type,NetProcThread,this);
    pthread_attr_destroy(&type);
	return true;
}

void NeEpollMgr::Update(void)
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


void NeEpollMgr::Finialize(void)
{
    m_bIsRun = false;
	while(true)
	{
		if(m_bExit)
		{
			break;
		}
	}
    void* status;
    pthread_join(m_ahThread,&status);

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
    close(m_iEpollFD);
	m_pkNetCall = NULL;
}

bool NeEpollMgr::Connect(const char* pcIP,int iPort,int iTimeOut,int iBufferSize,int iFlag)
{
	if( m_iThreadNum <= 0 || !m_ahThread || !pcIP || iPort < 0)
	{
		//PRINT("m_iThreadNum <= 0 || !m_ahThread || !pcIP || iPort < 0.\n");
		return false;
	}

	NeConnect* pkConnect = new NeConnect;
	memcpy(pkConnect->acIP, pcIP, sizeof(pkConnect->acIP));
	pkConnect->iPort = iPort;
	pkConnect->iTimeOut = iTimeOut;
	pkConnect->iBufferSize = iBufferSize;
	pkConnect->iFlag = iFlag;

    if(!Connect(pkConnect))
    {
		NeHSock kSock;
		PushConnected(kSock,(BYTE*)pkConnect->acIP,pkConnect->iPort);
        return false;
    }
	return true;
}


NeSock* NeEpollMgr::Connect(NeConnect* pkConnect)
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
	kSockAddr.sin_addr.s_addr = inet_addr(pkConnect->acIP);
	kSockAddr.sin_family		= AF_INET;
	kSockAddr.sin_port			= htons(pkConnect->iPort);

	// 是否启用超时参数
	if(pkConnect->iTimeOut > 0)
	{
		ulong ulValue = 1;
		int iRet = ::ioctl(hSocket,FIONBIO,&ulValue);
		if(iRet == SOCKET_ERROR)
		{
			close(hSocket);
			return NULL;
		}
		// 连接服务器
		iRet = connect(hSocket,(const sockaddr*)&kSockAddr,sizeof(kSockAddr));
		if(iRet < 0 && errno == EINPROGRESS)
		{
			// 等待连接完成　errno == EINPROGRESS 表示正在建立连接
			int iTimeOut = pkConnect->iTimeOut;
			struct pollfd kFd;
			kFd.fd = hSocket;
			kFd.events = POLLOUT | POLLERR | POLLHUP | POLLNVAL;
			int iRet = poll(&kFd,1,iTimeOut);
			if(iRet <= 0)
			{
				close(hSocket);
				return NULL;
			}
			if(kFd.revents & (POLLERR | POLLHUP | POLLNVAL))
			{
				close(hSocket);
				return NULL;
			}
			int iError = 0;
			socklen_t iLen = sizeof(int);
			iRet = getsockopt(hSocket,SOL_SOCKET,SO_ERROR,reinterpret_cast<char*>(&iError),&iLen);
			if(iRet < 0 || iError != 0)
			{
				close(hSocket);
				return NULL;
			}

		}else
		{
			close(hSocket);
			return NULL;
		}

	}
	else
	{
		close(hSocket);
		return NULL;
	}

	// 绑定该连接到完成端口
	NeSock* pkSock = CreateSock(hSocket,NULL,pkConnect);
	if(!pkSock)
	{
		close(hSocket);
		return NULL;
	}

	return pkSock;
}


bool NeEpollMgr::Listen(int iListenPort,int iListenNum,int iBufferSize,int iFlag)
{
	if(iListenPort <= 0)
	{
		//PRINT("iListenPort <= 0.\n");
		return false;
	}

	// 查看该端口是否已经在监听了
	for(int i = 0;i < (int)m_akListen.size();i++)
	{
		if(m_akListen[i].iPort == iListenPort)
		{
			//PRINT("m_akListen[i].iPort == iListenPort.\n");
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
	SOCKET hListenSock = (int)socket(AF_INET,SOCK_STREAM,0);
	if(hListenSock == INVALID_SOCKET)
	{
		//PRINT("hListenSock == INVALID_SOCKET.\n");
		return false;
	}

    int option = 1;
    setsockopt(hListenSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&option,sizeof(option));
    int arg = 1;
    ::ioctl(hListenSock, FIONBIO, &arg);
	// 绑定端口
	struct sockaddr_in kAddress;
	kAddress.sin_family = AF_INET;
	kAddress.sin_port = ntohs((u_short)iListenPort);
	kAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(hListenSock,(const sockaddr*)&kAddress, sizeof(kAddress)) != 0)
	{
		//PRINT("bind(hListenSock,(const sockaddr*)&kAddress, sizeof(kAddress)) != 0.\n");
		return false;
	}

	if(listen(hListenSock,4) != 0)
	{
		//PRINT("listen(hListenSock,4) != 0\n");
		return false;
	}

	NeListen kListen;
	kListen.hSock = hListenSock;
	kListen.iPort = iListenPort;
	kListen.iBufferSize = iBufferSize;
	kListen.iFlag = iFlag;
	kListen.iListenNum = iListenNum;

    kListen.apkAcceptOV = new NeAcceptOV;
    kListen.apkAcceptOV->hListenSock = hListenSock;
    kListen.apkAcceptOV->iPort = iListenPort;
    kListen.apkAcceptOV->iBufferSize = iBufferSize;
    kListen.apkAcceptOV->iFlag = iFlag;      

	AddSocket(hListenSock);

	m_akListen.push_back(kListen);

    return true;
}


void NeEpollMgr::AddSocket(SOCKET sFD)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	ev.events = EPOLLIN;
	ev.events |= EPOLLET;		
	ev.data.fd = sFD;

	if(epoll_ctl(m_iEpollFD, EPOLL_CTL_ADD, ev.data.fd, &ev))
	{
		printf("AddSocket Fail[%d].\n",sFD);
	}
}

void NeEpollMgr::RemoveSocket(SOCKET sFD)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	ev.data.fd = sFD;
	ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLONESHOT;

	if(epoll_ctl(m_iEpollFD, EPOLL_CTL_DEL, ev.data.fd, &ev))
	{
		printf("RemoveSocket Fail[%d].\n",sFD);
	}
}
void NeEpollMgr::PostEvent(SOCKET sFD,int iEvents)
{
    struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
    ev.data.fd = sFD;
    ev.events = iEvents | EPOLLET;			/* use edge-triggered instead of level-triggered because we're using nonblocking sockets */

    // post actual event
    if(epoll_ctl(m_iEpollFD, EPOLL_CTL_MOD, ev.data.fd, &ev))
    {
		printf("Could not post event on fd %u", sFD);
    }
}

TeSendResult NeEpollMgr::SendData(const NeHSock& rkSock,char* pcData,int iSize)
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
		MSock::iterator itr = m_kAllSock.find(rkSock.pkSock->hSocket);
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
    rkLock.UnLock();
	if(bNeedSend)
	{
        /*
        char* pcData = rkSendBuf.GetBuffer();
		int   iLen   = rkSendBuf.GetSize();

        int iRet = Write(rkSock.pkSock->hSocket,pcData,iLen);
        if(iRet < 0)
        {
             PushDisconnect(rkSock,TDC_RECV_ERROR2,errno);
        }else
        {
            rkSendBuf.Retract(iRet);
            if(iLen != iRet && iRet != 0)
            {
                PostEvent(rkSock.pkSock->hSocket,EPOLLOUT);
            }
        }

    }else if(eRet == TSR_SUCCESS)
    {
         PostEvent(rkSock.pkSock->hSocket,EPOLLOUT);
    */
    }
    PostEvent(rkSock.pkSock->hSocket,EPOLLOUT);
	return eRet;
}
void NeEpollMgr::Disconnect(const NeHSock& rkSock)
{
	PushDisconnect(rkSock,TDC_SELF_DISC,0);
}

void*  NeEpollMgr::NetProcThread(void* pkParam)
{
	NeEpollMgr* pkThis = (NeEpollMgr *)pkParam;
	pkThis->NetProc();

}
void NeEpollMgr::NetProc()
{
	PRINT("Net Thread Begin[%d].\n",(int)m_bIsRun);
	std::vector<SOCKET>  kNotSendList;
	while(m_bIsRun)
	{
		int iFdCount = epoll_wait(m_iEpollFD,m_kEvents,THREAD_EVENT_SIZE,5000);
		if(iFdCount < 0)
		{
			continue;;
		}

		for(int i = 0; i < iFdCount; ++i)
		{
			NeSock* pkSock = NULL;
            m_kSockLock.Lock();
			MSock::iterator itr = m_kAllSock.find(m_kEvents[i].data.fd);
			if(itr != m_kAllSock.end())
			{
				pkSock = itr->second;
			}
            m_kSockLock.UnLock();
			if(pkSock == NULL)
			{
				for(int j = 0;j < m_akListen.size();j++)
				{
					if(m_akListen[j].hSock != m_kEvents[i].data.fd)
					{
                        continue;
                    }
                    Accept(m_akListen[j].hSock,m_akListen[j].apkAcceptOV);
                    break;
				}
				continue;
			}

			if(m_kEvents[i].events & EPOLLHUP || m_kEvents[i].events & EPOLLERR)
			{
				// 关闭一个连接
				PushDisconnect(NeHSock(pkSock,pkSock->dwAllocID),TDC_SYSTEM_ERROR,errno);
				continue;
			}else if(m_kEvents[i].events & EPOLLIN)
			{
			    NeBuffer&     rkRecvBuf = pkSock->kRecvBuf;
                int iRet = ReadData(pkSock->hSocket,rkRecvBuf);
                if(iRet < 0)
                {
                    PushDisconnect(NeHSock(pkSock,pkSock->dwAllocID),TDC_RECV_ERROR1,errno);
                    continue;
                }else
                {
                    // 检查接收之后的数据是否构成了一个完整包
                    int iPos = 0;
                    NePack kPack;
                    while(rkRecvBuf.GetPack(iPos,kPack))
                    {
                        PushRecv(NeHSock(pkSock,pkSock->dwAllocID),kPack.pcData,kPack.iSize);
                        iPos += kPack.iAddSize;
                    }
                    if(iPos > 0)
                    {
                        rkRecvBuf.Retract(iPos);
                    }
                }

                // 进行下一次接收
                PostEvent(pkSock->hSocket,EPOLLOUT);

			}else if (m_kEvents[i].events & EPOLLOUT)
			{
	            NeBuffer& rkSendBuf = pkSock->kSendBuf;
			    NeSendBuffer& rkExtraBuf = pkSock->kExtraBuf;

			    pkSock->kSendLock.Lock();
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
                int iRet = WriteData(pkSock->hSocket,rkSendBuf);
                pkSock->kSendLock.UnLock();

                if(iRet < 0)
                {
                    PushDisconnect(NeHSock(pkSock,pkSock->dwAllocID),TDC_SEND_ERROR1,errno);
                    continue;
                }else if(iRet == 1)
				{
					kNotSendList.push_back(pkSock->hSocket);
				}
                PostEvent(pkSock->hSocket,EPOLLIN);    
			}
		}
		for(int i = 0;i < kNotSendList.size();i++)
		{
			PostEvent(kNotSendList[i],EPOLLOUT);
		}
		kNotSendList.clear();
		usleep(500);
	}
	m_bExit = true;
	PRINT("Net Thread End[%d].\n",(int)m_bIsRun);
}


void NeEpollMgr::PushConnected(const NeHSock& rkSock, BYTE abyIP[], int iPort)
{
	NePostData kPost;
	kPost.kSock = rkSock;
	kPost.eType = NPT_CONNECTED;
	memcpy(kPost.acIP,abyIP, sizeof(kPost.acIP));
	kPost.iPort = iPort;
	PushPost(kPost);
}

void NeEpollMgr::PushAccept(const NeHSock& rkSock,int iAcceptPort,BYTE abyIP[],int iPort)
{
	NePostData kPost;
	kPost.kSock = rkSock;
	kPost.eType = NPT_ACCEPTED;
	kPost.iAcceptPort = iAcceptPort;
	sprintf(kPost.acIP, "%d.%d.%d.%d", abyIP[0], abyIP[1], abyIP[2], abyIP[3]);
	kPost.iPort = iPort;
	PushPost(kPost);
}

void NeEpollMgr::PushDisconnect(const NeHSock& rkSock,TeDisconnectCode eCode,int iParam)
{
	bool bFind = false;
	NeSock* pkSock = NULL;
	m_kSockLock.Lock();

	MSock::iterator itr = m_kAllSock.find(rkSock.pkSock->hSocket);
	if(itr != m_kAllSock.end())
	{
		bFind = true;
		pkSock = itr->second;
        RemoveSocket(rkSock.pkSock->hSocket);
		m_kAllSock.erase(itr);
	}
	m_kSockLock.UnLock();

	if(bFind)
	{
		if(eCode == TDC_SELF_DISC)
		{
	        NeBuffer& rkSendBuf = rkSock.pkSock->kSendBuf;
			NeSendBuffer& rkExtraBuf = rkSock.pkSock->kExtraBuf;

			rkSock.pkSock->kSendLock.Lock();
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
            WriteData(rkSock.pkSock->hSocket,rkSendBuf);
            rkSock.pkSock->kSendLock.UnLock();
		}
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

void NeEpollMgr::PushPost(NePostData& rkData)
{
	m_kPostLock.Lock();
	m_kPostData.push(rkData);
	m_kPostLock.UnLock();
}

void NeEpollMgr::PushRecv(const NeHSock& rkSock,char* pcData,int iSize)
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

NeSock* NeEpollMgr::CreateSock(SOCKET hSocket,NeAcceptOV* pkAcceptOV,NeConnect* pkConnect)
{
	if(!pkAcceptOV && !pkConnect || hSocket == INVALID_SOCKET)
	{
		PRINT("Socket[%d]!pkAcceptOV && !pkConnect || hSocket == INVALID_SOCKET.\n",hSocket);
		return NULL;
	}

    // 修改该socket为非阻塞模式
    ulong ulValue = 1;
    int iRet = ::ioctl(hSocket,FIONBIO,&ulValue);
    if(iRet == SOCKET_ERROR)
    {
		PRINT("Socket[%d]iRet == SOCKET_ERROR.\n",hSocket);
        close(hSocket);
        return NULL;
    }

	int iBufferSize = 0;
	int iFlag = 0;
	if(pkAcceptOV)
	{
		// 继承hListenSock属性
		iBufferSize = pkAcceptOV->iBufferSize;
		iFlag = pkAcceptOV->iFlag;
	}
	else
	{
		iBufferSize = pkConnect->iBufferSize;
		iFlag = pkConnect->iFlag;
	}

//	if(iFlag & TSF_NO_DELAY)
	{
		//bool bNoDelay = true;
		int flag;
		//int ret = setsockopt(hSocket,IPPROTO_TCP,TCP_NODELAY,(char*)&bNoDelay,sizeof(bNoDelay));
		int ret = setsockopt(hSocket,IPPROTO_TCP,TCP_NODELAY,(char*)&flag,sizeof(flag));
		if (ret != -1) {
		   printf("SET NODELAY OK %u\n", hSocket);
		} else {
		   printf("SET NODELAY error %u %d %s\n", hSocket, errno, strerror(errno));
		}
	}

    KeepAlive(hSocket);
	SetLinger(hSocket);

	NeSock* pkSock = AllocSock(hSocket,iBufferSize,iFlag);
	if(!pkSock)
	{
		PRINT("Socket[%d]!pkSock.\n",hSocket);
		NET_ERROR();
		return NULL;
	}

	m_kSockLock.Lock();
	m_kAllSock[pkSock->hSocket] = pkSock;
	m_kSockLock.UnLock();
	
	if(pkAcceptOV)
	{
        BYTE abyIP[4];
		memcpy(abyIP,(void*)&pkAcceptOV->kAddress.sin_addr,sizeof(abyIP));
        //char* pcIP = (char*)inet_ntoa(pkAcceptOV->kAddress.sin_addr);
		int iPort = ntohs(pkAcceptOV->kAddress.sin_port);
		PushAccept(NeHSock(pkSock,pkSock->dwAllocID),pkAcceptOV->iPort,(BYTE*)abyIP,iPort);
	}
	else
	{
		NeHSock kSock(pkSock,pkSock->dwAllocID);
		PushConnected(kSock,(BYTE*)pkConnect->acIP,pkConnect->iPort);
	}

    AddSocket(hSocket);
	
	return pkSock;
}

void NeEpollMgr::ReleaseSocket(SOCKET hSocket)
{
    shutdown(hSocket,SHUT_RDWR);
    close(hSocket);
}

NeSock*	NeEpollMgr::AllocSock(SOCKET hSocket,int iBufferSize,int iFlag)
{
	if(hSocket == INVALID_SOCKET)
	{
		PRINT("Socket[%d]hSocket == INVALID_SOCKET\n",hSocket);
		return NULL;
	}

	// 确定socket是否可以绑定到完成端口
	DWORD dwAllocID = (DWORD)__sync_add_and_fetch(&m_lGenID,1);

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
	PRINT("InitBuffer Size %u\n",iBufferSize);
	return pkSock;
}

void NeEpollMgr::FreeSock(NeSock* pkSock)
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

void NeEpollMgr::Accept(SOCKET sFd,NeAcceptOV* pkOV)
{
    SOCKET aSocket = -1;
    int iLen = sizeof(sockaddr_in);
    while(aSocket = accept(sFd,(sockaddr*)&pkOV->kAddress,(socklen_t*)&iLen))
    {
        if(aSocket <= 0)
        {
            NET_ERROR();
            ReleaseSocket(aSocket);
            break;
        }
        NeSock* pkSock = CreateSock(aSocket,pkOV,NULL);
        if(!pkSock)
        {
            NET_ERROR();
			PRINT("Socket[%d]!pkSock.\n",aSocket);
            ReleaseSocket(aSocket);
            continue;
        }

    }
}


/*
 * 读数据
 */

int  NeEpollMgr::ReadData(SOCKET sFd,NeBuffer& rkBuf)
{
    if(sFd == INVALID_SOCKET) 
    {
		PRINT("Socket[%d]sFd == INVALID_SOCKET.\n",sFd);
        return -1;
    }
    char* pcData = rkBuf.GetFreeBuffer();
    int   iLen = rkBuf.GetFreeSize();
    while(pcData && iLen > 0)
    {
		int iRet = recv(sFd,pcData,iLen,0);
        if(iRet < 0)
        {
			if(errno == EAGAIN || errno == EWOULDBLOCK)
			{
				return 0;
			}else if(errno == EINTR)
			{
				PRINT("Socket[%d] Error[%d] iBytes < 0.\n",sFd,errno);
				break;
			}else
			{
				PRINT("Socket[%d] Error[%d] iBytes < 0.\n",sFd,errno);
				return -1;
			}
        }else if(iRet == 0)
        {
            return -1;
        }else
        {
            if(!rkBuf.Append(iRet))
            {
				PRINT("Socket[%d]iRet < 0[%d].\n",iRet,sFd);
                return -1;
            }
        }
        pcData = rkBuf.GetFreeBuffer();
        iLen = rkBuf.GetFreeSize();
    }
    return 1;
}
/*
 * 写数据
 */
int  NeEpollMgr::WriteData(SOCKET sFd,NeBuffer& rkBuf)
{
    if(sFd == INVALID_SOCKET) 
    {
		PRINT("Socket[%d]sFd == INVALID_SOCKET.\n",sFd);
        return -1;
    }

    char* pcData = rkBuf.GetBuffer();
    int iLen = rkBuf.GetSize();

    while(pcData && iLen > 0)
    {
		int iRet = send(sFd,pcData,iLen,0);
        if(iRet < 0)
        {
			if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
			{
				PRINT("Socket[%d] Error[%d] iBytes < 0.errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR \n",sFd,errno);
				break;
			}else
			{
				PRINT("Socket[%d] Error[%d] iBytes < 0.\n",sFd,errno);
				return -1;
			}
        }else if(iRet == 0)
        {
			PRINT("Socket[%d] Error[%d] iBytes < 0.iRet == 0 \n",sFd,errno);
            return -1;
        }else
        {
			//PRINT("Socket[%d] Error[%d] [%d]iBytes < 0.\n",sFd,errno,iRet);
            rkBuf.Retract(iRet);
        }
		//PRINT("Socket[%d] Send Size[%d] Real Size[%d] Error[%d].\n",sFd,iLen,iRet,errno);
        pcData = rkBuf.GetBuffer();
        iLen = rkBuf.GetSize();
    }
	if(pcData && iLen != 0)
	{
		PRINT("Socket[%d] Error[%d] pcData && iLen[%d] != 0.iRet == 0 \n",sFd,errno,iLen);
		return 1;
	}
    return 0;
}
void NeEpollMgr::KeepAlive(SOCKET fd)
{
    int iOption = 1;
    int iKeepIdle = 60;
    int iKeepInterval = 20;
    int iKeepCount = 3;

    setsockopt(fd,SOL_SOCKET,SO_KEEPALIVE,(const char*)&iOption,sizeof(iOption));
    setsockopt(fd,SOL_TCP,TCP_KEEPIDLE,(void *)&iKeepIdle, sizeof(iKeepIdle));
    setsockopt(fd,SOL_TCP,TCP_KEEPINTVL,(void *)&iKeepInterval, sizeof(iKeepInterval));
    setsockopt(fd,SOL_TCP,TCP_KEEPCNT,(void *)&iKeepCount, sizeof(iKeepCount)); 
}

void NeEpollMgr::SetLinger(SOCKET fd)
{
    struct linger kLinger = {0};
    kLinger.l_onoff = 1;
    kLinger.l_linger = 0;
    setsockopt(fd,SOL_SOCKET,SO_LINGER,(const char*)&kLinger,sizeof(kLinger));
}

void Printf(const char* files,const char* func,int iLine,const char *format,...)
{
	char acString1[2048] = {0};
	char acString2[2048] = {0};
	va_list argptr;

	va_start(argptr,format);
	vsprintf(acString1,format,argptr);
	va_end(argptr);

	SYSTEMTIME kCurTime;
	GetLocalTime(&kCurTime);

	snprintf(acString2,sizeof(acString2),"%04d-%02d-%02d %02d:%02d:%02d [%s][%d]> %s",kCurTime.wYear,kCurTime.wMonth,kCurTime.wDay,kCurTime.wHour,kCurTime.wMinute,kCurTime.wSecond,func,iLine,acString1);

	char acLogFile[256] = "Socket.log";
	FILE* pkLogFile = fopen(acLogFile,"a+");
	if(pkLogFile)
	{
		fputs(acString2,pkLogFile);
		fflush(pkLogFile);
		fclose(pkLogFile);
	}

}

#endif
