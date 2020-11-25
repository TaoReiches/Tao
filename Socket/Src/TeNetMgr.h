#pragma once
#include "stdio.h"
#include <iostream>
#include <vector>

// 自定义Sock句柄
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

// 网络断开的原因
enum TeDisconnectCode
{
	TDC_SELF_DISC	= 0	,	// 主动调用的断开
	TDC_SYSTEM_ERROR	,	// 系统底层返回该连接失败
	TDC_RECV_ERROR1		,	// 接收失败1
	TDC_RECV_ERROR2		,	// 接收失败2
	TDC_SEND_ERROR1		,	// 发送失败1
	TDC_SEND_ERROR2		,	// 发送失败2
};

// HTTP 下载结果
enum TeHttpResult
{
	THR_NULL = 0,
	// GET
	THR_GET_SUCCESS = 1,				// 唯一的 HTTP下载成功标识
	THR_GET_HTTP_ERROR = 2,				// HOST 存在HTTP服务器 HTTP请求失败  具体编码参照 百度 HTTP STATUS
	THR_GET_CONNECT_ERROR = 3,			// HOST 连接就失败了
	THR_GET_OTHER_ERROR = 4,			// 其他错误 iParam:0 本地SOCKET创建失败 1:网络连接失败 2:打开保存的文件失败(可能是文件夹未创建或者可写路径错误)

	// POST
	THR_POST_SUCCESS = 100,				// POST成功
	THR_POST_HTTP_ERROR = 101,			// HOST 存在HTTP服务器 HTTP请求失败  具体编码参照 百度 HTTP STATUS
	THR_POST_CONNECT_ERROR = 102,		// HOST 连接就失败了
	THR_POST_OTHER_ERROR = 103,			// 其他错误 iParam:0 本地SOCKET创建失败 1:POST文件打开失败

	// 以下错误类型 其中 ERROR和
	THR_LOG_INFO_OR_WARNING = 10000,	// 返回的信息或者警告 可以忽略
};

// 回调使用
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

// 发送结果
enum TeSendResult
{
	TSR_SUCCESS		= 0	,	// 发送成功
	TSR_NET_ERROR		,	// 网络错误(一般是已经断开了)
	TSR_NET_BUSY		,	// 网络繁忙(一般是发送缓冲满了)
	TSR_NET_CLOSED		,	// 网络已经关闭
	TSR_PARAM_ERROR		,	// 参数错误
};

// 连接Socket的属性
enum TeSockFlag
{
	TSF_16BIT_WIN	= (1<<0),	// 16位长度的win系统
	TSF_32BIT_WIN	= (1<<1),	// 32位长度的win系统
	TSF_16BIT_LINUX	= (1<<2),	// 16位长度的linux系统
	TSF_32BIT_LINUX	= (1<<3),	// 32位长度的linux系统
	TSF_NO_DELAY	= (1<<4),	// 发送数据不进行延时
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

// TCP网络管理类(适用于WINDOWS UNIX LINUX 较稳定已验证 常用于服务器)
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

// 跨平台TCP网络管理类(基于C++Socket 有较强的跨平台通用性 实现方式区别于TeNetMgr的2种包含)
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

// HTTP网络管理类(基于C++Socket 较强跨平台以及扩展性)
class TeMulitPlatHttMgr
{
public:
	TeMulitPlatHttMgr(void);
	~TeMulitPlatHttMgr(void);

	bool Initialize(TeNetCall* pkCall);
	void Update(void);
	void Finialize(void);

	// 下载文件到指定目录
	int DownloadFile(const char* pcHost, int iPort, const char* pcUrl, const char* pcToFile,int iBeginPos = 0,int iSize = 0);
	int HttpPostMsg(const char* pcHost,std::vector<std::string> rVParamName,std::vector<std::string> rVkParamStr, const std::string& rkFileParamName, const std::string& rkFileName, const std::string& rkFileContentType);

protected:
	void*	m_pkData;
};

// UDP网络管理类(可靠的UDP传输)
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