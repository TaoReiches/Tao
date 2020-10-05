#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TriggerMgr.h"
#include "TW_Switch.h"
#include "TW_MemoryPool.h"

enum TePtTriggerType
{
	PTTT_EVENT = 0,
	PTTT_TIMER,
};

enum TePtTriggerState
{
	PTTS_CREATE = 0,
	PTTS_WAIT,
	PTTS_STAGE,
	PTTS_OVER,
};

struct TePtTriggerInfo;

class TePtTrigger
{
	friend class TePtTriggerMgr;

public:
	TePtTrigger(void);
	~TePtTrigger(void);

	void	Enable(void);
	void	Disable(void);
	void	Finish(void);

	void	Execute(unsigned int uiTimeNow);
	void	WaitToTime(unsigned int uiTime);
	void	Return(void);
	bool	WaitFinish(void);

	void	OnDeleteUnit(void* pkUnit);

public:
	TePtTriggerType	m_eType;
	int				m_iID;
	TePtTriggerState	m_eState;
	unsigned int			m_uiWaitTime;
	unsigned int			m_uiCurTime;
	void* m_pkParam;
	PtConditionFun	m_pCondition;
	PtActionFun		m_pAction;
	TePtParam		m_kParam;
	TePtTriggerInfo* m_pkInfo;

	int				m_iTimerOut;
	unsigned int			m_uiPeriodTime;
	unsigned int			m_uiExeCounts;
	unsigned int			m_uiAllCounts;

	pt				m_kPt;
};
