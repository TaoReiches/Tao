/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TriggerMgr.h"
#include "TW_TriggerMgrDef.h"
#include "TW_Define.h"
#include "TW_MemoryPool.h"

TeMemoryPool<TePtTrigger>          mpTrigger(512);
#define DATA_CONFERENCE() TwPtTriggerMgrData& rkData = *((TwPtTriggerMgrData *)m_pkData)

TePtTriggerMgr::TePtTriggerMgr(void* pkParam)
{
	m_pkData = new TwPtTriggerMgrData;
	DATA_CONFERENCE();

	rkData.m_pkParam = pkParam;

	rkData.m_iGenID = 0;
	rkData.m_uiTimeNow = 0;
	rkData.m_pkCurTrigger.reset();

	rkData.m_iMaxEvent = 0;
	rkData.m_akEventTrgs.clear();
}

TePtTriggerMgr::~TePtTriggerMgr(void)
{
	Finialize();

	TwPtTriggerMgrData* pkData = (TwPtTriggerMgrData*)m_pkData;
	delete pkData;
	m_pkData = nullptr;
}

bool TePtTriggerMgr::Initialize(int iMaxEvent)
{
	DATA_CONFERENCE();

	rkData.m_iGenID = 0;
	rkData.m_uiTimeNow = 0;
	rkData.m_pkCurTrigger.reset();

	rkData.m_iMaxEvent = iMaxEvent;

	rkData.m_kTriggers.clear();

	return true;
}

void TePtTriggerMgr::Update(int iDeltaTime)
{
	DATA_CONFERENCE();

	rkData.m_uiTimeNow += iDeltaTime;

	for (auto kIter = rkData.m_kTriggers.begin(); kIter != rkData.m_kTriggers.end();)
	{
		auto pkTrigger = kIter->second;
		if (pkTrigger->m_eState == TwPtTriggerState::PTTS_OVER)
		{
			mpTrigger.free(pkTrigger.get());
            auto itr1 = kIter;
			++kIter;
			rkData.m_kTriggers.erase(itr1);
			continue;
		}

		rkData.m_pkCurTrigger = pkTrigger;
		rkData.m_pkCurParam = std::shared_ptr<TePtParam>(&(pkTrigger->m_kParam));

		pkTrigger->Execute(rkData.m_uiTimeNow);

		rkData.m_pkCurTrigger = NULL;
		rkData.m_pkCurParam = NULL;

		++kIter;
	}
}

void TePtTriggerMgr::Finialize(void)
{
	DATA_CONFERENCE();

	for (auto kIter = rkData.m_kTriggers.begin(); kIter != rkData.m_kTriggers.end(); ++kIter)
	{
		auto pkTrigger = kIter->second;
		mpTrigger.free(pkTrigger.get());
	}
	rkData.m_kTriggers.clear();

	rkData.m_iGenID = 0;
	rkData.m_uiTimeNow = 0;
	rkData.m_pkCurTrigger = NULL;

	rkData.m_iMaxEvent = 0;
	rkData.m_akEventTrgs.clear();
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
		auto pkTrigger = kIter->second;
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

    auto eventTrg = rkData.m_akEventTrgs.find(iEvent);
    if (eventTrg == rkData.m_akEventTrgs.end())
    {
        return;
    }

	auto kList = eventTrg->second;
	if (kList.size() == 0)
	{
		return;
	}

	for (auto kIter = kList.begin(); kIter != kList.end(); ++kIter)
	{
		auto& kInfo = *kIter;
		if (kInfo.bDisabled)
		{
			continue;
		}
		if (kInfo.iTrgCount >= 24)
		{
			throw 0;
			return;
		}

		auto pkBackupParam = rkData.m_pkCurParam;
		rkData.m_pkCurParam = std::shared_ptr<TePtParam>(&rkParam);
		if (kInfo.pCondition)
		{
			auto eRet = kInfo.pCondition(rkData.m_pkParam);
			if (eRet != TePtCondResult::PTCR_OK)
			{
				rkData.m_pkCurParam = pkBackupParam;
				continue;
			}
		}

		TePtTrigger kTempTrg;
		kTempTrg.m_iID = ++(rkData.m_iGenID);
		kTempTrg.m_pkInfo = std::shared_ptr<TwPtTriggerInfo>(&kInfo);
		kTempTrg.m_pkParam = rkData.m_pkParam;
		kTempTrg.m_pAction = kInfo.pAction;

		auto pkBackupTrg = rkData.m_pkCurTrigger;
		rkData.m_pkCurTrigger = std::shared_ptr<TePtTrigger>(&kTempTrg);

		kInfo.iTrgCount++;

		kTempTrg.Execute(rkData.m_uiTimeNow);

		kInfo.iTrgCount--;

		rkData.m_pkCurTrigger = pkBackupTrg;
		rkData.m_pkCurParam = pkBackupParam;

		if (kTempTrg.m_eState == TwPtTriggerState::PTTS_WAIT)
		{
			TePtTrigger* pkTrigger = mpTrigger.alloc();
			*pkTrigger = kTempTrg;
			pkTrigger->m_kParam = rkParam;

			rkData.m_kTriggers[pkTrigger->m_iID] = std::shared_ptr<TePtTrigger>(pkTrigger);
		}
	}
}

