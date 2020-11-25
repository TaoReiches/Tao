#include "NeUdpMgr.h"

NeUdpMgr::NeUdpMgr(void)
{
	m_pkNetCall = NULL;
	m_pkNetHost = NULL;
	m_pkNetClient = NULL;
	m_lGenID = 1;
}

NeUdpMgr::~NeUdpMgr(void)
{
	Finialize();
}

bool NeUdpMgr::Initialize(TeNetCall* pkNetCall)
{
	if(enet_initialize())
	{
		return false;
	}

	m_pkNetCall = pkNetCall;

	m_kUpdateThread.SetVoid(this);
	m_kUpdateThread.Start();

	return true;
}

void NeUdpMgr::Update(void)
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
				m_pkNetCall->OnNetUdpConnected(kData.iID, kData.acIP, kData.iPort);
			}
			break;
		}
		case NPT_ACCEPTED:
		{
			if (m_pkNetCall)
			{
				m_pkNetCall->OnNetUdpAccept(kData.iID, kData.iAcceptPort, kData.acIP, kData.iPort);
			}
			break;
		}
		case NPT_RECV:
		{
			char* pcData = m_kRecvBuffer.GetData(kData.iSize);
			if (m_pkNetCall && pcData)
			{
				m_pkNetCall->OnNetUdpRecv(kData.iID, pcData, kData.iSize);
			}
			break;
		}
		case NPT_DISCONNECT:
		{
			if (m_pkNetCall)
			{
				m_pkNetCall->OnNetUdpDisconnect(kData.iID);
			}
			break;
		}
		}
	}
}

void NeUdpMgr::Finialize(void)
{
	// 结束UDP
	m_kUpdateThread.Stop();

	if(m_pkNetHost)
	{
		enet_host_destroy(m_pkNetHost);
	}

	if(m_pkNetClient)
	{
		enet_host_destroy(m_pkNetClient);
	}	
}

bool NeUdpMgr::Listen(int iPort, int iMaxCount,int iBufferSize)
{
	ENetAddress kAddress;
	kAddress.host = ENET_HOST_ANY;
	kAddress.port = iPort;
	m_pkNetHost = enet_host_create(&kAddress, iMaxCount, 0, 0, 0);
	if (m_pkNetHost == NULL)
	{
		enet_deinitialize();
		return false;
	}
	
	return true;
}

bool NeUdpMgr::Connect(const char* pcIP, int iPort, int iTimeOut, int iBufferSize)
{
	if(!pcIP || iPort == 0)
	{
		return false;
	}
	
	if(!m_pkNetClient)
	{
		m_pkNetClient = enet_host_create(NULL, 1, 2, 64 * 1024, 64 * 1024);
		if(!m_pkNetClient)
		{
			return false;
		}
	}

	ENetAddress kAddress;
	enet_address_set_host(&kAddress, pcIP);
	kAddress.port = iPort;

	ENetPeer* pkPeer = enet_host_connect(m_pkNetClient,&kAddress,2,0);
	if(!pkPeer)
	{
		return false;
	}

	return true;
}

bool NeUdpMgr::SendData(int iConnectID, char* pcData, int iSize,int iFlag)
{
	return PushSend(iConnectID, pcData, iSize,iFlag);
}

bool NeUdpMgr::Disconnect(int iConnect)
{
	// 主动断开连接
	PushActiveDisconnect(iConnect);
	return true;
}

void NeUdpMgr::ThreadUpdate()
{
	while(!m_kSendData.empty())
	{	
		m_kSendMutex.Lock();
		NePostData kData = m_kSendData.front();
		m_kSendData.pop();
		m_kSendMutex.Unlock();

		switch (kData.eType)
		{
		case NPT_SEND:
		{
			char* pcData = m_kSendBuffer.GetData(kData.iSize);
			if (pcData)
			{
				auto iMapItr = m_kMapAllPeer.find(kData.iID);
				if (iMapItr != m_kMapAllPeer.end())
				{
					ENetPacket* pkPacket = enet_packet_create(pcData, kData.iSize, kData.iParam);
					if (!pkPacket)
					{
						continue;
					}

					enet_peer_send(iMapItr->second, 0, pkPacket);
				}
			}
			break;
		}
		case NPT_DISCONNECT:
		{
			auto iMapItr = m_kMapAllPeer.find(kData.iID);
			if (iMapItr != m_kMapAllPeer.end())
			{
				enet_peer_disconnect(iMapItr->second, 0);
			}
			break;
		}
		default:
		{
			// error
			break;
		}
		}
	}	

	if (m_pkNetHost != NULL)
	{
		ENetEvent kEvent;		
		while (enet_host_service(m_pkNetHost, &kEvent, 0) >= 0)
		{
			if (kEvent.type == ENET_EVENT_TYPE_CONNECT)
			{
				ENetAddress kRemote = kEvent.peer->address;
				char acIP[256] = { 0 };
				enet_address_get_host_ip(&kRemote, acIP, 256);
				kEvent.peer->data = (void*)m_lGenID++;

				AddPeer((intptr_t)kEvent.peer->data, kEvent.peer);
				PushAccept((intptr_t)kEvent.peer->data,m_pkNetHost->address.port,(BYTE*)acIP,kRemote.port);
			}
			else if (kEvent.type == ENET_EVENT_TYPE_RECEIVE)				// 收到数据
			{				
				PushRecv((intptr_t)kEvent.peer->data, (char*)kEvent.packet->data, kEvent.packet->dataLength);
				enet_packet_destroy(kEvent.packet);				
			}
			else if (kEvent.type == ENET_EVENT_TYPE_DISCONNECT)				// 失去连接
			{
				DelPeer((intptr_t)kEvent.peer->data);
				PushDisconnect((intptr_t)kEvent.peer->data);
			}
			else if (kEvent.type == ENET_EVENT_TYPE_NONE)
			{
				// 
				break;
			}
		}
	}

	if (m_pkNetClient != NULL)
	{
		ENetEvent kEvent;
		while (enet_host_service(m_pkNetClient, &kEvent, 0) >= 0)
		{
			if (kEvent.type == ENET_EVENT_TYPE_CONNECT)
			{
				ENetAddress kRemote = kEvent.peer->address;
				char acIP[256] = { 0 };
				enet_address_get_host_ip(&kRemote, acIP, 256);
				kEvent.peer->data = (void*)m_lGenID++;

				AddPeer((intptr_t)kEvent.peer->data, kEvent.peer);
				PushConnected((intptr_t)kEvent.peer->data, (BYTE*)acIP, kRemote.port);
			}
			else if (kEvent.type == ENET_EVENT_TYPE_RECEIVE)	// 收到数据
			{
				PushRecv((intptr_t)kEvent.peer->data, (char*)kEvent.packet->data, kEvent.packet->dataLength);
				enet_packet_destroy(kEvent.packet);				// 注意释放空间
			}
			else if (kEvent.type == ENET_EVENT_TYPE_DISCONNECT) // 失去连接
			{
				DelPeer((intptr_t)kEvent.peer->data);
				PushDisconnect((intptr_t)kEvent.peer->data);
			}
			else if (kEvent.type == ENET_EVENT_TYPE_NONE)
			{
				// 
				break;
			}
		}
	}
}

