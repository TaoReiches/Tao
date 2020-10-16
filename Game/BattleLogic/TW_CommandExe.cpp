/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandExe.h"
#include "TW_MemoryObject.h"

BeExeCommand::BeExeCommand()
{
	m_eCmdType = BeCommandType::BCT_STOP;
}

BeExeCommand::~BeExeCommand()
{
	SafeDeleteTask(m_pkCurTask);
}

BeCommandType BeExeCommand::GetType() const
{
	return m_eCmdType;
}
BeTaskType BeExeCommand::GetTaskType() const
{
	return m_pkCurTask ? m_pkCurTask->GetType() : BeTaskType::STT_NONE;
}

void BeExeCommand::SafeDeleteTask(std::unique_ptr<BeTask>& pkTask)
{
	if (pkTask)
	{
		switch (pkTask.get()->GetType())
		{
		case BeTaskType::STT_ACTION_STAND: mpTaskActionStand.free((BeTaskActionStand*)pkTask.get()); break;
		case BeTaskType::STT_ACTION_WALK:mpTaskActionWalk.free((BeTaskActionWalk*)pkTask.get()); break;
		case BeTaskType::STT_ACTION_ATTACK:mpTaskActionAttack.free((BeTaskActionAttack*)pkTask.get()); break;
		case BeTaskType::STT_ACTION_SPELL:mpTaskActionSpell.free((BeTaskActionSpell*)pkTask.get()); break;
		case BeTaskType::STT_ACTION_DEATH:mpTaskActionDeath.free((BeTaskActionDeath*)pkTask.get()); break;

		case BeTaskType::STT_MOVE_TO_POS:mpTaskMoveToPos.free((BeTaskMoveToPos*)pkTask.get()); break;
		case BeTaskType::STT_MOVE_TO_UNIT:mpTaskMoveToUnit.free((BeTaskMoveToUnit*)pkTask.get()); break;
		case BeTaskType::STT_ATTACK_UNIT:mpTaskAttackUnit.free((BeTaskAttackUnit*)pkTask.get()); break;
		case BeTaskType::STT_ATTACK_TO_POS:mpTaskAttackToPos.free((BeTaskAttackToPos*)pkTask.get()); break;
		case BeTaskType::STT_ATTACK_ITEM:mpTaskAttackItem.free((BeTaskAttackItem*)pkTask.get()); break;
		}
		pkTask.release();
	}
}

BeExeResult BeExeCommand::Execute(int& iDeltaTime)
{
	return BeExeResult::BER_EXE_END;
}

bool BeExeCommand::CanSkip(void) const
{
	return false;
}