bool	TePtTriggerMgr::FireTriggerBegin(int iEvent, TePtParam& rkParam, std::shared_ptr<TePtParam>& pkBackupParam)
{
	DATA_CONFERENCE();
	if (iEvent < 0 || iEvent >= rkData.m_iMaxEvent)
	{
		return false;
	}
	pkBackupParam = rkData.m_pkCurParam;
	rkData.m_pkCurParam = std::shared_ptr<TePtParam>(&rkParam);
	return true;
}

void	TePtTriggerMgr::FireTriggerEnd(std::shared_ptr<TePtParam>& pkBackupParam)
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
		TwPtTriggerInfo& kInfo = *kIter;
		if (kInfo.pAction == pAction)
		{
			kInfo.bDisabled = false;
			return;
		}
	}

	TwPtTriggerInfo info;
	info.iEvent = iEvent;
	info.bDisabled = false;
	info.pCondition = pCondition;
	info.pAction = pAction;
	info.iTrgCount = 0;

	kList.push_back(info);
}

int		TePtTriggerMgr::StartTimerTrg(int iTimeOut, PtActionFun pAction, const TePtParam& rkParam,
                                        PtConditionFun pCondition, unsigned int uiCounts, int iDelayTime)
{
	DATA_CONFERENCE();

	auto pkTrigger = std::shared_ptr<TePtTrigger>(mpTrigger.alloc());
	pkTrigger->m_iID = ++(rkData.m_iGenID);
	rkData.m_kTriggers[pkTrigger->m_iID] = pkTrigger;

	pkTrigger->m_eType = TwPtTriggerType::PTTT_TIMER;
	pkTrigger->m_iTimerOut = iTimeOut;
	pkTrigger->m_uiPeriodTime = rkData.m_uiTimeNow + iDelayTime;
	pkTrigger->m_pkParam = rkData.m_pkParam;
	pkTrigger->m_pCondition = pCondition;
	pkTrigger->m_pAction = pAction;
	pkTrigger->m_uiAllCounts = uiCounts;
	pkTrigger->m_kParam = rkParam;

	if (iDelayTime <= 0)
	{
		auto pkBackupTrg = rkData.m_pkCurTrigger;
		rkData.m_pkCurTrigger = pkTrigger;
		auto pkBackupParam = rkData.m_pkCurParam;
		rkData.m_pkCurParam = std::shared_ptr<TePtParam>(&(pkTrigger->m_kParam));

		pkTrigger->Execute(rkData.m_uiTimeNow);

		rkData.m_pkCurTrigger = pkBackupTrg;
		rkData.m_pkCurParam = pkBackupParam;
	}
	else
	{
		pkTrigger->m_eState = TwPtTriggerState::PTTS_STAGE;
	}

	return pkTrigger->m_iID;
}
