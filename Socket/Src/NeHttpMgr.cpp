#include "NeHttpMgr.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //--------------------------------------- Class NeMyGetHttp------------------------------
void NeMyHttp::OnDataComplete()
{
	if (m_pkNetMgr)
	{
		NePostDataHttp kPost;
		const std::string& rkState = GetStatus();

		if (rkState == "200" || rkState == "206")
		{
			// 文件下载成功
			kPost.eType = THR_GET_SUCCESS;
		}
		else
		{
			kPost.eType = THR_GET_HTTP_ERROR;
			kPost.iCode = atoi(rkState.c_str());
		}
		
		kPost.iKey = m_iKey;
		kPost.kHost = m_kHost;
		kPost.kUrl = m_kUrl;
		m_pkNetMgr->PushPost(kPost);
	}
}

void NeMyHttp::OnConnectFailed()
{
	if (m_pkNetMgr)
	{
		NePostDataHttp kPost;
		kPost.eType = THR_GET_CONNECT_ERROR;
		kPost.iKey = m_iKey;
		kPost.kHost = m_kHost;
		kPost.kUrl = m_kUrl;
		kPost.kMsg = "Connect To ServerFailed\n";
		m_pkNetMgr->PushPost(kPost);
	}
}

/*
	重载TcpSocket中的SetLineProtocol 的函数， HTTPSOCKET理论上不需要用到iBuf缓存，但是TCPSOCKET函数在设置SetLineProtocol FALSE的时候 模块有调用 DisableInputBuffer(x) 打开了缓存
	在HTTP SOCKET接收完HTTP的头部信息后 设置SetLineProtocol的时候 打开缓存，但是HTTPSOCKET并没有设置ibuf初始化 这种情况下会会掉写入缓存数据错误
*/
void NeMyHttp::SetLineProtocol(bool x)
{
	StreamSocket::SetLineProtocol(x);	
}

// --------------------------------------- Class NeMyGetHttp------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------- Class NeMyPostHttp------------------------------
void NeMyPostHttp::OnConnectFailed()
{
	if (m_pkNetMgr)
	{
		NePostDataHttp kPost;		
		kPost.eType = THR_POST_CONNECT_ERROR;
		kPost.iKey = m_iKey;
		kPost.kHost = m_kHost;
		kPost.kUrl = m_kUrl;
		kPost.kMsg = "Connect To ServerFailed\n";
		m_pkNetMgr->PushPost(kPost);
	}
}

void NeMyPostHttp::OnDataComplete()
{
	if (m_pkNetMgr)
	{
		NePostDataHttp kPost;
		
		const std::string& rkState = GetStatus();

		if (rkState == "200")
		{
			// 文件上传成功
			kPost.eType = THR_POST_SUCCESS;		
			//if (GetDataPtr() != nullptr)
			if (GetDataPtr() != NULL)
			{
				kPost.kBody = (char*)GetDataPtr();
			}			
		}
		else
		{
			kPost.eType = THR_POST_HTTP_ERROR;
			kPost.iCode = atoi(rkState.c_str());
		}

		kPost.iKey = m_iKey;
		kPost.kHost = m_kHost;
		kPost.kUrl = m_kUrl;
		m_pkNetMgr->PushPost(kPost);
	}
}

/*
重载TcpSocket中的SetLineProtocol 的函数， HTTPSOCKET理论上不需要用到iBuf缓存，但是TCPSOCKET函数在设置SetLineProtocol FALSE的时候 模块有调用 DisableInputBuffer(x) 打开了缓存
在HTTP SOCKET接收完HTTP的头部信息后 设置SetLineProtocol的时候 打开缓存，但是HTTPSOCKET并没有设置ibuf初始化 这种情况下会会掉写入缓存数据错误
*/
void NeMyPostHttp::SetLineProtocol(bool x)
{
	StreamSocket::SetLineProtocol(x);
}
// --------------------------------------- Class NeMyPostHttp------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------------------------------- Class NeLogError------------------------------

void NeLogError::error(ISocketHandler* pkHandle, Socket* pkSock, const std::string& user_text, int err, const std::string& sys_err, loglevel_t /* = LOG_LEVEL_WARNING */)
{
	NeHttpMgr* pkNetMgr = NULL;
	NePostDataHttp kPost;
	kPost.kMsg = user_text + "---ERR---" + sys_err;	

	if (m_iSockType == 0)
	{
		NeMyHttp* pkMyHttp = (NeMyHttp*)pkSock;
		if (!pkMyHttp && pkMyHttp->GetNetHttpMgr() != NULL)
		{
			return;
		}

		kPost.iKey = pkMyHttp->GetKey();
		kPost.kHost = pkMyHttp->GetHost();
		kPost.kUrl = pkMyHttp->GetUrl();
		pkNetMgr = pkMyHttp->GetNetHttpMgr();
	}
	else if(m_iSockType == 1)
	{
		NeMyPostHttp* pkMyHttp = (NeMyPostHttp*)pkSock;
		if (!pkMyHttp && pkMyHttp->GetNetHttpMgr() != NULL)
		{
			return;
		}

		kPost.iKey = pkMyHttp->GetKey();
		kPost.kHost = pkMyHttp->GetHost();
		kPost.kUrl = pkMyHttp->GetUrl();
		pkNetMgr = pkMyHttp->GetNetHttpMgr();
	}
	else
	{
		return;
	}

	// 根据LOG等级进行分类
	if(err == LOG_LEVEL_ERROR || err == LOG_LEVEL_FATAL)
	{
		// 这2种错误可以忽略
		kPost.eType = THR_LOG_INFO_OR_WARNING;
	}
	else
	{
		if (m_iSockType == 0)
		{
			kPost.eType = THR_GET_OTHER_ERROR;
		}
		else
		{
			kPost.eType = THR_POST_OTHER_ERROR;
		}
	}

	// 以下是特定的文本或者函数 定义的固定错误
	if (user_text == "AddFile")
	{
		kPost.eType = THR_POST_OTHER_ERROR;
		kPost.iCode = 1;
	}

	if(user_text == "HttpGetSocket" && sys_err == "connect() failed miserably")
	{
		kPost.eType = THR_GET_OTHER_ERROR;
		kPost.iCode = 1;
	}

	if(user_text == "OnHeaderComplete" && sys_err == "GetSocket tofile open file error")
	{
		kPost.eType = THR_GET_OTHER_ERROR;
		kPost.iCode = 2;
	}
	// 以上是特定的文本或者函数 定义的固定错误

	// PUSH 消息
	if (pkNetMgr)
	{
		pkNetMgr->PushPost(kPost);
	}
}

