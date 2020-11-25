#pragma once
#include "TeInclude.h"
#include "NeBuffer.h"
#include "NeSendBuffer.h"
#include "TeNetMgr.h"
#include "Include/SeLock.h"

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


// Winsock准备工作
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
// 外部使用Sock
struct NeSock;
struct NeHSock
{
	NeHSock(NeSock* sock = NULL, unsigned int id = 0) :pkSock(sock), dwAllocID(id)
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

class NeMulitSock;
struct NeHMulitSock
{
	NeHMulitSock(NeMulitSock* sock = NULL, unsigned int id = 0) :pkSock(sock), dwAllocID(id)
	{
	}

	bool operator < (const NeHMulitSock& rkR) const
	{
		if ((pkSock < rkR.pkSock) || (pkSock == rkR.pkSock && dwAllocID < rkR.dwAllocID))
		{
			return true;
		}
		return false;
	}

	unsigned int			dwAllocID;
	NeMulitSock*			pkSock;
};

// OV类型
enum NeOVType
{
	NOV_RECV = 0,	// 接收
	NOV_SEND,	// 发送
};

// 客户端连接定义
struct NeConnect
{
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

// OV定义
struct NeOverlap : public OVERLAPPED
{
	NeOVType	eType;
	NeHSock		kSock;
	WSABUF		kWSABuf;
};


// AcceptOV定义
#define NET_ADDR_LEN	(sizeof(SOCKADDR_IN)+16)
#define NET_APTBUF_LEN	256
struct NeAcceptOV : public OVERLAPPED
{
	// 创建后不会改变的
	SOCKET	hListenSock;				// 监听socket
	int		iPort;						// 监听端口
	int		iBufferSize;				// 给接收的socket分配的缓冲大小
	int		iFlag;						// 连接属性

	// 动态改变的
	SOCKET	hSocket;					// 预分配sokcet
	char	acBuffer[NET_APTBUF_LEN];
};
#elif defined __LINUX__
struct NeAcceptOV
{
	// 创建后不会改变的
	SOCKET	hListenSock;				// 监听socket
	int		iPort;						// 监听端口
	int		iBufferSize;				// 给接收的socket分配的缓冲大小
	int		iFlag;						// 连接属性

	// 动态改变的
	SOCKET	hSocket;					// 预分配sokcet
	struct sockaddr_in	kAddress;
};
#else
struct NeAcceptOV
{
};
#endif

// 监听结构定义
#define MAX_PACK_SIZE	(64 * 1024)
#define MIN_BUFFER_SIZE (8 * 1024)
#define MAX_BUFFER_SZIE (16 * 1024 * 1024)
struct NeListen
{
	NeListen()
	{
		hSock = INVALID_SOCKET;
		iPort = 0;
		iBufferSize = MIN_BUFFER_SIZE;
		iFlag = TSF_16BIT_WIN;
		iListenNum = 16;
#ifdef WIN32
		apkAcceptOV = NULL;
#endif
	}

	SOCKET		hSock;			// 监听socket
	int			iPort;			// 监听端口
	int			iBufferSize;	// 给接收的socket分配的缓冲大小
	int			iFlag;			// 连接属性
	int			iListenNum;		// 监听队列数量
	NeAcceptOV*	apkAcceptOV;	// 接收连接缓冲

};
typedef std::vector<NeListen> VListen;

// 自定义Sock
struct NeSock
{
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

// 投递类型
enum NePostType
{
	NPT_CONNECTED	,	// 客户端连接返回结果
	NPT_ACCEPTED	,	// 服务器端接收到连接
	NPT_RECV		,	// 收到网络数据包
	NPT_DISCONNECT	,	// 断开连接
	NPT_SEND		,	// 发送数据
};

// 投递数据
struct NePostData
{
	NeHSock			kSock;
	NeHMulitSock	kMulitSock;
	int				iID;
	NePostType		eType;

	// 具体数据
	char			acIP[16];
	int				iAcceptPort;
	int				iPort;
	int				iSize;
	int				iCode;
	int				iParam;
};
typedef std::queue<NePostData> QPostData;

enum NeIOCPKey
{
	NIK_ALL_EXIT = 0,
	NIK_CONNECT_KEY,
	NIK_ACCEPT_KEY,
	NIK_ORIGIN_KEY,
};

struct NeDelSock
{
	NeDelSock(unsigned int time, NeSock* sock) :dwDelTime(time), pkSock(sock)
	{
	}

	unsigned int	dwDelTime;
	NeSock*			pkSock;
};
typedef std::list<NeDelSock> LDelSock;

void NetError(const char* pcFun, int iLine);
#define NET_ERROR()	
 