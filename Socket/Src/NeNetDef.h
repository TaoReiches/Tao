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


// Winsock׼������
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
// �ⲿʹ��Sock
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

// OV����
enum NeOVType
{
	NOV_RECV = 0,	// ����
	NOV_SEND,	// ����
};

// �ͻ������Ӷ���
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

// OV����
struct NeOverlap : public OVERLAPPED
{
	NeOVType	eType;
	NeHSock		kSock;
	WSABUF		kWSABuf;
};


// AcceptOV����
#define NET_ADDR_LEN	(sizeof(SOCKADDR_IN)+16)
#define NET_APTBUF_LEN	256
struct NeAcceptOV : public OVERLAPPED
{
	// �����󲻻�ı��
	SOCKET	hListenSock;				// ����socket
	int		iPort;						// �����˿�
	int		iBufferSize;				// �����յ�socket����Ļ����С
	int		iFlag;						// ��������

	// ��̬�ı��
	SOCKET	hSocket;					// Ԥ����sokcet
	char	acBuffer[NET_APTBUF_LEN];
};
#elif defined __LINUX__
struct NeAcceptOV
{
	// �����󲻻�ı��
	SOCKET	hListenSock;				// ����socket
	int		iPort;						// �����˿�
	int		iBufferSize;				// �����յ�socket����Ļ����С
	int		iFlag;						// ��������

	// ��̬�ı��
	SOCKET	hSocket;					// Ԥ����sokcet
	struct sockaddr_in	kAddress;
};
#else
struct NeAcceptOV
{
};
#endif

// �����ṹ����
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

	SOCKET		hSock;			// ����socket
	int			iPort;			// �����˿�
	int			iBufferSize;	// �����յ�socket����Ļ����С
	int			iFlag;			// ��������
	int			iListenNum;		// ������������
	NeAcceptOV*	apkAcceptOV;	// �������ӻ���

};
typedef std::vector<NeListen> VListen;

// �Զ���Sock
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

// Ͷ������
enum NePostType
{
	NPT_CONNECTED	,	// �ͻ������ӷ��ؽ��
	NPT_ACCEPTED	,	// �������˽��յ�����
	NPT_RECV		,	// �յ��������ݰ�
	NPT_DISCONNECT	,	// �Ͽ�����
	NPT_SEND		,	// ��������
};

// Ͷ������
struct NePostData
{
	NeHSock			kSock;
	NeHMulitSock	kMulitSock;
	int				iID;
	NePostType		eType;

	// ��������
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
 