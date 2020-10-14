/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Command.h"
#include "TW_TaskAction.h"
#include "TW_TaskComplex.h"
#include "TW_Unit.h"
#include "TW_UnitMgr.h"
#include "TW_Main.h"
#include "Skill_table.hpp"
#include "TW_MapItem.h"
#include "TW_MapItemMgr.h"

BeCommand::BeCommand(BeCommandType cmd, int unit, float x, float y, int data, bool bShiftDelete, int data2, BeCommandTargetType eTType, bool bForceAttackOnceIn, float fDirX, float fDirY)
{
	eCmdType = cmd;
	iUnitID = unit;
	kPos.fX = x;
	kPos.fY = y;
	iData = data;
	iData2 = data2;
	bForceAttackOnce = bForceAttackOnceIn;
	bShiftCanDelete = bShiftDelete;
	eTargetType = eTType;
	eGiveType = BCT_NONE;
	bForceExecute = false;
	kDirPos.fX = fDirX;
	kDirPos.fY = fDirY;
}

BeExeCommand::BeExeCommand()
{
	m_eCmdType = BCT_STOP;
	m_pkCurTask = NULL;
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
	return m_pkCurTask ? m_pkCurTask->GetType() : STT_NONE;
}

void BeExeCommand::SafeDeleteTask(BeTask*& pkTask)
{
	if (pkTask)
	{
		switch (pkTask->GetType())
		{
		case STT_ACTION_STAND:BeTaskActionStand::DEL((BeTaskActionStand*)pkTask); break;
		case STT_ACTION_WALK:BeTaskActionWalk::DEL((BeTaskActionWalk*)pkTask); break;
		case STT_ACTION_ATTACK:BeTaskActionAttack::DEL((BeTaskActionAttack*)pkTask); break;
		case STT_ACTION_SPELL:BeTaskActionSpell::DEL((BeTaskActionSpell*)pkTask); break;
		case STT_ACTION_DEATH:BeTaskActionDeath::DEL((BeTaskActionDeath*)pkTask); break;

		case STT_MOVE_TO_POS:BeTaskMoveToPos::DEL((BeTaskMoveToPos*)pkTask); break;
		case STT_MOVE_TO_UNIT:BeTaskMoveToUnit::DEL((BeTaskMoveToUnit*)pkTask); break;
		case STT_ATTACK_UNIT:BeTaskAttackUnit::DEL((BeTaskAttackUnit*)pkTask); break;
		case STT_ATTACK_TO_POS:BeTaskAttackToPos::DEL((BeTaskAttackToPos*)pkTask); break;
		case STT_FOLLOW_UNIT:BeTaskFollowUnit::DEL((BeTaskFollowUnit*)pkTask); break;
		case STT_ATTACK_ITEM:BeTaskAttackItem::DEL((BeTaskAttackItem*)pkTask); break;
		}
		pkTask = 0;
	}
}

BeExeResult BeExeCommand::Execute(int& iDeltaTime)
{
	return BER_EXE_END;
}

bool BeExeCommand::CanSkip(void) const
{
	return false;
}

BeStopCommand::BeStopCommand()
{
	m_eCmdType = BCT_STOP;

	m_iStopTime = 0;
	m_bDead = false;
	m_pkCurTask = BeTaskActionStand::NEW();
}

bool BeStopCommand::IsDead()
{
	return m_bDead;
}

void BeStopCommand::SetDead()
{
	m_bDead = true;
	SafeDeleteTask(m_pkCurTask);
	m_pkCurTask = BeTaskActionDeath::NEW();
	m_pkCurTask->AttachMain(pkAttachMain);
	m_pkCurTask->AttachUnit(pkAttachUnit);
}

BeExeResult BeStopCommand::Execute(int& iDeltaTime)
{
	BeExeCommand::Execute(iDeltaTime);
	gUnit.SetAttackingUnitID(0);

	if (!gUnit.IsDead() && !gUnit.HasUnitCarryFlag(BUCF_DIZZY) && !gUnit.HasUnitCarryFlag(BUCF_ISALLFORBID) && !gUnit.HasFlag(BUF_ISPERSIST) && (m_iStopTime <= 0))
	{
		if (gUnit.GetCommandCount() > 0)
		{
			return BER_EXE_END;
		}
	}

	m_iStopTime -= iDeltaTime;
	return m_pkCurTask->Execute(iDeltaTime);
}

