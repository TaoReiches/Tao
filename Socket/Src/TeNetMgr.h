#pragma once
#include "stdio.h"
#include <iostream>
#include <vector>

// �Զ���Sock���
struct HSock
{
	HSock(void* sock = NULL,unsigned int id = 0):pkSock(sock),dwAllocID(id)
	{
	}

	inline bool operator < (const HSock& rkR) const
	{
		if((pkSock < rkR.pkSock) || (pkSock == rkR.pkSock && dwAllocID < rkR.dwAllocID))
		{
			return true;
		}
		return false;
	}

	inline bool operator == (const HSock& rkR) const
	{
		if(pkSock == rkR.pkSock && dwAllocID == rkR.dwAllocID)
		{
			return true;
		}
		return false;
	}

	inline bool IsInvalid(void) const
	{
		if(dwAllocID != 0 && pkSock != NULL)
		{
			return false;
		}
		return true;
	}

	unsigned int	dwAllocID;
	void*			pkSock;
};

// ����Ͽ���ԭ��
enum TeDisconnectCode
{
	TDC_SELF_DISC	= 0	,	// �������õĶϿ�
	TDC_SYSTEM_ERROR	,	// ϵͳ�ײ㷵�ظ�����ʧ��
	TDC_RECV_ERROR1		,	// ����ʧ��1
	TDC_RECV_ERROR2		,	// ����ʧ��2
	TDC_SEND_ERROR1		,	// ����ʧ��1
	TDC_SEND_ERROR2		,	// ����ʧ��2
};

// HTTP ���ؽ��
enum TeHttpResult
{
	THR_NULL = 0,
	// GET
	THR_GET_SUCCESS = 1,				// Ψһ�� HTTP���سɹ���ʶ
	THR_GET_HTTP_ERROR = 2,				// HOST ����HTTP������ HTTP����ʧ��  ���������� �ٶ� HTTP STATUS
	THR_GET_CONNECT_ERROR = 3,			// HOST ���Ӿ�ʧ����
	THR_GET_OTHER_ERROR = 4,			// �������� iParam:0 ����SOCKET����ʧ�� 1:��������ʧ�� 2:�򿪱�����ļ�ʧ��(�������ļ���δ�������߿�д·������)

	// POST
	THR_POST_SUCCESS = 100,				// POST�ɹ�
	THR_POST_HTTP_ERROR = 101,			// HOST ����HTTP������ HTTP����ʧ��  ���������� �ٶ� HTTP STATUS
	THR_POST_CONNECT_ERROR = 102,		// HOST ���Ӿ�ʧ����
	THR_POST_OTHER_ERROR = 103,			// �������� iParam:0 ����SOCKET����ʧ�� 1:POST�ļ���ʧ��

	// ���´������� ���� ERROR��
	THR_LOG_INFO_OR_WARNING = 10000,	// ���ص���Ϣ���߾��� ���Ժ���
};

// �ص�ʹ��
class TeNetCall
{
public:
	TeNetCall(void){}
	virtual ~TeNetCall(void){}

	// TCP
	virtual void OnNetConnected(const HSock& rkSock, const char* pcIP, int iPort){}
	virtual void OnNetAccept(const HSock& rkSock, int iListenPort, const char* pcIP, int iPort){}
	virtual void OnNetRecv(const HSock& rkSock, void* pkData, int iSize){}
	virtual void OnNetDisconnect(const HSock& rkSock, TeDisconnectCode eCode, int iParam){}

	// UDP
	virtual void OnNetUdpConnected(int iID,const char* pcIP,int iPort){}
	virtual void OnNetUdpAccept(int iID,int iListtenPort,const char* pcIP,int iPort){}
	virtual void OnNetUdpRecv(int iID,void* pkData,int iSize){}
	virtual void OnNetUdpDisconnect(int iID){}

	// HTTP
	virtual void OnHttpResult(TeHttpResult eType, int iKey, int iParam, const char* pcHost, const char* pcUrl, const char* pcBody,const char* pcError = ""){}
};

// ���ͽ��
enum TeSendResult
{
	TSR_SUCCESS		= 0	,	// ���ͳɹ�
	TSR_NET_ERROR		,	// �������(һ�����Ѿ��Ͽ���)
	TSR_NET_BUSY		,	// ���緱æ(һ���Ƿ��ͻ�������)
	TSR_NET_CLOSED		,	// �����Ѿ��ر�
	TSR_PARAM_ERROR		,	// ��������
};

