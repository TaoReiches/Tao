/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandSpell.h"
#include "Skill_table.hpp"
#include "TW_ShareUnitData.h"
#include "TW_Main.h"
#include "TW_Unit.h"
#include "TW_TaskMoveToUnit.h"
#include "TW_MemoryObject.h"
#include "TW_MapItem.h"
#include "TW_MapItemMgr.h"
#include "TW_TaskMoveToPos.h"
#include "TW_TaskActionSpell.h"
#include "TW_UnitMgr.h"

BeSpellCommand::BeSpellCommand()
{
	m_eCmdType = TwCommandType::BCT_SPELL;

	m_iSkillTypeID = 0;
	m_iSkillLevel = 1;
	m_bExpendMP = true;
	m_iTargetID = 0;
	m_iTargetType = BeCommandTargetType::BCTT_NONE;
	m_iItemID = 0;
	m_iUsePlayer = -1;
}

void BeSpellCommand::SpellTargetID(int iSkillTypeID, int iID, const TwPos2& kPos, int iSkillLevel, int iItemID, bool bExpendMP, int iUsePlayer, int iTargetType)
{
	auto& pkResData = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID);
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

	m_iTargetType = BeCommandTargetType::BCTT_UNIT;

	if (!m_pkCurTask || m_pkCurTask->GetType() != BeTaskType::STT_MOVE_TO_UNIT)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask.reset(dynamic_cast<BeTask*>(mpTaskMoveToUnit.alloc()));
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pkAttachUnit);
	}

	((BeTaskMoveToUnit*)m_pkCurTask.get())->SetTargetID(iID, (float)rkData.GetSpellDistance());

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
	if (!m_pkCurTask || m_pkCurTask->GetType() != BeTaskType::STT_MOVE_TO_POS)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask.reset(dynamic_cast<BeTask*>(mpTaskMoveToPos.alloc()));
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pkAttachUnit);
	}

	auto pkMapItem = gMapItemMgr.GetMapItemByID(iTargetID);
	if (!pkMapItem)
	{
		return;
	}
	TwPos2 kPos;
	kPos.fX = pkMapItem->GetPosX();
	kPos.fY = pkMapItem->GetPosY();

	m_iSkillTypeID = iSkillTypeID;
	m_iTargetID = iTargetID;
	m_kTargetPos = kPos;
	m_iSkillLevel = iSkillLevel;
	m_iItemID = iItemID;
	m_iUsePlayer = iUsePlayer;
	m_bExpendMP = bExpendMP;
	m_iTargetType = BeCommandTargetType::BCTT_MAPITEM;
}