bool BeStopCommand::CanHungUp(BeGiveCmdType eCmdType, bool bNeedHangCurrent) const
{
	return false;
}

bool BeStopCommand::CanCancel() const
{
	return CanInterrupt();
}

bool BeStopCommand::CanInterrupt() const
{
	if (!gUnit.HasUnitCarryFlag(BUCF_DIZZY))
	{
		return true;
	}
	return false;
}

BeHoldCommand::BeHoldCommand()
{
	m_eCmdType = BCT_HOLD;

	m_pkCurTask = BeTaskActionStand::NEW();
}

bool BeHoldCommand::FindAttack(void)
{
	return false;
}

BeExeResult BeHoldCommand::Execute(int& iDeltaTime)
{
	BeExeCommand::Execute(iDeltaTime);
	int iLoopCount = 20;
	while (iDeltaTime > 0)
	{
		--iLoopCount;
		if (iLoopCount <= 0)
		{
			iDeltaTime = 0;
			break;
		}
		if (m_pkCurTask->GetType() == STT_ACTION_STAND)
		{
			FindAttack();
		}

		if (m_pkCurTask->GetType() == STT_ACTION_ATTACK)
		{
			BeExeResult eRet = m_pkCurTask->Execute(iDeltaTime);
			if (eRet == BER_TIME_OUT)
			{
				return BER_TIME_OUT;
			}

			if (iDeltaTime > 0)
			{
				SafeDeleteTask(m_pkCurTask);
				m_pkCurTask = BeTaskActionStand::NEW();
				m_pkCurTask->AttachMain(pkAttachMain);
				m_pkCurTask->AttachUnit(pkAttachUnit);
				continue;
			}

			int iTargetID = ((BeTaskActionAttack*)m_pkCurTask)->GetTargetID();
			BeUnit* pkTarget = gUnitMgr.GetUnitByID(iTargetID);
			if (!pkTarget)
			{
				if (FindAttack())
				{
					continue;
				}
				else
				{
					SafeDeleteTask(m_pkCurTask);
					m_pkCurTask = BeTaskActionStand::NEW();
					m_pkCurTask->AttachMain(pkAttachMain);
					m_pkCurTask->AttachUnit(pkAttachUnit);
				}
				continue;
			}

			float fTargetDistance2 = GetDistance2(gUnit.GetPosX(), gUnit.GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
			float fMaxAttackRange = gUnit.GetAttackRange(pkTarget);
			if (fTargetDistance2 > fMaxAttackRange * fMaxAttackRange)
			{
				SafeDeleteTask(m_pkCurTask);
				m_pkCurTask = BeTaskActionStand::NEW();
				m_pkCurTask->AttachMain(pkAttachMain);
				m_pkCurTask->AttachUnit(pkAttachUnit);
				continue;
			}

			if (pkTarget->IsDead() || !gUnit.UnitCanAttack(pkTarget, true, true))
			{
				if (FindAttack())
				{
					continue;
				}
				else
				{
					SafeDeleteTask(m_pkCurTask);
					m_pkCurTask = BeTaskActionStand::NEW();
					m_pkCurTask->AttachMain(pkAttachMain);
					m_pkCurTask->AttachUnit(pkAttachUnit);
				}
			}
			else
			{
				iDeltaTime = 0;
				return BER_TIME_OUT;
			}
			continue;
		}

		gUnit.SetAttackingUnitID(0);

		m_pkCurTask->Execute(iDeltaTime);
	}

	return BER_TIME_OUT;
}

bool BeHoldCommand::CanHungUp(BeGiveCmdType eCmdType, bool bNeedHangCurrent) const
{
	return true;
}

bool BeHoldCommand::CanCancel() const
{
	return true;
}

bool BeHoldCommand::CanInterrupt() const
{
	return true;
}

BeMoveCommand::BeMoveCommand()
{
	m_eCmdType = BCT_MOVE;
}

void BeMoveCommand::SetFollowID(int iID, float fDistance)
{
	if (!m_pkCurTask || m_pkCurTask->GetType() != STT_FOLLOW_UNIT)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask = BeTaskFollowUnit::NEW();
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pkAttachUnit);
	}

	((BeTaskFollowUnit*)m_pkCurTask)->SetFollowID(iID, fDistance);
}