// ����Socket������
enum TeSockFlag
{
	TSF_16BIT_WIN	= (1<<0),	// 16λ���ȵ�winϵͳ
	TSF_32BIT_WIN	= (1<<1),	// 32λ���ȵ�winϵͳ
	TSF_16BIT_LINUX	= (1<<2),	// 16λ���ȵ�linuxϵͳ
	TSF_32BIT_LINUX	= (1<<3),	// 32λ���ȵ�linuxϵͳ
	TSF_NO_DELAY	= (1<<4),	// �������ݲ�������ʱ
};

// UDP SNED FLAG
// this enum equals to _ENetPacketFlag  don't modify this please
enum TeUDPSendFlag
{
	/** packet must be received by the target peer and resend attempts should be
	* made until the packet is delivered */
	UDP_PACKET_FLAG_RELIABLE			= (1<<0),
	/** packet will not be sequenced with other packets
	* not supported for reliable packets
	*/
	UDP_PACKET_FLAG_UNSEQUENCED			= (1<<1),
	/** packet will not allocate data, and user must supply it instead */
	UDP_PACKET_FLAG_NO_ALLOCATE			= (1<<2),
	/** packet will be fragmented using unreliable (instead of reliable) sends
	* if it exceeds the MTU */
	UDP_PACKET_FLAG_UNRELIABLE_FRAGMENT = (1<<3),
	/** whether the packet has been sent from all queues it has been entered into */
	UDP_PACKET_FLAG_SENT				= (1<<8),
};

// TCP���������(������WINDOWS UNIX LINUX ���ȶ�����֤ �����ڷ�����)
class TeNetMgr
{
public:
	TeNetMgr(void);
	~TeNetMgr(void);

	bool Initialize(TeNetCall* pkCall,int iMaxThread = 2);
	void Update(void);
	void Finialize(void);

	bool Connect(const char* pcIP,int iPort,int iTimeOut = 2000,int iBufferSize = 8 * 1024,int iFlag = TSF_16BIT_WIN);
	bool Listen(int iListenPort,int iListenNum = 8,int iBufferSize = 8 * 1024,int iFlag = TSF_16BIT_WIN);
	bool SendData(const HSock& rkSock,void* pkData,int iSize);
	TeSendResult SendDataDetail(const HSock& rkSock,void* pkData,int iSize);
	void Disconnect(const HSock& rkSock);

protected:
	void* m_pkData;
};

// ��ƽ̨TCP���������(����C++Socket �н�ǿ�Ŀ�ƽ̨ͨ���� ʵ�ַ�ʽ������TeNetMgr��2�ְ���)
class TeMulitPlatNetMgr
{
public:
	TeMulitPlatNetMgr(void);
	~TeMulitPlatNetMgr(void);

	bool Initialize(TeNetCall* pkCall);
	void Update(void);
	void Finialize(void);

	bool Connect(const char* pcIP, int iPort, int iTimeOut = 2000, int iBufferSize = 8 * 1024, int iFlag = TSF_16BIT_WIN);
	bool Listen(int iListenPort, int iListenNum = 8, int iBufferSize = 8 * 1024, int iFlag = TSF_16BIT_WIN);
	bool SendData(const HSock& rkSock, void* pkData, int iSize);
	TeSendResult SendDataDetail(const HSock& rkSock, void* pkData, int iSize);
	void Disconnect(const HSock& rkSock);

protected:
	void* m_pkData;
};

// HTTP���������(����C++Socket ��ǿ��ƽ̨�Լ���չ��)
class TeMulitPlatHttMgr
{
public:
	TeMulitPlatHttMgr(void);
	~TeMulitPlatHttMgr(void);

	bool Initialize(TeNetCall* pkCall);
	void Update(void);
	void Finialize(void);

	// �����ļ���ָ��Ŀ¼
	int DownloadFile(const char* pcHost, int iPort, const char* pcUrl, const char* pcToFile,int iBeginPos = 0,int iSize = 0);
	int HttpPostMsg(const char* pcHost,std::vector<std::string> rVParamName,std::vector<std::string> rVkParamStr, const std::string& rkFileParamName, const std::string& rkFileName, const std::string& rkFileContentType);

protected:
	void*	m_pkData;
};

// UDP���������(�ɿ���UDP����)
class TeUdpMgr
{
public:
	TeUdpMgr(void);
	~TeUdpMgr(void);

	bool Initialize(TeNetCall* pkNetCall);
	void Update(void);
	void Finialize(void);

	bool Listen(int iPort, int iMaxCount = 2 * 1024,int iBufferSize = 8 * 1024);
	bool Connect(const char* pcIP, int iPort, int iTimeOut, int iBufferSize);
	bool SendData(int iConnect, void* pcData, int iSize, int iFlag = UDP_PACKET_FLAG_RELIABLE);
	void Disconnect(int iConnect);

protected:
	void*	m_pkData;
};