/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TaskActionDeath.h"
#include "TW_Main.h"
#include "TW_Unit.h"

BeTaskActionDeath::BeTaskActionDeath()
{
	TaskType = TwTaskType::STT_ACTION_DEATH;

	m_bInit = false;
}

TwExeResult BeTaskActionDeath::Execute(int& iDeltaTime)
{
	TwTask::Execute(iDeltaTime);
	gUnit->ClrFlag(TwUnitFlag::BUF_MOVING);
	if (!m_bInit)
	{
		gUnit->SetUnitAction(BUA_DEATH, BAN_death);
		m_bInit = true;
	}

	int iLoopCount = 10;
	while (iDeltaTime > 0)
	{
		--iLoopCount;
		if (iLoopCount <= 0)
		{
			iDeltaTime = 0;
			break;
		}
		if (gUnit->IsActionNow(BUA_DEATH))
		{
			if (gUnit->IsActionCurTimeOut(iDeltaTime))
			{
				iDeltaTime -= gUnit->GetActionCurTimeNeed();

				return TwExeResult::ER_ALL_OVER;
			}
			else
			{
				gUnit->IncActionCurTime(iDeltaTime);
				iDeltaTime = 0;

				return TwExeResult::ER_TIME_OUT;
			}
		}
		else
		{
			if (gUnit->IsActionCurTimeOut(iDeltaTime))
			{
				return TwExeResult::ER_ALL_OVER;
			}
			else
			{
				gUnit->IncActionCurTime(iDeltaTime);
				iDeltaTime = 0;

				return TwExeResult::ER_TIME_OUT;
			}
		}
	}

	return TwExeResult::ER_TIME_OUT;
}
