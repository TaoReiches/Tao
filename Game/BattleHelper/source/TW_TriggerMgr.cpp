/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TriggerMgr.h"
#include "TW_TriggerMgrDef.h"
#include "TW_Define.h"

#define DATA_CONFERENCE() TePtTriggerMgrData& rkData = *((TePtTriggerMgrData *)m_pkData)

TePtTriggerMgr::TePtTriggerMgr(void* pkParam)
{
	m_pkData = new TePtTriggerMgrData;
	DATA_CONFERENCE();

	rkData.m_pkParam = pkParam;

	rkData.m_iGenID = 0;
	rkData.m_uiTimeNow = 0;
	rkData.m_pkCurTrigger = NULL;

	rkData.m_iMaxEvent = 0;
	rkData.m_akEventTrgs = NULL;
}

TePtTriggerMgr::~TePtTriggerMgr(void)
{
	Finialize();

	TePtTriggerMgrData* pkData = (TePtTriggerMgrData*)m_pkData;
	delete pkData;
	m_pkData = NULL;
}

bool TePtTriggerMgr::Initialize(int iMaxEvent)
{
	DATA_CONFERENCE();

	rkData.m_iGenID = 0;
	rkData.m_uiTimeNow = 0;
	rkData.m_pkCurTrigger = NULL;

	rkData.m_iMaxEvent = iMaxEvent;
	rkData.m_akEventTrgs = new PtListTrgInfo[iMaxEvent];

	rkData.m_kTriggers.clear();

	return true;
}

void TePtTriggerMgr::Update(int iDeltaTime)
{
	DATA_CONFERENCE();

	rkData.m_uiTimeNow += iDeltaTime;

	for (std::map<int, TePtTrigger*>::iterator kIter = rkData.m_kTriggers.begin(); kIter != rkData.m_kTriggers.end();)
	{
		TePtTrigger* pkTrigger = kIter->second;
		if (pkTrigger->m_eState == PTTS_OVER)
		{
			SAFE_DELETE(pkTrigger);
			std::map<int, TePtTrigger*>::iterator itr1 = kIter;
			++kIter;
			rkData.m_kTriggers.erase(itr1);
			continue;
		}

		rkData.m_pkCurTrigger = pkTrigger;
		rkData.m_pkCurParam = &(pkTrigger->m_kParam);

		pkTrigger->Execute(rkData.m_uiTimeNow);

		rkData.m_pkCurTrigger = NULL;
		rkData.m_pkCurParam = NULL;

		++kIter;
	}
}

void TePtTriggerMgr::Finialize(void)
{
	DATA_CONFERENCE();

	for (std::map<int, TePtTrigger*>::iterator kIter = rkData.m_kTriggers.begin(); kIter != rkData.m_kTriggers.end(); ++kIter)
	{
		TePtTrigger* pkTrigger = kIter->second;

		SAFE_DELETE(pkTrigger);
	}
	rkData.m_kTriggers.clear();

	rkData.m_iGenID = 0;
	rkData.m_uiTimeNow = 0;
	rkData.m_pkCurTrigger = NULL;

	rkData.m_iMaxEvent = 0;
	SAFE_ARRAY_DELETE(rkData.m_akEventTrgs);
}

unsigned int TePtTriggerMgr::GetTimeNow(void)
{
	DATA_CONFERENCE();

	return rkData.m_uiTimeNow;
}

int TePtTriggerMgr::GetTriggerID(void)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurTrigger)
	{
		return rkData.m_pkCurTrigger->m_iID;
	}
	return -1;
}

bool TePtTriggerMgr::HasParam(int iParamID)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurParam)
	{
		return rkData.m_pkCurParam->HasParam(iParamID);
	}
	return false;
}

int TePtTriggerMgr::GetParamInt(int iParamID, int iDefault)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurParam)
	{
		return rkData.m_pkCurParam->GetParamInt(iParamID, iDefault);
	}
	return iDefault;
}

float TePtTriggerMgr::GetParamFloat(int iParamID, float fDefault)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurParam)
	{
		return rkData.m_pkCurParam->GetParamFloat(iParamID, fDefault);
	}
	return fDefault;
}

void* TePtTriggerMgr::GetParamVoid(int iParamID)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurParam)
	{
		return rkData.m_pkCurParam->GetParamVoid(iParamID);
	}
	return NULL;
}

void    TePtTriggerMgr::OnUnitDelete(void* pkUnit)
{
	DATA_CONFERENCE();

	for (auto kIter = rkData.m_kTriggers.begin(); kIter != rkData.m_kTriggers.end(); ++kIter)
	{
		TePtTrigger* pkTrigger = kIter->second;

		if (pkTrigger)
		{
			pkTrigger->OnDeleteUnit(pkUnit);
		}
	}
}

void	TePtTriggerMgr::TrgWait(int iTime)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurTrigger)
	{
		rkData.m_pkCurTrigger->WaitToTime(rkData.m_uiTimeNow + iTime);
	}
}

void	TePtTriggerMgr::Return(void)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurTrigger)
	{
		rkData.m_pkCurTrigger->Return();
	}
}

void	TePtTriggerMgr::Enable(void)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurTrigger)
	{
		rkData.m_pkCurTrigger->Enable();
	}
}

void	TePtTriggerMgr::Disable(void)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurTrigger)
	{
		rkData.m_pkCurTrigger->Disable();
	}
}

void	TePtTriggerMgr::Finish(void)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurTrigger)
	{
		rkData.m_pkCurTrigger->Finish();
	}
}

