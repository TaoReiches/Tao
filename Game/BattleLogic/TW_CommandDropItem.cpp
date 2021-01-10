/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandDropItem.h"
#include "TW_MemoryObject.h"
#include "TW_UnitMgr.h"
#include "TW_Main.h"
#include "TW_TaskMoveToUnit.h"
#include "TW_TaskMoveToPos.h"

BeDropItemCommand::BeDropItemCommand()
{
	m_eCmdType = TwCommandType::BCT_DROP_ITEM;
	m_iTargetID = 0;
	m_iItemID = 0;
}

void BeDropItemCommand::SetTargetIDDropItem(int iID, int iItemID)
{
	m_iItemID = iItemID;
	m_iTargetID = iID;
	SafeDeleteTask(m_pkCurTask);
	m_pkCurTask.reset(dynamic_cast<TwTask*>(mpTaskMoveToUnit.alloc()));
	m_pkCurTask->AttachMain(pkAttachMain);
	m_pkCurTask->AttachUnit(pAttachUnit);

	std::shared_ptr<TwUnit> pkTarget = gUnitMgr->GetUnitByID(iID);

	((BeTaskMoveToUnit*)m_pkCurTask.get())->SetTargetID(iID, 1000.0f);
}

void BeDropItemCommand::SetTargetPosDropItem(const TwPos2& kPos, int iItemID)
{
	m_iItemID = iItemID;
	m_kTargetPos = kPos;
	SafeDeleteTask(m_pkCurTask);
	m_pkCurTask.reset(dynamic_cast<TwTask*>(mpTaskMoveToPos.alloc()));
	m_pkCurTask->AttachMain(pkAttachMain);
	m_pkCurTask->AttachUnit(pAttachUnit);

	((TwTaskMoveToPos*)m_pkCurTask.get())->SetTargetPos(kPos, 1000.0f);
}

TwExeResult BeDropItemCommand::Execute(int& iDeltaTime)
{
	BeExeCommand::Execute(iDeltaTime);

	return TwExeResult::ER_EXE_END;
}

bool BeDropItemCommand::CanHungUp(TwGiveCmdType eCmdType, bool bNeedHangCurrent) const
{
	if (!m_pkCurTask)
	{
		return false;
	}

	if (m_pkCurTask->GetType() == TwTaskType::STT_MOVE_TO_POS || m_pkCurTask->GetType() == TwTaskType::STT_MOVE_TO_UNIT)
	{
		return true;
	}
	return false;
}

bool BeDropItemCommand::CanCancel() const
{
	return true;
}
bool BeDropItemCommand::CanInterrupt() const
{
	return true;
}
