#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <list>
#include "TW_Trigger.h"

class TwPtTriggerInfo
{
public:
    TwTriggerEvent  iEvent;
	bool			bDisabled;
	PtConditionFun	pCondition;
	PtActionFun		pAction;
	int				iTrgCount;
};

typedef std::list<TwPtTriggerInfo>	PtListTrgInfo;

class TwPtTriggerMgrData
{
public:
	void*                                       m_pkParam;
	int                                         m_iGenID;
	unsigned int                                m_uiTimeNow;
	std::shared_ptr<TwPtTrigger>                m_pkCurTrigger;
    std::shared_ptr<TwPtParam>                  m_pkCurParam;
	int                                         m_iMaxEvent;
    std::map<TwTriggerEvent, PtListTrgInfo>     m_akEventTrgs;
	std::map<int, std::shared_ptr<TwPtTrigger>> m_kTriggers;
};