TePos2 BeMoveCommand::GetTargetPos() const
{
	return ((BeTaskMoveToPos*)m_pkCurTask)->GetTargetPos();
}

void BeMoveCommand::SetTargetPos(const TePos2& kPos, float fDistance, bool bTurn)
{
	if (!m_pkCurTask || m_pkCurTask->GetType() != STT_MOVE_TO_POS)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask = BeTaskMoveToPos::NEW();
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pkAttachUnit);
	}

	((BeTaskMoveToPos*)m_pkCurTask)->SetTargetPos(kPos, fDistance, bTurn);
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

BePatrolCommand::BePatrolCommand()
{
	m_eCmdType = BCT_PATROL;

	m_pkCurTask = BeTaskAttackToPos::NEW();
	m_iStopTime = 0;
	m_iCurStopTime = 0;
}

void BePatrolCommand::SetPatrol(const TePos2& kTargetPos, const TePos2& kNextPos)
{
	m_kTargetPos = kTargetPos;
	m_kNextPos = kNextPos;

	((BeTaskAttackToPos*)m_pkCurTask)->SetTargetPos(m_kTargetPos);
}

BeExeResult BePatrolCommand::Execute(int& iDeltaTime)
{
	BeExeCommand::Execute(iDeltaTime);
	int iLoopCount = 20;
	while (iDeltaTime > 0)
	{
		--iLoopCount;
		if (iLoopCount <= 0)
		{
			iDeltaTime = 0;
			break;
		}
		BeExeResult eRet = m_pkCurTask->Execute(iDeltaTime);
		if (eRet == BER_EXE_END)
		{
			if (m_pkCurTask->GetType() == STT_ATTACK_TO_POS)
			{
				BeTaskAttackToPos* pkA2P = dynamic_cast<BeTaskAttackToPos*> (m_pkCurTask);
				if (pkA2P && pkA2P->GetMoveResult() == TFR_NONE)
				{
					iDeltaTime = 0;
				}
			}

			m_iCurStopTime += iDeltaTime;
			if (m_iCurStopTime > m_iStopTime)
			{
				TePos2 kTemp = m_kTargetPos;
				m_kTargetPos = m_kNextPos;
				m_kNextPos = kTemp;

				float fDist2 = GetDistance2(m_kTargetPos.fX, m_kTargetPos.fY, m_kNextPos.fX, m_kNextPos.fY);
				if (fDist2 < 50.0f * 50.0f)
				{
					return BER_EXE_END;
				}

				((BeTaskAttackToPos*)m_pkCurTask)->SetTargetPos(m_kTargetPos);
				m_iCurStopTime = 0;
			}
			else
			{
				iDeltaTime = 0;
			}

		}
	}
	return BER_TIME_OUT;
}

bool BePatrolCommand::CanHungUp(BeGiveCmdType eCmdType, bool bNeedHangCurrent) const
{
	return true;
}
bool BePatrolCommand::CanCancel() const
{
	return true;
}
bool BePatrolCommand::CanInterrupt() const
{
	return true;
}

BeAttackCommand::BeAttackCommand()
{
	m_eCmdType = BCT_ATTACK;
}

void BeAttackCommand::SetTargetID(int iID, float fDistance, bool bIsOrb, int iSkillTypeID, int iSkillLevel, int iAttackCountLimit)
{
	if (!m_pkCurTask || m_pkCurTask->GetType() != STT_ATTACK_UNIT)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask = BeTaskAttackUnit::NEW();
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pkAttachUnit);
	}

	((BeTaskAttackUnit*)m_pkCurTask)->SetTargetID(iID, fDistance, bIsOrb, iSkillTypeID, iSkillLevel, iAttackCountLimit);
}