void NeUdpMgr::PushConnected(int iID, BYTE abyIP[], int iPort)
{
	NePostData kPost;
	kPost.iID = iID;
	kPost.eType = NPT_CONNECTED;
	memcpy(kPost.acIP, abyIP, sizeof(kPost.acIP));
	kPost.iPort = iPort;
	PushPost(kPost);
}

void NeUdpMgr::PushAccept(int iID, int iAcceptPort, BYTE abyIP[], int iPort)
{
	NePostData kPost;
	kPost.iID = iID;
	kPost.eType = NPT_ACCEPTED;
	kPost.iAcceptPort = iAcceptPort;
	memcpy(kPost.acIP, abyIP, sizeof(kPost.acIP));
	kPost.iPort = iPort;
	PushPost(kPost);
}

void NeUdpMgr::PushDisconnect(int iID)
{
	NePostData kPost;
	kPost.eType = NPT_DISCONNECT;
	kPost.iID = iID;
	PushPost(kPost);
}

void NeUdpMgr::PushPost(NePostData& rkData)
{
	m_kPostMutex.Lock();
	m_kPostData.push(rkData);
	m_kPostMutex.Unlock();
}

void NeUdpMgr::PushRecv(int iID, char* pcData, int iSize)
{
	m_kPostMutex.Lock();

	int iPushSize = m_kRecvBuffer.PushData(pcData, iSize);
	if (iPushSize == iSize && iPushSize > 0)
	{
		NePostData kPost;
		kPost.eType = NPT_RECV;
		kPost.iID = iID;
		kPost.iSize = iPushSize;
		m_kPostData.push(kPost);
	}

	m_kPostMutex.Unlock();
}

bool NeUdpMgr::PushSend(int iID, char* pcData, int iSize, int iFlag)
{
	m_kSendMutex.Lock();

	int iPushSize = m_kSendBuffer.PushData(pcData, iSize);
	if (iPushSize == iSize && iPushSize > 0)
	{
		NePostData kPost;
		kPost.eType = NPT_SEND;
		kPost.iID = iID;
		kPost.iSize = iPushSize;
		kPost.iParam = iFlag;
		m_kSendData.push(kPost);

		m_kSendMutex.Unlock();
		return true;
	}
	else
	{
		m_kSendMutex.Unlock();
		return false;
	}
}

void NeUdpMgr::PushActiveDisconnect(int iID)
{
	m_kSendMutex.Lock();
	NePostData kPost;
	kPost.eType = NPT_DISCONNECT;
	kPost.iID = iID;	
	m_kSendData.push(kPost);
	m_kSendMutex.Unlock();
}

void NeUdpMgr::AddPeer(int iID, ENetPeer* pkPeer)
{
	m_kMapAllPeer[iID] = pkPeer;
}

void NeUdpMgr::DelPeer(int iID)
{
	MapAllNetPeer::iterator iMapItr = m_kMapAllPeer.find(iID);
	if(iMapItr != m_kMapAllPeer.end())
	{
		m_kMapAllPeer.erase(iMapItr);
	}
}

void NeUdpMgr::NeUdpMgrThread::Run()
{
	while (IsRunning())
	{
		NeUdpMgr* pkUdpMgr = (NeUdpMgr*)m_pkData;
		if (pkUdpMgr)
		{
			pkUdpMgr->ThreadUpdate();
		}
#ifdef _WIN32
		Sleep(1);
#else
        sleep(0.001f);
#endif
	}	
}