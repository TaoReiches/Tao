#pragma once
#include "TeInclude.h"
#include "NeBuffer.h"
#include "NeSendBuffer.h"
#include "TeNetMgr.h"
#include "Include/SeLock.h"
#include "TeNetMgr.h"

#ifndef WIN32
#define  INVALID_SOCKET -1
#define  SOCKET			int
#define  SOCKET_ERROR   -1
#endif

#define MIN_THREAD_NUM		2
#define MAX_THREAD_NUM		8
#define WAIT_THREAD_TIME	6000
#define SOCKET_HOLDER_SIZE  50000
#define THREAD_EVENT_SIZE	4096

#ifdef WIN32
typedef LPFN_ACCEPTEX FAcceptEx;
typedef LPFN_GETACCEPTEXSOCKADDRS FSockaddrs;

struct NeWinSock
{
	NeWinSock(void)
	{
		WSADATA kWSAData;
		WSAStartup(0x0202, &kWSAData);
	}

	~NeWinSock(void)
	{
		WSACleanup();
	}
};

#endif

class NeSock;
class NeHSock
{
public:
	NeHSock(NeSock* sock = nullptr, unsigned int id = 0) :pkSock(sock), dwAllocID(id)
	{
	}

	bool operator < (const NeHSock& rkR) const
	{
		if ((pkSock < rkR.pkSock) || (pkSock == rkR.pkSock && dwAllocID < rkR.dwAllocID))
		{
			return true;
		}
		return false;
	}

	unsigned int	dwAllocID;
	NeSock*			pkSock;
};

enum class NeOVType
{
	NOV_RECV = 0,
	NOV_SEND,
};

class NeConnect
{
public:
	NeConnect()
	{
		memset(this, 0, sizeof(NeConnect));
	}

	char		acIP[16];
	int			iPort;
	int			iTimeOut;
	int			iBufferSize;
	int			iFlag;
};

#ifdef WIN32

class NeOverlap : public OVERLAPPED
{
public:
	NeOVType	eType;
	NeHSock		kSock;
	WSABUF		kWSABuf;
};

#define NET_ADDR_LEN	(sizeof(SOCKADDR_IN)+16)
#define NET_APTBUF_LEN	256
class NeAcceptOV : public OVERLAPPED
{
public:
	SOCKET	hListenSock;
	int		iPort;
	int		iBufferSize;
	int		iFlag;

	SOCKET	hSocket;
	char	acBuffer[NET_APTBUF_LEN];
};
#elif defined __LINUX__
struct NeAcceptOV
{
	SOCKET	hListenSock;
	int		iPort;
	int		iBufferSize;
	int		iFlag;

	SOCKET	hSocket;
	struct sockaddr_in	kAddress;
};
#else
struct NeAcceptOV
{
};
#endif

#define MAX_PACK_SIZE	(64 * 1024)
#define MIN_BUFFER_SIZE (8 * 1024)
#define MAX_BUFFER_SZIE (16 * 1024 * 1024)
class NeListen
{
public:
	NeListen()
	{
		hSock = INVALID_SOCKET;
		iPort = 0;
		iBufferSize = MIN_BUFFER_SIZE;
		iFlag = TeSockFlag::TSF_16BIT_WIN;
		iListenNum = 16;
#ifdef WIN32
		apkAcceptOV = nullptr;
#endif
	}

	SOCKET		hSock;
	int			iPort;
	int			iBufferSize;
    TeSockFlag	iFlag;
	int			iListenNum;
	NeAcceptOV*	apkAcceptOV;

};
typedef std::vector<NeListen> VListen;

class NeSock
{
public:
	SOCKET			hSocket;
	unsigned int	dwAllocID;

	SeLock			kRecvLock;
#ifdef WIN32
	NeOverlap		kRecvOV;
#endif
	NeBuffer		kRecvBuf;

	SeLock			kSendLock;
#ifdef WIN32
	NeOverlap		kSendOV;
#endif
	NeBuffer		kSendBuf;
	NeSendBuffer	kExtraBuf;
};
typedef std::map<unsigned int, NeSock*> MSock;

enum class NePostType
{
	NPT_CONNECTED	,
	NPT_ACCEPTED	,
	NPT_RECV		,
	NPT_DISCONNECT	,
	NPT_SEND		,
};

class NePostData
{
public:
	NeHSock			kSock;
	int				iID;
	NePostType		eType;

	char			acIP[16];
	int				iAcceptPort;
	int				iPort;
	int				iSize;
    TeDisconnectCode iCode;
	int				iParam;
};
typedef std::queue<NePostData> QPostData;

enum class NeIOCPKey
{
	NIK_ALL_EXIT = 0,
	NIK_CONNECT_KEY,
	NIK_ACCEPT_KEY,
	NIK_ORIGIN_KEY,
};

class NeDelSock
{
public:
	NeDelSock(unsigned int time, NeSock* sock) :dwDelTime(time), pkSock(sock)
	{
	}

	unsigned int	dwDelTime;
	NeSock*			pkSock;
};
typedef std::list<NeDelSock> LDelSock;

void NetError(const char* pcFun, int iLine);
#define NET_ERROR()	
 