void BeSpellCommand::SpellTargetPos(int iSkillTypeID, const TwPos2& kPos, const TwPos2& kDirPos, int iSkillLevel, int iItemID, bool bExpendMP, int iUsePlayer)
{
	if (!m_pkCurTask || m_pkCurTask->GetType() != BeTaskType::STT_MOVE_TO_POS)
	{
		SafeDeleteTask(m_pkCurTask);
		m_pkCurTask.reset(dynamic_cast<BeTask*>(mpTaskMoveToPos.alloc()));
		m_pkCurTask->AttachMain(pkAttachMain);
		m_pkCurTask->AttachUnit(pkAttachUnit);
	}

	auto& pkSkillRes = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID);
	if (!pkSkillRes)
	{
		return;
	}

	if (pkSkillRes->uiOperateType == SKILL_OPERATETYPE_SINGLETARGET)
	{
		SeCalSkillLvlData kData;
		if (!gUnit.GetSkillLvlData(kData, iSkillTypeID))
		{
			return;
		}
		((BeTaskMoveToPos*)m_pkCurTask.get())->SetTargetPos(kPos, (float)kData.GetSpellDistance());
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
		return BeExeResult::BER_EXE_END;
	}
	if (m_pkCurTask->GetType() != BeTaskType::STT_ACTION_SPELL)
	{
		auto& pkResData = SkillTableMgr::Get()->GetSkillTable(m_iSkillTypeID);
		if (!pkResData)
		{
			return BeExeResult::BER_EXE_END;
		}
		if (!gUnit.CanMove())
        {
			return BeExeResult::BER_EXE_END;
		}

		BeExeResult eResult = m_pkCurTask->Execute(iDeltaTime);
		if (eResult == BeExeResult::BER_TIME_OUT)
		{
			if (m_pkCurTask->GetType() == BeTaskType::STT_MOVE_TO_UNIT)
			{
				BeUnit* pkTarget = gUnitMgr.GetUnitByID(m_iTargetID);
				if (pkTarget)
				{
					m_kTargetPos = TwPos2(pkTarget->GetPosX(), pkTarget->GetPosY());
				}
			}
			return BeExeResult::BER_TIME_OUT;
		}

		if (m_pkCurTask->GetType() == BeTaskType::STT_MOVE_TO_UNIT)
		{
			if (m_iTargetID != 0)
			{
				if (((BeTaskMoveToUnit*)m_pkCurTask.get())->GetMoveResult() != BeMoveResult::BMR_SUCCESS)
				{
					{
						return BeExeResult::BER_EXE_END;
					}
				}
			}
		}
		else if (m_pkCurTask->GetType() == BeTaskType::STT_MOVE_TO_POS && pkResData->uiOperateType == SKILL_OPERATETYPE_SINGLETARGET)
		{
			if (((BeTaskMoveToPos*)m_pkCurTask.get())->GetMoveResult() != BeMoveResult::BMR_SUCCESS)
			{
				return BeExeResult::BER_EXE_END;
			}
		}

		SafeDeleteTask(m_pkCurTask);
		auto pkTask = std::unique_ptr<BeTaskActionSpell>(mpTaskActionSpell.alloc());
		pkTask->AttachMain(pkAttachMain);
		pkTask->AttachUnit(pkAttachUnit);

		bool	bSucSet = false;
		if (m_iTargetID)
		{
			if (m_iTargetType == BeCommandTargetType::BCTT_UNIT)
			{
				BeUnit* pkTarget = gUnitMgr.GetUnitByID(m_iTargetID);
				if (pkTarget)
				{
					bSucSet = pkTask->SpellTargetID(m_iSkillTypeID, m_iSkillLevel, m_bExpendMP, m_iTargetID, m_kTargetPos, m_iItemID, m_iUsePlayer, static_cast<int>(m_iTargetType));
				}
				else
				{
					bSucSet = pkTask->SpellTargetPos(m_iSkillTypeID, m_iSkillLevel, m_bExpendMP, m_kTargetPos, m_kDirectPos, m_iItemID, m_iUsePlayer);
				}

			}
			else if (m_iTargetType == BeCommandTargetType::BCTT_MAPITEM)
			{
				auto pkMapItem = gMapItemMgr.GetMapItemByID(m_iTargetID);
				if (pkMapItem)
				{
					bSucSet = pkTask->SpellTargetItem(m_iSkillTypeID, m_iSkillLevel, m_bExpendMP, m_kTargetPos, m_iItemID, m_iTargetID, m_iUsePlayer);
				}
				else
				{
					bSucSet = pkTask->SpellTargetPos(m_iSkillTypeID, m_iSkillLevel, m_bExpendMP, m_kTargetPos, m_kDirectPos, m_iItemID, m_iUsePlayer);
				}
			}
		}
		else
		{
			bSucSet = pkTask->SpellTargetPos(m_iSkillTypeID, m_iSkillLevel, m_bExpendMP, m_kTargetPos, m_kDirectPos, m_iItemID, m_iUsePlayer);
		}

		m_pkCurTask.reset(std::move(dynamic_cast<BeTask*>(pkTask.get())));
		if (!bSucSet)
		{
			SafeDeleteTask(m_pkCurTask);
			return BeExeResult::BER_EXE_END;
		}
	}

	return m_pkCurTask->Execute(iDeltaTime);
}

bool BeSpellCommand::CanHungUp(TwGiveCmdType eCmdType, bool bNeedHangCurrent) const
{
	if (!m_pkCurTask)
	{
		return true;
	}
	if (m_pkCurTask->GetType() != BeTaskType::STT_ACTION_SPELL)
	{
		return true;
	}
	else if (((BeTaskActionSpell*)m_pkCurTask.get())->GetPhase() == BeSpellPhase::BSP_PREPARE &&
		(eCmdType != TwGiveCmdType::BCT_HUNG_CURRENT || bNeedHangCurrent))
	{
		return true;
	}
	else if (((BeTaskActionSpell*)m_pkCurTask.get())->GetPhase() == BeSpellPhase::BSP_CAST &&
		(eCmdType != TwGiveCmdType::BCT_HUNG_CURRENT || bNeedHangCurrent))
	{
		{
			if (TwGiveCmdType::BCT_DIZZY_INTERRUPT == eCmdType)
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

	if (m_pkCurTask->GetType() != BeTaskType::STT_ACTION_SPELL)
	{
		return true;
	}
	else if (((BeTaskActionSpell*)m_pkCurTask.get())->GetPhase() == BeSpellPhase::BSP_PREPARE)
	{
		return false;
	}
	else if (((BeTaskActionSpell*)m_pkCurTask.get())->GetPhase() == BeSpellPhase::BSP_CAST)
	{
		auto& pkRes = ((BeTaskActionSpell*)m_pkCurTask.get())->GetSkillRes();
		if (pkRes)
		{
			return true;
		}

		return false;
	}
	else if (((BeTaskActionSpell*)m_pkCurTask.get())->GetPhase() == BeSpellPhase::BSP_EFFECT)
	{
		auto& pkRes = ((BeTaskActionSpell*)m_pkCurTask.get())->GetSkillRes();
		if (pkRes && (pkRes->iShakesTime[0] > 0))
		{
			return false;
		}

		return true;
	}
	else if (((BeTaskActionSpell*)m_pkCurTask.get())->GetPhase() == BeSpellPhase::BSP_SHAKES)
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

	if (m_pkCurTask->GetType() != BeTaskType::STT_ACTION_SPELL)
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
	if (m_pkCurTask->GetType() == BeTaskType::STT_ACTION_SPELL
		&& ((BeTaskActionSpell*)m_pkCurTask.get())->CanSkip())
	{
		return true;
	}
	return false;
}

