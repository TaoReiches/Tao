/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Trigger.h"
#include "TW_TriggerMgrDef.h"

//ME_IMPLEMENT_POOL(TePtTrigger,true,128,32);

TwPtTrigger::TwPtTrigger(void)
{
	m_eType = TwPtTriggerType::PTTT_EVENT;
	m_iID = 0;
	m_eState = TwPtTriggerState::PTTS_CREATE;
	m_uiWaitTime = 0;

	m_pkParam = nullptr;
	m_pCondition = nullptr;
	m_pAction = nullptr;
	m_pkInfo = nullptr;

	m_iTimerOut = 0;
	m_uiPeriodTime = 0;
	m_uiExeCounts = 0;
	m_uiAllCounts = 0;
	m_uiCurTime = 0;
}

TwPtTrigger::~TwPtTrigger(void)
{

}

void TwPtTrigger::Enable(void)
{
	if (m_pkInfo)
	{
		m_pkInfo->bDisabled = false;
	}
}

void TwPtTrigger::Disable(void)
{
	if (m_pkInfo)
	{
		m_pkInfo->bDisabled = true;
	}
}

void	TwPtTrigger::Finish(void)
{
	m_eState = TwPtTriggerState::PTTS_OVER;
}

void	TwPtTrigger::Execute(unsigned int uiTimeNow)
{
	m_uiCurTime = uiTimeNow;
	m_kPt.pkAttachMain = m_pkParam;

	if (m_eType == TwPtTriggerType::PTTT_TIMER)
	{
		if (m_eState == TwPtTriggerState::PTTS_WAIT)
		{
			m_pAction(&m_kPt);
		}
		if (uiTimeNow > m_uiPeriodTime)
		{
			m_uiPeriodTime += m_iTimerOut;
			m_uiExeCounts++;

			m_pAction(&m_kPt);
		}
	}
	else
	{
		if (m_eState == TwPtTriggerState::PTTS_WAIT)
		{
			if (m_uiCurTime >= m_uiWaitTime)
			{
				m_pAction(&m_kPt);
			}
		}
		else
		{
			m_pAction(&m_kPt);
		}
	}
}

bool	TwPtTrigger::WaitFinish(void)
{
	if (m_uiCurTime >= m_uiWaitTime)
	{
		return true;
	}
	return false;
}

void	TwPtTrigger::WaitToTime(unsigned int uiTime)
{
	m_eState = TwPtTriggerState::PTTS_WAIT;
	m_uiWaitTime = uiTime;
}

void	TwPtTrigger::Return(void)
{
	if (m_eType == TwPtTriggerType::PTTT_TIMER)
	{
		if (m_uiExeCounts < m_uiAllCounts)
		{
			m_eState = TwPtTriggerState::PTTS_STAGE;
		}
		else
		{
			m_eState = TwPtTriggerState::PTTS_OVER;
		}
	}
	else
	{
		m_eState = TwPtTriggerState::PTTS_OVER;
	}
}

void	TwPtTrigger::OnDeleteUnit(void* pkUnit)
{
	m_kParam.OnDeleteUnit(pkUnit);
}
