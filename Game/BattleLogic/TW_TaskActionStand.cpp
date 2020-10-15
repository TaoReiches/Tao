/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TaskActionStand.h"
#include "TW_Main.h"
#include "TW_Unit.h"

BeTaskActionStand::BeTaskActionStand()
{
	m_eType = BeTaskType::STT_ACTION_STAND;
}

BeExeResult BeTaskActionStand::Execute(int& iDeltaTime)
{
	BeTask::Execute(iDeltaTime);

	if (m_iTaskTime > 200)
	{
		gUnit.ClrFlag(BUF_MOVING);
	}

	if (!gUnit.IsActionNow(BUA_STAND))
	{
		gUnit.SetUnitAction(BUA_STAND, BAN_stand);
	}

	gUnit.IncActionCurTime(iDeltaTime);
	iDeltaTime = 0;

	return BeExeResult::BER_TIME_OUT;
}
