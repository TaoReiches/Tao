#pragma once

class HSock
{
public:
	HSock(void* sock = nullptr,unsigned int id = 0):pkSock(sock),uiAllocID(id)
	{
	}

	bool operator < (const HSock& rkR) const
	{
		if((pkSock < rkR.pkSock) || (pkSock == rkR.pkSock && uiAllocID < rkR.uiAllocID))
		{
			return true;
		}
		return false;
	}

	bool operator == (const HSock& rkR) const
	{
		if(pkSock == rkR.pkSock && uiAllocID == rkR.uiAllocID)
		{
			return true;
		}
		return false;
	}

	bool IsInvalid(void) const
	{
		if(uiAllocID != 0 && pkSock != nullptr)
		{
			return false;
		}
		return true;
	}

	unsigned int	uiAllocID;
	void*			pkSock;
};

enum class TeDisconnectCode
{
	TDC_SELF_DISC	= 0	,
	TDC_SYSTEM_ERROR	,
	TDC_RECV_ERROR1		,
	TDC_RECV_ERROR2		,
	TDC_SEND_ERROR1		,
	TDC_SEND_ERROR2		,
};

class TeNetCall
{
public:
	TeNetCall(void){}
	virtual ~TeNetCall(void){}

	// TCP
	virtual void OnNetConnected(const HSock& rkSock, const char* pcIP, int iPort) = 0;
	virtual void OnNetAccept(const HSock& rkSock, int iListenPort, const char* pcIP, int iPort) = 0;
	virtual void OnNetRecv(const HSock& rkSock, void* pkData, int iSize) = 0;
	virtual void OnNetDisconnect(const HSock& rkSock, TeDisconnectCode eCode, int iParam) = 0;
};

enum class TeSendResult
{
	TSR_SUCCESS		= 0	,
	TSR_NET_ERROR		,
	TSR_NET_BUSY		,
	TSR_NET_CLOSED		,
	TSR_PARAM_ERROR		,
};

enum TeSockFlag
{
	TSF_16BIT_WIN	= (1<<0),
	TSF_32BIT_WIN	= (1<<1),
	TSF_16BIT_LINUX	= (1<<2),
	TSF_32BIT_LINUX	= (1<<3),
	TSF_NO_DELAY	= (1<<4),
};

class TeNetMgr
{
public:
	TeNetMgr(void);
	~TeNetMgr(void);

	bool Initialize(TeNetCall* pkCall,int iMaxThread = 2);
	void Update(void);
	void Finialize(void);

	bool Connect(const char* pcIP,int iPort,int iTimeOut = 2000,int iBufferSize = 8 * 1024, TeSockFlag iFlag = TeSockFlag::TSF_16BIT_WIN);
	bool Listen(int iListenPort,int iListenNum = 8,int iBufferSize = 8 * 1024, TeSockFlag iFlag = TeSockFlag::TSF_16BIT_WIN);
	bool SendData(const HSock& rkSock,void* pkData,int iSize);
	TeSendResult SendDataDetail(const HSock& rkSock,void* pkData,int iSize);
	void Disconnect(const HSock& rkSock);

protected:
	void*   m_pkData;
};