void BeAttackCommand::SetTargetPos(const TePos2& kPos, float fRange)
{
	if (!m_pkCurTask || m_pkCurTask->GetType() != STT_ATTACK_TO_POS)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask = BeTaskAttackToPos::NEW();
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pkAttachUnit);
	}

	((BeTaskAttackToPos*)m_pkCurTask)->SetTargetPos(kPos, fRange);
}

void BeAttackCommand::SetTargetItem(int iItemID)
{
	if (!m_pkCurTask || m_pkCurTask->GetType() != STT_ATTACK_ITEM)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask = BeTaskAttackItem::NEW();
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pkAttachUnit);
	}

	((BeTaskAttackItem*)m_pkCurTask)->SetTargetID(iItemID, 0.0f);
}

BeExeResult BeAttackCommand::Execute(int& iDeltaTime)
{
	BeExeCommand::Execute(iDeltaTime);

	if (m_pkCurTask)
	{
		return m_pkCurTask->Execute(iDeltaTime);
	}

	return BER_EXE_END;
}

bool BeAttackCommand::CanHungUp(BeGiveCmdType eCmdType, bool bNeedHangCurrent) const
{
	return CanCancel();
}
bool BeAttackCommand::CanCancel() const
{
	if (m_pkCurTask && m_pkCurTask->GetType() == STT_ATTACK_UNIT)
	{
		return ((BeTaskAttackUnit*)m_pkCurTask)->IsCanCancel();
	}
	else if (m_pkCurTask && m_pkCurTask->GetType() == STT_ATTACK_TO_POS)
	{
		return ((BeTaskAttackToPos*)m_pkCurTask)->IsCanCancel();
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

BeSpellCommand::BeSpellCommand()
{
	m_eCmdType = BCT_SPELL;

	m_iSkillTypeID = 0;
	m_iSkillLevel = 1;
	m_bExpendMP = true;
	m_iTargetID = 0;
	m_iTargetType = 0;
	m_iItemID = 0;
	m_iUsePlayer = -1;
}

void BeSpellCommand::SpellTargetID(int iSkillTypeID, int iID, const TePos2& kPos, int iSkillLevel, int iItemID, bool bExpendMP, int iUsePlayer, int iTargetType)
{
	const SkillTable* pkResData = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID);
	if (!pkResData)
	{
		return;
	}

	SeCalSkillLvlData kData;
	if (!gUnit.GetSkillLvlData(kData, iSkillTypeID))
	{
		return;
	}
	SeCalSkillLvlData& rkData = kData;

	m_iTargetType = BCTT_UNIT;

	if (!m_pkCurTask || m_pkCurTask->GetType() != STT_MOVE_TO_UNIT)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask = BeTaskMoveToUnit::NEW();
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pkAttachUnit);
	}

	((BeTaskMoveToUnit*)m_pkCurTask)->SetTargetID(iID, (float)rkData.GetSpellDistance());

	m_iSkillTypeID = iSkillTypeID;
	m_iTargetID = iID;
	m_kTargetPos = kPos;
	m_iSkillLevel = iSkillLevel;
	m_iItemID = iItemID;
	m_iUsePlayer = iUsePlayer;
	m_bExpendMP = bExpendMP;
}

void BeSpellCommand::SpellTargetItem(int iSkillTypeID, int iTargetID, int iSkillLevel, int iItemID, bool bExpendMP, int iUsePlayer)
{
	if (!m_pkCurTask || m_pkCurTask->GetType() != STT_MOVE_TO_POS)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask = BeTaskMoveToPos::NEW();
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pkAttachUnit);
	}

	BeMapItem* pkMapItem = gMapItemMgr.GetMapItemByID(iTargetID);
	if (!pkMapItem)
	{
		return;
	}
	TePos2 kPos;
	kPos.fX = pkMapItem->GetPosX();
	kPos.fY = pkMapItem->GetPosY();

	m_iSkillTypeID = iSkillTypeID;
	m_iTargetID = iTargetID;
	m_kTargetPos = kPos;
	m_iSkillLevel = iSkillLevel;
	m_iItemID = iItemID;
	m_iUsePlayer = iUsePlayer;
	m_bExpendMP = bExpendMP;
	m_iTargetType = BCTT_MAPITEM;
}

