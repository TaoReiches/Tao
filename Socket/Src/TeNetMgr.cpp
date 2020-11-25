#if defined WIN32
#include "NeIOCPMgr.h"
#elif defined __LINUX__
#include "Include/Linux.h"
#include "NeEpollMgr.h"
#else
#endif
#include "TeNetMgr.h"
#include "NeMulitTcpMgr.h"
#include "NeHttpMgr.h"
#include "NeUdpMgr.h"

#if defined WIN32
#define DATA_CONFERENCE()	NeIOCPMgr& rkSockMgr = *((NeIOCPMgr *)m_pkData)
#elif defined __LINUX__
#define DATA_CONFERENCE()	NeEpollMgr& rkSockMgr = *((NeEpollMgr *)m_pkData)
#else
#endif

#define DATA_MULITCONFERENCE()		NeMulitTcpMgr& rkSockMgr = *((NeMulitTcpMgr *)m_pkData)
#define DATA_HTTPCONFERENCE()		NeHttpMgr& rkHttpMgr = *((NeHttpMgr *)m_pkData)
#define DATA_UDPCONFERENCE()		NeUdpMgr& rkUdpMgr = *((NeUdpMgr *)m_pkData)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------------------------------- Class TeNetMgr------------------------------

TeNetMgr::TeNetMgr(void)
{
#if defined WIN32
	m_pkData = new NeIOCPMgr;
#elif defined __LINUX__
	m_pkData = new NeEpollMgr;
#else
#endif
}

TeNetMgr::~TeNetMgr(void)
{
#if defined WIN32
	NeIOCPMgr* pkData = (NeIOCPMgr *)m_pkData;
	SAFE_DELETE(pkData);
#elif defined __LINUX__
	NeEpollMgr* pkData = (NeEpollMgr* )m_pkData;
#else
#endif
}

bool TeNetMgr::Initialize(TeNetCall* pkCall, int iMaxThread)
{
#if defined WIN32 || defined __LINUX__
	DATA_CONFERENCE();

	return rkSockMgr.Initialize(pkCall, iMaxThread);
#else
	return false;
#endif
}

void TeNetMgr::Update(void)
{
#if defined WIN32 || defined __LINUX__
	DATA_CONFERENCE();

	rkSockMgr.Update();
#endif
}

void TeNetMgr::Finialize(void)
{
#if defined WIN32 || defined __LINUX__
	DATA_CONFERENCE();

	rkSockMgr.Finialize();
#endif
}

bool TeNetMgr::Connect(const char* pcIP, int iPort, int iTimeOut, int iBufferSize, int iFlag)
{
#if defined WIN32 || defined __LINUX__
	DATA_CONFERENCE();

	return rkSockMgr.Connect(pcIP, iPort, iTimeOut, iBufferSize, iFlag);
#else
	return false;
#endif
}

bool TeNetMgr::Listen(int iListenPort, int iListenNum, int iBufferSize, int iFlag)
{
#if defined WIN32 || defined __LINUX__
	DATA_CONFERENCE();

	return rkSockMgr.Listen(iListenPort, iListenNum, iBufferSize, iFlag);
#else
	return false;
#endif
}

bool TeNetMgr::SendData(const HSock& rkSock, void* pkData, int iSize)
{
#if defined WIN32 || defined __LINUX__
	DATA_CONFERENCE();

	if (rkSock.IsInvalid() || pkData == NULL || iSize <= 0)
	{
		return false;
	}

	TeSendResult eRet = rkSockMgr.SendData(*((NeHSock *)&rkSock), (char *)pkData, iSize);
	if (eRet != TSR_SUCCESS)
	{
		return false;
	}
	return true;
#else
	return false;
#endif
}

TeSendResult TeNetMgr::SendDataDetail(const HSock& rkSock, void* pkData, int iSize)
{
#if defined WIN32 || defined __LINUX__
	DATA_CONFERENCE();

	if (rkSock.IsInvalid() || pkData == NULL || iSize <= 0)
	{
		return TSR_PARAM_ERROR;
	}

	return rkSockMgr.SendData(*((NeHSock *)&rkSock), (char *)pkData, iSize);
#else
	return TSR_SUCCESS;
#endif
}

void TeNetMgr::Disconnect(const HSock& rkSock)
{
#if defined WIN32 || defined __LINUX__
	DATA_CONFERENCE();

	if (rkSock.IsInvalid())
	{
		return;
	}

	rkSockMgr.Disconnect(*((NeHSock *)&rkSock));
#endif
}

// --------------------------------------- Class TeNetMgr------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------------------------------- Class TeMulitPlatNetMgr------------------------------

TeMulitPlatNetMgr::TeMulitPlatNetMgr(void)
{
	m_pkData = new NeMulitTcpMgr;
}

TeMulitPlatNetMgr::~TeMulitPlatNetMgr(void)
{
	NeMulitTcpMgr* pkData = (NeMulitTcpMgr *)m_pkData;
	SAFE_DELETE(pkData);
}

bool TeMulitPlatNetMgr::Initialize(TeNetCall* pkCall)
{
	DATA_MULITCONFERENCE();

	return rkSockMgr.Initialize(pkCall);
}

void TeMulitPlatNetMgr::Update(void)
{
	DATA_MULITCONFERENCE();

	rkSockMgr.Update();
}

void TeMulitPlatNetMgr::Finialize(void)
{
	DATA_MULITCONFERENCE();

	rkSockMgr.Finialize();
}

