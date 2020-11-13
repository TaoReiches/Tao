/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandAttack.h"
#include "TW_MemoryObject.h"
#include "TW_TaskAttackUnit.h"
#include "TW_TaskAttackToPos.h"
#include "TW_TaskAttackItem.h"

BeAttackCommand::BeAttackCommand()
{
	m_eCmdType = BeCommandType::BCT_ATTACK;
}

void BeAttackCommand::SetTargetID(int iID, float fDistance, bool bIsOrb, int iSkillTypeID, int iSkillLevel, int iAttackCountLimit)
{
	if (!m_pkCurTask || m_pkCurTask->GetType() != BeTaskType::STT_ATTACK_UNIT)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask.reset(dynamic_cast<BeTask*>(mpTaskAttackUnit.alloc()));
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pkAttachUnit);
	}

	((BeTaskAttackUnit*)m_pkCurTask.get())->SetTargetID(iID, fDistance, bIsOrb, iSkillTypeID, iSkillLevel, iAttackCountLimit);
}

void BeAttackCommand::SetTargetPos(const TwPos2& kPos, float fRange)
{
	if (!m_pkCurTask || m_pkCurTask->GetType() != BeTaskType::STT_ATTACK_TO_POS)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask.reset(dynamic_cast<BeTask*>(mpTaskAttackToPos.alloc()));
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pkAttachUnit);
	}

	((BeTaskAttackToPos*)m_pkCurTask.get())->SetTargetPos(kPos, fRange);
}

void BeAttackCommand::SetTargetItem(int iItemID)
{
	if (!m_pkCurTask || m_pkCurTask->GetType() != BeTaskType::STT_ATTACK_ITEM)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask.reset(dynamic_cast<BeTask*>(mpTaskAttackItem.alloc()));
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pkAttachUnit);
	}

	((BeTaskAttackItem*)m_pkCurTask.get())->SetTargetID(iItemID, 0.0f);
}

BeExeResult BeAttackCommand::Execute(int& iDeltaTime)
{
	BeExeCommand::Execute(iDeltaTime);

	if (m_pkCurTask)
	{
		return m_pkCurTask->Execute(iDeltaTime);
	}

	return BeExeResult::BER_EXE_END;
}

bool BeAttackCommand::CanHungUp(BeGiveCmdType eCmdType, bool bNeedHangCurrent) const
{
	return CanCancel();
}
bool BeAttackCommand::CanCancel() const
{
	if (m_pkCurTask && m_pkCurTask->GetType() == BeTaskType::STT_ATTACK_UNIT)
	{
		return ((BeTaskAttackUnit*)m_pkCurTask.get())->IsCanCancel();
	}
	else if (m_pkCurTask && m_pkCurTask->GetType() == BeTaskType::STT_ATTACK_TO_POS)
	{
		return ((BeTaskAttackToPos*)m_pkCurTask.get())->IsCanCancel();
	}

	return true;
}
bool BeAttackCommand::CanInterrupt() const
{
	return CanCancel();
}

bool BeAttackCommand::CanSkip() const
{
	return CanCancel();
}