void BeSpellCommand::SpellTargetPos(int iSkillTypeID, const TePos2& kPos, const TePos2& kDirPos, int iSkillLevel, int iItemID, bool bExpendMP, int iUsePlayer)
{
	if (!m_pkCurTask || m_pkCurTask->GetType() != STT_MOVE_TO_POS)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask = BeTaskMoveToPos::NEW();
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pkAttachUnit);
	}

	const SkillTable* pkSkillRes = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID);
	if (!pkSkillRes)
	{
		return;
	}

	if (pkSkillRes->uiOperateType == SKILL_OPERATETYPE_MUBIAO || SKILL_OPERATETYPE_MOFAQIANG == pkSkillRes->uiOperateType)
	{
		SeCalSkillLvlData kData;
		if (!gUnit.GetSkillLvlData(kData, iSkillTypeID))
		{
			return;
		}
		((BeTaskMoveToPos*)m_pkCurTask)->SetTargetPos(kPos, (float)kData.GetSpellDistance());
	}

	m_iSkillTypeID = iSkillTypeID;
	m_kTargetPos = kPos;
	m_iSkillLevel = iSkillLevel;
	m_iItemID = iItemID;
	m_iUsePlayer = iUsePlayer;
	m_bExpendMP = bExpendMP;
	m_kDirectPos = kDirPos;
}

BeExeResult BeSpellCommand::Execute(int& iDeltaTime)
{
	BeExeCommand::Execute(iDeltaTime);
	if (!m_pkCurTask)
	{
		return BER_EXE_END;
	}
	if (m_pkCurTask->GetType() != STT_ACTION_SPELL)
	{
		const SkillTable* pkResData = SkillTableMgr::Get()->GetSkillTable(m_iSkillTypeID);
		if (!pkResData)
		{
			return BER_EXE_END;
		}
		if (!gUnit.CanMove() && (pkResData->uiSkillProperty & SKILL_SKILLPROPERTY_DISPLACE)) {
			return BER_EXE_END;
		}

		BeExeResult eResult = m_pkCurTask->Execute(iDeltaTime);
		if (eResult == BER_TIME_OUT)
		{
			if (m_pkCurTask->GetType() == STT_MOVE_TO_UNIT)
			{
				BeUnit* pkTarget = gUnitMgr.GetUnitByID(m_iTargetID);
				if (pkTarget)
				{
					m_kTargetPos = TePos2(pkTarget->GetPosX(), pkTarget->GetPosY());
				}
			}
			return BER_TIME_OUT;
		}

		if (m_pkCurTask->GetType() == STT_MOVE_TO_UNIT)
		{
			if (m_iTargetID != 0)
			{
				if (((BeTaskMoveToUnit*)m_pkCurTask)->GetMoveResult() != BMR_SUCCESS)
				{
					{
						return BER_EXE_END;
					}
				}
			}
		}
		else if (m_pkCurTask->GetType() == STT_MOVE_TO_POS && pkResData->uiOperateType == SKILL_OPERATETYPE_MUBIAO)
		{
			if (((BeTaskMoveToPos*)m_pkCurTask)->GetMoveResult() != BMR_SUCCESS)
			{
				return BER_EXE_END;
			}
		}

		SafeDeleteTask(m_pkCurTask);
		BeTaskActionSpell* pkTask = BeTaskActionSpell::NEW();
		pkTask->AttachMain(pkAttachMain);
		pkTask->AttachUnit(pkAttachUnit);

		bool	bSucSet = false;
		if (m_iTargetID)
		{
			if (m_iTargetType == BCTT_UNIT)
			{
				BeUnit* pkTarget = gUnitMgr.GetUnitByID(m_iTargetID);
				if (pkTarget)
				{
					bSucSet = pkTask->SpellTargetID(m_iSkillTypeID, m_iSkillLevel, m_bExpendMP, m_iTargetID, m_kTargetPos, m_iItemID, m_iUsePlayer, m_iTargetType);
				}
				else
				{
					bSucSet = pkTask->SpellTargetPos(m_iSkillTypeID, m_iSkillLevel, m_bExpendMP, m_kTargetPos, m_kDirectPos, m_iItemID, m_iUsePlayer);
				}

			}
			else if (m_iTargetType == BCTT_MAPITEM)
			{
				BeMapItem* pkMapItem = gMapItemMgr.GetMapItemByID(m_iTargetID);
				if (pkMapItem)
				{
					bSucSet = pkTask->SpellTargetItem(m_iSkillTypeID, m_iSkillLevel, m_bExpendMP, m_kTargetPos, m_iItemID, m_iTargetID, m_iUsePlayer);
				}
				else
				{
					bSucSet = pkTask->SpellTargetPos(m_iSkillTypeID, m_iSkillLevel, m_bExpendMP, m_kTargetPos, m_kDirectPos, m_iItemID, m_iUsePlayer);
				}
			}
			else
			{
			}
		}
		else
		{
			bSucSet = pkTask->SpellTargetPos(m_iSkillTypeID, m_iSkillLevel, m_bExpendMP, m_kTargetPos, m_kDirectPos, m_iItemID, m_iUsePlayer);
		}

		m_pkCurTask = pkTask;
		if (!bSucSet)
		{
			SafeDeleteTask(m_pkCurTask);
			return BER_EXE_END;
		}
	}

	return m_pkCurTask->Execute(iDeltaTime);
}

