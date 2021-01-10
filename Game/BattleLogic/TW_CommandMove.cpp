/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandMove.h"
#include "TW_TaskMoveToPos.h"
#include "TW_MemoryObject.h"

BeMoveCommand::BeMoveCommand()
{
	m_eCmdType = TwCommandType::BCT_MOVE;
}

TwPos2 BeMoveCommand::GetTargetPos() const
{
	return ((TwTaskMoveToPos*)m_pkCurTask.get())->GetTargetPos();
}

void BeMoveCommand::SetTargetPos(const TwPos2& kPos, float fDistance, bool bTurn)
{
	if (!m_pkCurTask || m_pkCurTask->GetType() != TwTaskType::STT_MOVE_TO_POS)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask.reset(dynamic_cast<TwTask*>(mpTaskMoveToPos.alloc()));
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pAttachUnit);
	}

	((TwTaskMoveToPos*)m_pkCurTask.get())->SetTargetPos(kPos, fDistance, bTurn);
}

TwExeResult BeMoveCommand::Execute(int& iDeltaTime)
{
	BeExeCommand::Execute(iDeltaTime);
	return m_pkCurTask->Execute(iDeltaTime);
}

bool BeMoveCommand::CanHungUp(TwGiveCmdType eCmdType, bool bNeedHangCurrent) const
{
	return true;
}
bool BeMoveCommand::CanCancel() const
{
	return true;
}
bool BeMoveCommand::CanInterrupt() const
{
	return true;
}
