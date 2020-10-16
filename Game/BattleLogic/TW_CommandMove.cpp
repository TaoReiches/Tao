/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandMove.h"
#include "TW_TaskMoveToPos.h"
#include "TW_MemoryObject.h"

BeMoveCommand::BeMoveCommand()
{
	m_eCmdType = BeCommandType::BCT_MOVE;
}

TePos2 BeMoveCommand::GetTargetPos() const
{
	return ((BeTaskMoveToPos*)m_pkCurTask.get())->GetTargetPos();
}

void BeMoveCommand::SetTargetPos(const TePos2& kPos, float fDistance, bool bTurn)
{
	if (!m_pkCurTask || m_pkCurTask->GetType() != BeTaskType::STT_MOVE_TO_POS)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask.reset(dynamic_cast<BeTask*>(mpTaskMoveToPos.alloc()));
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pkAttachUnit);
	}

	((BeTaskMoveToPos*)m_pkCurTask.get())->SetTargetPos(kPos, fDistance, bTurn);
}

BeExeResult BeMoveCommand::Execute(int& iDeltaTime)
{
	BeExeCommand::Execute(iDeltaTime);
	return m_pkCurTask->Execute(iDeltaTime);
}

bool BeMoveCommand::CanHungUp(BeGiveCmdType eCmdType, bool bNeedHangCurrent) const
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