bool BeSpellCommand::CanHungUp(BeGiveCmdType eCmdType, bool bNeedHangCurrent) const
{
	if (!m_pkCurTask)
	{
		return true;
	}
	if (m_pkCurTask->GetType() != STT_ACTION_SPELL)
	{
		return true;
	}
	else if (((BeTaskActionSpell*)m_pkCurTask)->GetPhase() == BSP_PREPARE && (eCmdType != BCT_HUNG_CURRENT || bNeedHangCurrent))
	{
		return true;
	}
	else if (((BeTaskActionSpell*)m_pkCurTask)->GetPhase() == BSP_CAST && (eCmdType != BCT_HUNG_CURRENT || bNeedHangCurrent))
	{
		{
			if (BCT_DIZZY_INTERRUPT == eCmdType)
			{
				return true;
			}
			return false;
		}
	}
	return false;
}

bool BeSpellCommand::CanCancel() const
{
	if (!m_pkCurTask)
	{
		return false;
	}

	if (m_pkCurTask->GetType() != STT_ACTION_SPELL)
	{
		return true;
	}
	else if (((BeTaskActionSpell*)m_pkCurTask)->GetPhase() == BSP_PREPARE)
	{
		return false;
	}
	else if (((BeTaskActionSpell*)m_pkCurTask)->GetPhase() == BSP_CAST)
	{
		const SkillTable* pkRes = ((BeTaskActionSpell*)m_pkCurTask)->GetSkillRes();
		if (pkRes && (pkRes->uiSkillProperty & SKILL_SKILLPROPERTY_KEDADUAN))
		{
			return true;
		}

		return false;
	}
	else if (((BeTaskActionSpell*)m_pkCurTask)->GetPhase() == BSP_EFFECT)
	{
		const SkillTable* pkRes = ((BeTaskActionSpell*)m_pkCurTask)->GetSkillRes();
		if (pkRes && (pkRes->uiSkillProperty & SKILL_SKILLPROPERTY_CHIXUSHIFA || pkRes->iShakesTime[0] > 0))
		{
			return false;
		}

		return true;
	}
	else if (((BeTaskActionSpell*)m_pkCurTask)->GetPhase() == BSP_SHAKES)
	{
		return false;
	}
	return true;
}
bool BeSpellCommand::CanInterrupt() const
{
	if (CanCancel())
	{
		return true;
	}
	if (!m_pkCurTask)
	{
		return false;
	}

	if (m_pkCurTask->GetType() != STT_ACTION_SPELL)
	{
		return true;
	}

	return false;
}

bool BeSpellCommand::CanSkip(void) const
{
	if (!m_pkCurTask)
	{
		return false;
	}
	if (m_pkCurTask->GetType() == STT_ACTION_SPELL
		&& ((BeTaskActionSpell*)m_pkCurTask)->CanSkip())
	{
		return true;
	}
	return false;
}