void	TePtTriggerMgr::FireTrigger(int iEvent, TePtParam& rkParam)
{
	DATA_CONFERENCE();

	if (iEvent < 0 || iEvent >= rkData.m_iMaxEvent)
	{
		return;
	}

	PtListTrgInfo& kList = rkData.m_akEventTrgs[iEvent];
	if (kList.size() <= 0)
	{
		return;
	}

	for (PtListTrgInfo::iterator kIter = kList.begin(); kIter != kList.end(); ++kIter)
	{
		TePtTriggerInfo& kInfo = *kIter;

		if (kInfo.bDisabled)
		{
			continue;
		}

		if (kInfo.iTrgCount >= 24)
		{
			throw 0;
			return;
		}

		TePtParam* pkBackupParam = rkData.m_pkCurParam;
		rkData.m_pkCurParam = &rkParam;
		if (kInfo.pCondition)
		{
			TePtCondResult eRet = kInfo.pCondition(rkData.m_pkParam);
			if (eRet != PTCR_OK)
			{
				rkData.m_pkCurParam = pkBackupParam;
				continue;
			}
		}

		TePtTrigger kTempTrg;
		kTempTrg.m_iID = ++(rkData.m_iGenID);
		kTempTrg.m_pkInfo = &kInfo;
		kTempTrg.m_pkParam = rkData.m_pkParam;
		kTempTrg.m_pAction = kInfo.pAction;

		TePtTrigger* pkBackupTrg = rkData.m_pkCurTrigger;
		rkData.m_pkCurTrigger = &kTempTrg;

		kInfo.iTrgCount++;

		kTempTrg.Execute(rkData.m_uiTimeNow);

		kInfo.iTrgCount--;

		rkData.m_pkCurTrigger = pkBackupTrg;
		rkData.m_pkCurParam = pkBackupParam;

		if (kTempTrg.m_eState == PTTS_WAIT)
		{
			TePtTrigger* pkTrigger = new TePtTrigger();
			*pkTrigger = kTempTrg;
			pkTrigger->m_kParam = rkParam;

			rkData.m_kTriggers[pkTrigger->m_iID] = pkTrigger;
		}
	}
}

bool	TePtTriggerMgr::FireTriggerBegin(int iEvent, TePtParam& rkParam, TePtParam*& pkBackupParam)
{
	DATA_CONFERENCE();
	if (iEvent < 0 || iEvent >= rkData.m_iMaxEvent)
	{
		return false;
	}
	pkBackupParam = rkData.m_pkCurParam;
	rkData.m_pkCurParam = &rkParam;
	return true;
}

void	TePtTriggerMgr::FireTriggerEnd(TePtParam* pkBackupParam)
{
	DATA_CONFERENCE();
	rkData.m_pkCurParam = pkBackupParam;
}

bool	TePtTriggerMgr::WaitFinish(void)
{
	DATA_CONFERENCE();
	if (rkData.m_pkCurTrigger)
	{
		return rkData.m_pkCurTrigger->WaitFinish();
	}

	return false;
}

void	TePtTriggerMgr::RegisterEventTrg(int iEvent, PtActionFun pAction, PtConditionFun pCondition)
{
	DATA_CONFERENCE();

	if (!pAction || iEvent < 0 || iEvent >= rkData.m_iMaxEvent)
	{
		return;
	}

	PtListTrgInfo& kList = rkData.m_akEventTrgs[iEvent];
	for (PtListTrgInfo::iterator kIter = kList.begin(); kIter != kList.end(); ++kIter)
	{
		TePtTriggerInfo& kInfo = *kIter;
		if (kInfo.pAction == pAction)
		{
			kInfo.bDisabled = false;
			return;
		}
	}

	TePtTriggerInfo info;
	info.iEvent = iEvent;
	info.bDisabled = false;
	info.pCondition = pCondition;
	info.pAction = pAction;
	info.iTrgCount = 0;

	kList.push_back(info);
}

int		TePtTriggerMgr::StartTimerTrg(int iTimeOut, PtActionFun pAction, const TePtParam& rkParam, PtConditionFun pCondition, unsigned int uiCounts, int iDelayTime)
{
	DATA_CONFERENCE();

	TePtTrigger* pkTrigger = new TePtTrigger();
	pkTrigger->m_iID = ++(rkData.m_iGenID);
	rkData.m_kTriggers[pkTrigger->m_iID] = pkTrigger;

	pkTrigger->m_eType = PTTT_TIMER;
	pkTrigger->m_iTimerOut = iTimeOut;
	pkTrigger->m_uiPeriodTime = rkData.m_uiTimeNow + iDelayTime;
	pkTrigger->m_pkParam = rkData.m_pkParam;
	pkTrigger->m_pCondition = pCondition;
	pkTrigger->m_pAction = pAction;
	pkTrigger->m_uiAllCounts = uiCounts;
	pkTrigger->m_kParam = rkParam;

	if (iDelayTime <= 0)
	{
		TePtTrigger* pkBackupTrg = rkData.m_pkCurTrigger;
		rkData.m_pkCurTrigger = pkTrigger;
		TePtParam* pkBackupParam = rkData.m_pkCurParam;
		rkData.m_pkCurParam = &(pkTrigger->m_kParam);

		pkTrigger->Execute(rkData.m_uiTimeNow);

		rkData.m_pkCurTrigger = pkBackupTrg;
		rkData.m_pkCurParam = pkBackupParam;
	}
	else
	{
		pkTrigger->m_eState = PTTS_STAGE;
	}

	return pkTrigger->m_iID;
}
