/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandStop.h"
#include "TW_Main.h"
#include "TW_ShareUnitData.h"
#include "TW_Unit.h"
#include "TW_MemoryObject.h"
#include "TW_TaskActionStand.h"
#include "TW_TaskActionDeath.h"

BeStopCommand::BeStopCommand()
{
	m_eCmdType = TwCommandType::BCT_STOP;

	m_iStopTime = 0;
	m_bDead = false;
	m_pkCurTask.reset(dynamic_cast<TwTask*>(mpTaskActionStand.alloc()));
}

bool BeStopCommand::IsDead()
{
	return m_bDead;
}

void BeStopCommand::SetDead()
{
	m_bDead = true;
	SafeDeleteTask(m_pkCurTask);
	m_pkCurTask.reset(dynamic_cast<TwTask*>(mpTaskActionDeath.alloc()));
	m_pkCurTask->AttachMain(pkAttachMain);
	m_pkCurTask->AttachUnit(pkAttachUnit);
}

BeExeResult BeStopCommand::Execute(int& iDeltaTime)
{
	BeExeCommand::Execute(iDeltaTime);
	gUnit->SetAttackingUnitID(0);

	if (!gUnit->IsDead() && !gUnit->HasUnitCarryFlag(BUCF_DIZZY) && !gUnit->HasUnitCarryFlag(BUCF_ISALLFORBID) && !gUnit->HasFlag(TwUnitFlag::BUF_ISPERSIST) && (m_iStopTime <= 0))
	{
		if (gUnit->GetCommandCount() > 0)
		{
			return BeExeResult::BER_EXE_END;
		}
	}

	m_iStopTime -= iDeltaTime;
	return m_pkCurTask->Execute(iDeltaTime);
}

bool BeStopCommand::CanHungUp(TwGiveCmdType eCmdType, bool bNeedHangCurrent) const
{
	return false;
}

bool BeStopCommand::CanCancel() const
{
	return CanInterrupt();
}

bool BeStopCommand::CanInterrupt() const
{
	if (!gUnit->HasUnitCarryFlag(BUCF_DIZZY))
	{
		return true;
	}
	return false;
}
