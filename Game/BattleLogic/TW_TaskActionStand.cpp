/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TaskActionStand.h"
#include "TW_Main.h"
#include "TW_Unit.h"

BeTaskActionStand::BeTaskActionStand()
{
	TaskType = TwTaskType::STT_ACTION_STAND;
}

TwExeResult BeTaskActionStand::Execute(int& iDeltaTime)
{
	TwTask::Execute(iDeltaTime);

	if (TaskTime > 200)
	{
		gUnit->ClrFlag(TwUnitFlag::BUF_MOVING);
	}

	if (!gUnit->IsActionNow(BUA_STAND))
	{
		gUnit->SetUnitAction(BUA_STAND, BAN_stand);
	}

	gUnit->IncActionCurTime(iDeltaTime);
	iDeltaTime = 0;

	return TwExeResult::ER_TIME_OUT;
}
