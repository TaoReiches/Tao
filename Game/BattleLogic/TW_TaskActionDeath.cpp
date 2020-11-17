/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TaskActionDeath.h"
#include "TW_Main.h"
#include "TW_Unit.h"

BeTaskActionDeath::BeTaskActionDeath()
{
	m_eType = BeTaskType::STT_ACTION_DEATH;

	m_bInit = false;
}

BeExeResult BeTaskActionDeath::Execute(int& iDeltaTime)
{
	BeTask::Execute(iDeltaTime);
	gUnit.ClrFlag(BUF_MOVING);
	if (!m_bInit)
	{
		gUnit.SetUnitAction(BUA_DEATH, BAN_death);
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
		if (gUnit.IsActionNow(BUA_DEATH))
		{
			if (gUnit.IsActionCurTimeOut(iDeltaTime))
			{
				iDeltaTime -= gUnit.GetActionCurTimeNeed();

				return BeExeResult::BER_ALL_OVER;
			}
			else
			{
				gUnit.IncActionCurTime(iDeltaTime);
				iDeltaTime = 0;

				return BeExeResult::BER_TIME_OUT;
			}
		}
		else
		{
			if (gUnit.IsActionCurTimeOut(iDeltaTime))
			{
				return BeExeResult::BER_ALL_OVER;
			}
			else
			{
				gUnit.IncActionCurTime(iDeltaTime);
				iDeltaTime = 0;

				return BeExeResult::BER_TIME_OUT;
			}
		}
	}

	return BeExeResult::BER_TIME_OUT;
}
