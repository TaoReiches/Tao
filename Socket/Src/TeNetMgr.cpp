#if defined WIN32
#include "NeIOCPMgr.h"
#elif defined __LINUX__
#include "Include/Linux.h"
#include "NeEpollMgr.h"
#else
#endif
#include "TeNetMgr.h"

#if defined WIN32
#define DATA_CONFERENCE()	NeIOCPMgr& rkSockMgr = *((NeIOCPMgr *)m_pkData)
#elif defined __LINUX__
#define DATA_CONFERENCE()	NeEpollMgr& rkSockMgr = *((NeEpollMgr *)m_pkData)
#else
#endif

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

bool TeNetMgr::Connect(const char* pcIP, int iPort, int iTimeOut, int iBufferSize, TeSockFlag iFlag)
{
#if defined WIN32 || defined __LINUX__
	DATA_CONFERENCE();

	return rkSockMgr.Connect(pcIP, iPort, iTimeOut, iBufferSize, iFlag);
#else
	return false;
#endif
}

bool TeNetMgr::Listen(int iListenPort, int iListenNum, int iBufferSize, TeSockFlag iFlag)
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
	if (eRet != TeSendResult::TSR_SUCCESS)
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
		return TeSendResult::TSR_PARAM_ERROR;
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
