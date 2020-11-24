/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TriggerMgr.h"
#include "TW_TriggerMgrDef.h"
#include "TW_Define.h"
#include "TW_MemoryPool.h"

TwMemoryPool<TwPtTrigger>          mpTrigger(512);
#define DATA_CONFERENCE() TwPtTriggerMgrData& rkData = *((TwPtTriggerMgrData *)m_pkData)

TwPtTriggerMgr::TwPtTriggerMgr(void* pkParam)
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

TwPtTriggerMgr::~TwPtTriggerMgr(void)
{
	Finialize();

	TwPtTriggerMgrData* pkData = (TwPtTriggerMgrData*)m_pkData;
	delete pkData;
	m_pkData = nullptr;
}

bool TwPtTriggerMgr::Initialize(int iMaxEvent)
{
	DATA_CONFERENCE();

	rkData.m_iGenID = 0;
	rkData.m_uiTimeNow = 0;
	rkData.m_pkCurTrigger.reset();

	rkData.m_iMaxEvent = iMaxEvent;

	rkData.m_kTriggers.clear();

	return true;
}

void TwPtTriggerMgr::Update(int iDeltaTime)
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
		rkData.m_pkCurParam = std::shared_ptr<TwPtParam>(&(pkTrigger->m_kParam));

		pkTrigger->Execute(rkData.m_uiTimeNow);

		rkData.m_pkCurTrigger = NULL;
		rkData.m_pkCurParam = NULL;

		++kIter;
	}
}

void TwPtTriggerMgr::Finialize(void)
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

unsigned int TwPtTriggerMgr::GetTimeNow(void)
{
	DATA_CONFERENCE();

	return rkData.m_uiTimeNow;
}

int TwPtTriggerMgr::GetTriggerID(void)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurTrigger)
	{
		return rkData.m_pkCurTrigger->m_iID;
	}
	return -1;
}

bool TwPtTriggerMgr::HasParam(TwTrgParamID iParamID)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurParam)
	{
		return rkData.m_pkCurParam->HasParam(iParamID);
	}
	return false;
}

int TwPtTriggerMgr::GetParamInt(TwTrgParamID iParamID, int iDefault)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurParam)
	{
		return rkData.m_pkCurParam->GetParamInt(iParamID, iDefault);
	}
	return iDefault;
}

float TwPtTriggerMgr::GetParamFloat(TwTrgParamID iParamID, float fDefault)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurParam)
	{
		return rkData.m_pkCurParam->GetParamFloat(iParamID, fDefault);
	}
	return fDefault;
}

void* TwPtTriggerMgr::GetParamVoid(TwTrgParamID iParamID)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurParam)
	{
		return rkData.m_pkCurParam->GetParamVoid(iParamID);
	}
	return NULL;
}

void    TwPtTriggerMgr::OnUnitDelete(void* pkUnit)
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

void	TwPtTriggerMgr::TrgWait(int iTime)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurTrigger)
	{
		rkData.m_pkCurTrigger->WaitToTime(rkData.m_uiTimeNow + iTime);
	}
}

void	TwPtTriggerMgr::Return(void)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurTrigger)
	{
		rkData.m_pkCurTrigger->Return();
	}
}

void	TwPtTriggerMgr::Enable(void)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurTrigger)
	{
		rkData.m_pkCurTrigger->Enable();
	}
}

void	TwPtTriggerMgr::Disable(void)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurTrigger)
	{
		rkData.m_pkCurTrigger->Disable();
	}
}

void	TwPtTriggerMgr::Finish(void)
{
	DATA_CONFERENCE();

	if (rkData.m_pkCurTrigger)
	{
		rkData.m_pkCurTrigger->Finish();
	}
}

void	TwPtTriggerMgr::FireTrigger(TwTriggerEvent iEvent, TwPtParam& rkParam)
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
		rkData.m_pkCurParam = std::shared_ptr<TwPtParam>(&rkParam);
		if (kInfo.pCondition)
		{
			auto eRet = kInfo.pCondition(rkData.m_pkParam);
			if (eRet != TwPtCondResult::PTCR_OK)
			{
				rkData.m_pkCurParam = pkBackupParam;
				continue;
			}
		}

		TwPtTrigger kTempTrg;
		kTempTrg.m_iID = ++(rkData.m_iGenID);
		kTempTrg.m_pkInfo = std::shared_ptr<TwPtTriggerInfo>(&kInfo);
		kTempTrg.m_pkParam = rkData.m_pkParam;
		kTempTrg.m_pAction = kInfo.pAction;

		auto pkBackupTrg = rkData.m_pkCurTrigger;
		rkData.m_pkCurTrigger = std::shared_ptr<TwPtTrigger>(&kTempTrg);

		kInfo.iTrgCount++;

		kTempTrg.Execute(rkData.m_uiTimeNow);

		kInfo.iTrgCount--;

		rkData.m_pkCurTrigger = pkBackupTrg;
		rkData.m_pkCurParam = pkBackupParam;

		if (kTempTrg.m_eState == TwPtTriggerState::PTTS_WAIT)
		{
			TwPtTrigger* pkTrigger = mpTrigger.alloc();
			*pkTrigger = kTempTrg;
			pkTrigger->m_kParam = rkParam;

			rkData.m_kTriggers[pkTrigger->m_iID] = std::shared_ptr<TwPtTrigger>(pkTrigger);
		}
	}
}

bool	TwPtTriggerMgr::FireTriggerBegin(TwTriggerEvent iEvent, TwPtParam& rkParam, std::shared_ptr<TwPtParam>& pkBackupParam)
{
	DATA_CONFERENCE();
	if (static_cast<int>(iEvent) < 0 || static_cast<int>(iEvent) >= rkData.m_iMaxEvent)
	{
		return false;
	}
	pkBackupParam = rkData.m_pkCurParam;
	rkData.m_pkCurParam = std::shared_ptr<TwPtParam>(&rkParam);
	return true;
}

void	TwPtTriggerMgr::FireTriggerEnd(std::shared_ptr<TwPtParam>& pkBackupParam)
{
	DATA_CONFERENCE();
	rkData.m_pkCurParam = pkBackupParam;
}

bool	TwPtTriggerMgr::WaitFinish(void)
{
	DATA_CONFERENCE();
	if (rkData.m_pkCurTrigger)
	{
		return rkData.m_pkCurTrigger->WaitFinish();
	}

	return false;
}

void	TwPtTriggerMgr::RegisterEventTrg(TwTriggerEvent iEvent, PtActionFun pAction, PtConditionFun pCondition)
{
	DATA_CONFERENCE();

	if (!pAction || static_cast<int>(iEvent) < 0 || static_cast<int>(iEvent) >= rkData.m_iMaxEvent)
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

int		TwPtTriggerMgr::StartTimerTrg(int iTimeOut, PtActionFun pAction, const TwPtParam& rkParam,
                                        PtConditionFun pCondition, unsigned int uiCounts, int iDelayTime)
{
	DATA_CONFERENCE();

	auto pkTrigger = std::shared_ptr<TwPtTrigger>(mpTrigger.alloc());
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
		rkData.m_pkCurParam = std::shared_ptr<TwPtParam>(&(pkTrigger->m_kParam));

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
