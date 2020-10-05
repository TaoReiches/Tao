#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <list>
#include "TW_Trigger.h"

struct TePtTriggerInfo
{
	int				iEvent;
	bool			bDisabled;
	PtConditionFun	pCondition;
	PtActionFun		pAction;
	int				iTrgCount;
};

typedef std::list<TePtTriggerInfo>	PtListTrgInfo;

struct TePtTriggerMgrData
{
	void* m_pkParam;

	int							m_iGenID;
	unsigned int						m_uiTimeNow;
	TePtTrigger* m_pkCurTrigger;
	TePtParam* m_pkCurParam;

	int							m_iMaxEvent;
	PtListTrgInfo* m_akEventTrgs;
	std::map<int, TePtTrigger*>	m_kTriggers;
};