BeDropItemCommand::BeDropItemCommand()
{
	m_eCmdType = BCT_DROP_ITEM;
	m_iTargetID = 0;
	m_iItemID = 0;
}

void BeDropItemCommand::SetTargetIDDropItem(int iID, int iItemID)
{
	m_iItemID = iItemID;
	m_iTargetID = iID;
	SafeDeleteTask(m_pkCurTask);
	m_pkCurTask = BeTaskMoveToUnit::NEW();
	m_pkCurTask->AttachMain(pkAttachMain);
	m_pkCurTask->AttachUnit(pkAttachUnit);

	BeUnit* pkTarget = gUnitMgr.GetUnitByID(iID);

	((BeTaskMoveToUnit*)m_pkCurTask)->SetTargetID(iID, 1000.0f);
}

void BeDropItemCommand::SetTargetPosDropItem(const TePos2& kPos, int iItemID)
{
	m_iItemID = iItemID;
	m_kTargetPos = kPos;
	SafeDeleteTask(m_pkCurTask);
	m_pkCurTask = BeTaskMoveToPos::NEW();
	m_pkCurTask->AttachMain(pkAttachMain);
	m_pkCurTask->AttachUnit(pkAttachUnit);

	((BeTaskMoveToPos*)m_pkCurTask)->SetTargetPos(kPos, 1000.0f);
}

BeExeResult BeDropItemCommand::Execute(int& iDeltaTime)
{
	BeExeCommand::Execute(iDeltaTime);

	return BER_EXE_END;
}

bool BeDropItemCommand::CanHungUp(BeGiveCmdType eCmdType, bool bNeedHangCurrent) const
{
	if (!m_pkCurTask)
	{
		return false;
	}

	if (m_pkCurTask->GetType() == STT_MOVE_TO_POS || m_pkCurTask->GetType() == STT_MOVE_TO_UNIT)
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

BePickItemCommand::BePickItemCommand()
{
	m_eCmdType = BCT_PICK_ITEM;
	m_iItemID = 0;
}

bool BePickItemCommand::SetItemID(int iItemID)
{
	m_iItemID = iItemID;
	BeMapItem* pkMapItem = gMapItemMgr.GetMapItemByID(m_iItemID);
	if (pkMapItem)
	{
		m_kTargetPos.fX = pkMapItem->GetPosX();
		m_kTargetPos.fY = pkMapItem->GetPosY();
	}
	else
	{
		return false;
	}

	SafeDeleteTask(m_pkCurTask);
	m_pkCurTask = BeTaskMoveToPos::NEW();
	m_pkCurTask->AttachMain(pkAttachMain);
	m_pkCurTask->AttachUnit(pkAttachUnit);

	((BeTaskMoveToPos*)m_pkCurTask)->SetTargetPos(m_kTargetPos, 150.0f);
	return true;
}

BeExeResult BePickItemCommand::Execute(int& iDeltaTime)
{
	BeExeCommand::Execute(iDeltaTime);
	if (m_pkCurTask->Execute(iDeltaTime) == BER_EXE_END)
	{
		if (gUnit.IsGhost() || gUnit.IsDividMan())
		{
			return BER_EXE_END;
		}

		if (m_pkCurTask->GetType() == STT_MOVE_TO_POS)
		{
			if (((BeTaskMoveToPos*)m_pkCurTask)->GetMoveResult() == BMR_SUCCESS)
			{
				BeMapItem* pkMapItem = gMapItemMgr.GetMapItemByID(m_iItemID);
				if (!pkMapItem)
				{
					return BER_EXE_END;
				}

				gUnit.PickMapItem(pkMapItem);
				return BER_EXE_END;
			}
		}
	}
	return BER_TIME_OUT;
}

bool BePickItemCommand::CanHungUp(BeGiveCmdType eCmdType, bool bNeedHangCurrent) const
{
	if (!m_pkCurTask)
	{
		return false;
	}

	if (m_pkCurTask->GetType() == STT_MOVE_TO_POS || m_pkCurTask->GetType() == STT_MOVE_TO_UNIT)
	{
		return true;
	}
	return false;
}

bool BePickItemCommand::CanCancel() const
{
	return true;
}
bool BePickItemCommand::CanInterrupt() const
{
	return true;
}