bool TeMulitPlatNetMgr::Connect(const char* pcIP, int iPort, int iTimeOut, int iBufferSize, int iFlag)
{
	DATA_MULITCONFERENCE();

	return rkSockMgr.Connect(pcIP, iPort, iTimeOut, iBufferSize, iFlag);
}

bool TeMulitPlatNetMgr::Listen(int iListenPort, int iListenNum, int iBufferSize, int iFlag)
{
	DATA_MULITCONFERENCE();

	return rkSockMgr.Listen(iListenPort, iListenNum, iBufferSize, iFlag);
}

bool TeMulitPlatNetMgr::SendData(const HSock& rkSock, void* pkData, int iSize)
{
	DATA_MULITCONFERENCE();

	if (rkSock.IsInvalid() || pkData == NULL || iSize <= 0)
	{
		return false;
	}

	TeSendResult eRet = rkSockMgr.SendData(*((NeHMulitSock *)&rkSock), (char *)pkData, iSize);
	if (eRet != TSR_SUCCESS)
	{
		return false;
	}
	return true;
}

TeSendResult TeMulitPlatNetMgr::SendDataDetail(const HSock& rkSock, void* pkData, int iSize)
{
	DATA_MULITCONFERENCE();

	if (rkSock.IsInvalid() || pkData == NULL || iSize <= 0)
	{
		return TSR_PARAM_ERROR;
	}

	return rkSockMgr.SendData(*((NeHMulitSock *)&rkSock), (char *)pkData, iSize);
}

void TeMulitPlatNetMgr::Disconnect(const HSock& rkSock)
{
	DATA_MULITCONFERENCE();

	if (rkSock.IsInvalid())
	{
		return;
	}

	rkSockMgr.Disconnect(*((NeHMulitSock *)&rkSock));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------------------------------- Class TeMulitPlatHttMgr------------------------------

TeMulitPlatHttMgr::TeMulitPlatHttMgr(void)
{
	m_pkData = new NeHttpMgr();
}

TeMulitPlatHttMgr::~TeMulitPlatHttMgr(void)
{
	NeHttpMgr* pkData = (NeHttpMgr *)m_pkData;
	SAFE_DELETE(pkData);
}

bool TeMulitPlatHttMgr::Initialize(TeNetCall* pkCall)
{
	DATA_HTTPCONFERENCE();

	if (!rkHttpMgr.Initialize(pkCall))
	{
		return false;
	}

	return true;
}

void TeMulitPlatHttMgr::Update(void)
{
	DATA_HTTPCONFERENCE();

	rkHttpMgr.Update();
}

void TeMulitPlatHttMgr::Finialize(void)
{
	DATA_HTTPCONFERENCE();

	rkHttpMgr.Finialize();
}

int TeMulitPlatHttMgr::DownloadFile(const char* pcHost, int iPort, const char* pcUrl, const char* pcToFile, int iBeginPos, int iSize)
{
	DATA_HTTPCONFERENCE();

	return rkHttpMgr.DownloadFile(pcHost, iPort, pcUrl, pcToFile, iBeginPos, iSize);
}

int TeMulitPlatHttMgr::HttpPostMsg(const char* pcHost, std::vector<std::string> rVParamName, std::vector<std::string> rVkParamStr, const std::string& rkFileParamName, const std::string& rkFileName, const std::string& rkFileContentType)
{
	DATA_HTTPCONFERENCE();

	return rkHttpMgr.HttpPostMsg(pcHost, rVParamName, rVkParamStr, rkFileParamName, rkFileName, rkFileContentType);
}

// --------------------------------------- Class TeMulitPlatHttMgr------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------------------------------- Class TeUdpMgr------------------------------
TeUdpMgr::TeUdpMgr(void)
{
	m_pkData = new NeUdpMgr();
}

TeUdpMgr::~TeUdpMgr(void)
{
	NeUdpMgr* pkData = (NeUdpMgr *)m_pkData;
	SAFE_DELETE(pkData);
}

bool TeUdpMgr::Initialize(TeNetCall* pkNetCall)
{
	DATA_UDPCONFERENCE();

	if (!rkUdpMgr.Initialize(pkNetCall))
	{
		return false;
	}

	return true;
}

void TeUdpMgr::Update(void)
{
	DATA_UDPCONFERENCE();

	rkUdpMgr.Update();
}

void TeUdpMgr::Finialize(void)
{
	DATA_UDPCONFERENCE();

	rkUdpMgr.Finialize();
}

bool TeUdpMgr::Listen(int iPort,int iMaxCount, int iBufferSize)
{
	DATA_UDPCONFERENCE();
	
	if(!rkUdpMgr.Listen(iPort,iMaxCount,iBufferSize))
	{
		return false;
	}
	
	return true;
}

bool TeUdpMgr::Connect(const char* pcIP, int iPort, int iTimeOut, int iBufferSize)
{
	DATA_UDPCONFERENCE();

	if (!rkUdpMgr.Connect(pcIP, iPort,iTimeOut,iBufferSize))
	{
		return false;
	}

	return true;
}

bool TeUdpMgr::SendData(int iConnect, void* pcData, int iSize,int iFlag)
{
	DATA_UDPCONFERENCE();

	if (!rkUdpMgr.SendData(iConnect, (char *)pcData, iSize, iFlag))
	{
		return false;
	}

	return true;
}

void TeUdpMgr::Disconnect(int iConnect)
{
	DATA_UDPCONFERENCE();

	rkUdpMgr.Disconnect(iConnect);

	return;
}

// --------------------------------------- Class TeUdpMgr------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
