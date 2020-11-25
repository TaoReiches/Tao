#pragma once
#include "TeInclude.h"
#include "TeNetMgr.h"
#include "C++Sockets/SocketHandler.h"
#include "C++Sockets/StdoutLog.h"
#include "C++Sockets/HttpGetSocket.h"
#include "C++Sockets/HttpPostSocket.h"

class NeHttpMgr;
// 投递数据
struct NePostDataHttp
{
	NePostDataHttp()
	{
		iKey = -1;
		eType = THR_NULL;
		iCode = 0;
		kHost = "";
		kUrl = "";
		kMsg = "";
		kBody = "";
	}

	int				iKey;
	TeHttpResult	eType;
	int				iCode;
	std::string		kHost;
	std::string		kUrl;
	std::string		kMsg;
	std::string		kBody;
};
typedef std::queue<NePostDataHttp> QPostDataHttp;

class NeMyHttp : public HttpGetSocket
{
public:
	NeMyHttp(ISocketHandler& h, NeHttpMgr* pkNetMgr,int iKey, const char* pcHost, int iPort, const char* pcUrl, const char* pcToFile, int iBeginPos, int iSize) : HttpGetSocket(h, pcHost, iPort, pcUrl, pcToFile, true, iBeginPos, iSize), m_pkNetMgr(pkNetMgr), m_iKey(iKey),m_kHost(pcHost), m_kUrl(pcUrl)
	{
	}

	void OnDataComplete();
	void OnConnectFailed();
	void SetLineProtocol(bool = true);

	const std::string& GetUrl()	{ return m_kUrl; }
	const std::string& GetHost() { return m_kHost; }
	int GetKey() { return m_iKey; }
	NeHttpMgr* GetNetHttpMgr() { return m_pkNetMgr; }


protected:
	NeHttpMgr*		m_pkNetMgr;
	std::string		m_kUrl;
	std::string		m_kHost;
	int				m_iKey;
};

class NeMyPostHttp : public HttpPostSocket
{
public:
	NeMyPostHttp(ISocketHandler& rkHandle, NeHttpMgr* pkNetMgr,int iKey, const char* pcHost) : HttpPostSocket(rkHandle, pcHost), m_pkNetMgr(pkNetMgr),m_iKey(iKey),m_kHost(pcHost), m_kUrl("")
	{

	}

	const std::string& GetUrl()	{ return m_kUrl; }
	const std::string& GetHost() { return m_kHost; }
	int GetKey() { return m_iKey; }
	NeHttpMgr* GetNetHttpMgr() { return m_pkNetMgr; }
	void SetLineProtocol(bool = true);
	void OnConnectFailed();
	void OnDataComplete();

protected:
	NeHttpMgr*		m_pkNetMgr;
	std::string		m_kUrl;
	std::string		m_kHost;
	int				m_iKey;
};

// 日志输出信息
class NeLogError : public StdoutLog
{
public:
	NeLogError()
	{
		
	}
	~NeLogError()
	{

	}

	void SetSockType(int iType)
	{
		m_iSockType = iType;
	}

	void error(ISocketHandler* pkHandle, Socket* pkSock, const std::string& user_text, int err, const std::string& sys_err, loglevel_t = LOG_LEVEL_WARNING);

protected:
	int		m_iSockType;
};

// 网络HANDLE类
class NeHttpMgr : public SocketHandler
{
	friend class NeMyHttp; 
public:
	NeHttpMgr(void);
	~NeHttpMgr(void);

	bool Initialize(TeNetCall* pkNetCall, int iMaxThread = 2);
	void Update(void);
	void Finialize(void);

	int DownloadFile(const char* pcHost, int iPort,const char* pcUrl, const char* pcToFile,int iBeginPos,int iSize);
	int HttpPostMsg(const char* pcHost, std::vector<std::string> rVParamName, std::vector<std::string> rVkParamStr, const std::string& rkFileParamName, const std::string& rkFileName, const std::string& rkFileContentType);

	// 放入消息队列
	void PushPost(NePostDataHttp& rkData);
	
	int GenKey();
protected:
	volatile long						m_lGenID;
	TeNetCall*							m_pkNetCall;
	NeLogError							m_kLog;

	Mutex								m_kPostMutex;
	QPostDataHttp						m_kPostData;
};