// --------------------------------------- Class NeLogError------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------------------------------- Class NeHttpMgr------------------------------
NeHttpMgr::NeHttpMgr(void)
{
	RegStdLog(&m_kLog);
	m_lGenID = 3;
}

NeHttpMgr::~NeHttpMgr(void)
{

}

bool NeHttpMgr::Initialize(TeNetCall* pkNetCall, int iMaxThread)
{
	m_pkNetCall = pkNetCall;

	SetNumberOfThreads(iMaxThread);

	return true;
}

void NeHttpMgr::Update(void)
{
	while (true)
	{
		if (m_kPostData.empty())
		{
			break;
		}

		if (!m_pkNetCall)
		{
			break;
		}
		m_kPostMutex.Lock();
		NePostDataHttp kData = m_kPostData.front();
		m_kPostData.pop();
		m_kPostMutex.Unlock();
				
		m_pkNetCall->OnHttpResult(kData.eType,kData.iKey,kData.iCode, kData.kHost.c_str(), kData.kUrl.c_str(),kData.kBody.c_str(),kData.kMsg.c_str());
	}
}

void NeHttpMgr::Finialize(void)
{

}

int NeHttpMgr::DownloadFile(const char* pcHost, int iPort, const char* pcUrl, const char* pcToFile, int iBeginPos, int iSize)
{
	if(!pcHost || !pcUrl || !pcToFile)
	{
		return -1;
	}
	
	m_kLog.SetSockType(0);

	ISocketHandler& rkScoketHandler = GetRandomHandler();
	if(!rkScoketHandler.IsRegStdLog())
	{
		rkScoketHandler.RegStdLog(&m_kLog);
	}
	
	int iKey = GenKey();
	NeMyHttp* pkSock = new NeMyHttp(rkScoketHandler, this, iKey,pcHost, iPort, pcUrl, pcToFile, iBeginPos, iSize);
	if (!pkSock)
	{
		NePostDataHttp kPost;
		kPost.eType = THR_GET_OTHER_ERROR;
		kPost.iCode = 0;
		kPost.iKey = iKey;
		kPost.kHost = pcHost;
		kPost.kUrl = pcUrl;		
		PushPost(kPost);

		delete pkSock;
		return iKey;
	}

	// 设置由handler自动销毁
	pkSock->SetDeleteByHandler(true);

	rkScoketHandler.Add(pkSock);

	return iKey;
}

int NeHttpMgr::HttpPostMsg(const char* pcHost, std::vector<std::string> rVParamName, std::vector<std::string> rVkParamStr, const std::string& rkFileParamName, const std::string& rkFileName, const std::string& rkFileContentType)
{
	if (!pcHost)
	{
		return -1;
	}

	m_kLog.SetSockType(1);

	ISocketHandler& rkScoketHandler = GetRandomHandler();
	if (!rkScoketHandler.IsRegStdLog())
	{
		rkScoketHandler.RegStdLog(&m_kLog);
	}

	int iKey = GenKey();
	NeMyPostHttp* pkSock = new NeMyPostHttp(rkScoketHandler, this, iKey,pcHost);
	if (!pkSock)
	{
		NePostDataHttp kPost;
		kPost.eType = THR_POST_OTHER_ERROR;
		kPost.iCode = 0;
		kPost.iKey = iKey;
		kPost.kHost = pcHost;
		PushPost(kPost);

		delete pkSock;
		return iKey;
	}

	// 
	if (!rVParamName.empty() && rVParamName.size() == rVkParamStr.size())
	{
		for (int i = 0; i < (int)rVParamName.size();++i)
		{
			pkSock->AddField(rVParamName[i], rVkParamStr[i]);
		}		
	}

	//
	if(!rkFileName.empty())
	{
		pkSock->AddFile(rkFileParamName, rkFileName, rkFileContentType);
	}	

	// 设置由handler自动销毁
	pkSock->SetDeleteByHandler(true);

	// 
	pkSock->Open();

	rkScoketHandler.Add(pkSock);
	return iKey;
}

void NeHttpMgr::PushPost(NePostDataHttp& rkData)
{
	m_kPostMutex.Lock();
	m_kPostData.push(rkData);
	m_kPostMutex.Unlock();
}

int NeHttpMgr::GenKey()
{
	return m_lGenID++;
}

// --------------------------------------- Class NeHttpMgr------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
