/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Commander.h"
#include "TW_Skill.h"
#include "TW_Main.h"
#include "TW_Unit.h"
#include "TW_UnitMgr.h"
#include "TW_Item.h"
#include "TW_TriggerEvent.h"
#include "TW_MapItem.h"
#include "TW_MapItemMgr.h"
#include "Skill_table.hpp"
#include "Item_table.hpp"

BeCommander::BeCommander()
{
	m_pkCurCmd = NULL;
	m_bForceNexeCmd = false;
	m_kCommands.clear();
}

BeCommander::~BeCommander()
{
	SafeDeleteCommand(m_pkCurCmd);
}

void BeCommander::OnDelete(void)
{
	SafeDeleteCommand(m_pkCurCmd);
}

BeExeCommand* BeCommander::GetCurCmd(void)
{
	return m_pkCurCmd;
}

int	BeCommander::GetCommandCount() const
{
	return (int)m_kCommands.size();
}

BeCommand& BeCommander::GetCurCommand(void)
{
	return m_kCurCmd;
}

std::list<BeCommand>& BeCommander::GetCommands(void)
{
	return m_kCommands;
}

void BeCommander::SetCommands(const std::list<BeCommand>& rkCommands)
{
	m_kCommands.clear();
	std::copy(rkCommands.begin(), rkCommands.end(), std::back_inserter(m_kCommands));
}

bool BeCommander::IsIdle(void) const
{
	if (m_kCommands.empty() && ((m_pkCurCmd && m_pkCurCmd->GetType() == BCT_STOP) || !m_pkCurCmd))
	{
		return true;
	}
	return false;
}

void BeCommander::GiveCommand(BeCommand kCmd, BeGiveCmdType eType, int iUnitID, bool bNeedHangCurrent)
{
	kCmd.eGiveType = eType;

	BeGiveCmdType& reType = kCmd.eGiveType;
	switch (reType)
	{
	case BCT_PLAYER_SHIFT:
	case BCT_SYSTEM_SHIFT:
	{
		m_kCommands.push_back(kCmd);
		break;
	}
	case BCT_SYSTEM_CLEAR:
	{
		for (std::list<BeCommand>::iterator it = m_kCommands.begin(); it != m_kCommands.end();)
		{
			std::list<BeCommand>::iterator itDel = it;
			++it;

			if (itDel->eGiveType == BCT_SYSTEM_SHIFT)
			{
				continue;
			}

			m_kCommands.erase(itDel);
		}

		m_kCommands.insert(m_kCommands.begin(), kCmd);
		m_bForceNexeCmd = true;
		break;
	}
	case BCT_PLAYER_CMD:
	{
		for (std::list<BeCommand>::iterator it = m_kCommands.begin(); it != m_kCommands.end();)
		{
			std::list<BeCommand>::iterator itDel = it;
			++it;
			if (itDel->eCmdType < reType)
			{
				if (itDel->eGiveType == BCT_SYSTEM_SHIFT)
				{
					continue;
				}
				m_kCommands.erase(itDel);
			}
		}

		m_kCommands.push_back(kCmd);
		break;
	}
	case BCT_PLAYER_ACTION:
	{
		if (!m_pkCurCmd)
		{
			m_bForceNexeCmd = true;
		}

		else if (!m_pkCurCmd->CanCancel())
		{
			m_bForceNexeCmd = false;
		}
		else if (m_pkCurCmd->CanInterrupt() && kCmd.eCmdType == BCT_STOP)
		{
			m_bForceNexeCmd = true;
		}
		else if (kCmd.eCmdType == BCT_STOP)
		{
			m_bForceNexeCmd = true;
			reType = BCT_PLAYER_CMD;

			if (!m_pkCurCmd->CanCancel())
			{
				m_bForceNexeCmd = false;
			}
		}
		else if (kCmd.eCmdType == BCT_MOVE || kCmd.eCmdType == BCT_MOVE_DIRECT)
		{
			if (m_pkCurCmd->GetType() == BCT_MOVE || m_pkCurCmd->GetType() == BCT_MOVE_DIRECT)
			{
				m_bForceNexeCmd = true;
				reType = BCT_PLAYER_CMD;
			}
			else if (!m_pkCurCmd->CanCancel())
			{
				m_bForceNexeCmd = false;
			}
			else
			{
				m_bForceNexeCmd = true;
			}
		}
		else
		{
			m_bForceNexeCmd = true;
		}

		if (reType != BCT_PLAYER_INSERT || !m_bForceNexeCmd)
		{
			for (std::list<BeCommand>::iterator it = m_kCommands.begin(); it != m_kCommands.end();)
			{
				std::list<BeCommand >::iterator itDel = it;
				++it;
				BeGiveCmdType eCmpType = reType;
				if (reType == BCT_PLAYER_ACTION)
				{
					eCmpType = BCT_PLAYER_CMD;
				}
				if (kCmd.eCmdType == BCT_STOP)
				{
					eCmpType = BCT_SKILL_ADDON;
				}

				if (m_pkCurCmd && !m_pkCurCmd->CanInterrupt())
				{
					eCmpType = BCT_SKILL_ADDON;
				}
				if (itDel->eGiveType < eCmpType)
				{
					if (itDel->eGiveType == BCT_SYSTEM_SHIFT)
					{
						continue;
					}

					if (itDel->eCmdType == BCT_MOVE_DIRECT && (kCmd.eCmdType != BCT_MOVE_DIRECT && kCmd.eCmdType != BCT_STOP))
					{
						continue;
					}
					m_kCommands.erase(itDel);
				}
			}

			if (kCmd.eCmdType != BCT_SPELL && kCmd.eCmdType != BCT_USE_ITEM)
			{
				reType = BCT_PLAYER_REDUCE;
			}

			m_kCommands.push_back(kCmd);

			if (kCmd.eCmdType == BCT_SPELL)
			{
				BeSkill* pkSkill = gUnit.GetSkill(kCmd.iData);
				if (pkSkill)
				{
					const	SkillTable* pkSkillRes = pkSkill->GetResPtr();
					if (pkSkillRes)
					{
						if (pkSkillRes->uiAfterActionType == SKILL_AFTERACTIONTYPE_ATTACKTARGET)
						{
							BeUnit* pkTarget = gUnitMgr.GetUnitByID(kCmd.iUnitID);
							if (pkTarget && pkTarget->GetCamp() != gUnit.GetCamp())
							{
								BeCommand	kComm(BCT_ATTACK, pkTarget->GetID());
								m_kCommands.push_back(kComm);
							}
						}
						else if (pkSkillRes->uiAfterActionType == SKILL_AFTERACTIONTYPE_DOLAST)
						{
							if (m_kCurCmd.eCmdType == BCT_MOVE)
							{
								m_kCommands.push_back(m_kCurCmd);
							}
							else if (m_kCurCmd.eCmdType == BCT_ATTACK) {

								BeUnit* pkTarget = gUnitMgr.GetUnitByID(m_kCurCmd.iUnitID);

								if (pkTarget && !pkTarget->IsDead()) {
									m_kCommands.push_back(m_kCurCmd);
								}
							}
						}
					}
				}
			}
			if (kCmd.eCmdType == BCT_USE_ITEM)
			{
				do
				{
					BeItem* pkItem = gUnit.GetItemByID(kCmd.iData);
					if (!pkItem)
					{
						break;
					}
					int iSkillTypeID = gUnit.GetItemSkillTypeID(kCmd.iData);
					BeSkill* pkSkill = pkItem->GetSkillByTypeID(iSkillTypeID);
					if (!pkSkill)
					{
						break;
					}
					const	SkillTable* pkSkillRes = pkSkill->GetResPtr();
					if (pkSkillRes)
					{
						if (pkSkillRes->uiAfterActionType == SKILL_AFTERACTIONTYPE_ATTACKTARGET)
						{
							BeUnit* pkTarget = gUnitMgr.GetUnitByID(kCmd.iUnitID);
							if (pkTarget)
							{
								BeCommand	kComm(BCT_ATTACK, pkTarget->GetID());
								m_kCommands.push_back(kComm);
							}
						}
						else if (pkSkillRes->uiAfterActionType == SKILL_AFTERACTIONTYPE_DOLAST)
						{
							if (m_kCurCmd.eCmdType == BCT_MOVE || m_kCurCmd.eCmdType == BCT_ATTACK)
							{
								m_kCommands.push_back(m_kCurCmd);
							}
						}
					}
				} while (0);
			}
			break;
		}
	}
	case BCT_PLAYER_INSERT:
	{
		if (!m_pkCurCmd)
		{
			m_kCommands.insert(m_kCommands.begin(), kCmd);
			m_bForceNexeCmd = true;
			break;
		}
		else if (m_pkCurCmd->CanHungUp(reType, bNeedHangCurrent))
		{
			m_kCurCmd.eGiveType = BCT_PLAYER_SHIFT;
			std::list<BeCommand>::iterator it = m_kCommands.begin();
			for (; it != m_kCommands.end(); ++it)
			{
				if (it->eGiveType < reType)
				{
					break;
				}
			}
			m_kCommands.insert(it, kCmd);
			m_kCommands.insert(it, m_kCurCmd);
			m_bForceNexeCmd = true;

			break;
		}
		else
		{
			if (m_pkCurCmd->CanCancel())
			{
				std::list<BeCommand>::iterator it = m_kCommands.begin();
				for (; it != m_kCommands.end(); ++it)
				{
					if (it->eGiveType < reType)
					{
						break;
					}
				}
				m_kCommands.insert(it, kCmd);
				m_bForceNexeCmd = true;
				break;
			}
		}
		break;
	}
	case BCT_PLAYER_HUNG:
	{
		std::list<BeCommand>::iterator it = m_kCommands.begin();
		for (; it != m_kCommands.end(); ++it)
		{
			if (it->eGiveType < reType)
			{
				break;
			}
		}
		m_kCommands.insert(it, kCmd);
		break;
	}
	case BCT_DEATH:
	{
		m_kCommands.clear();
		m_kCommands.insert(m_kCommands.begin(), kCmd);
		m_bForceNexeCmd = true;
		break;
	}
	case BCT_DIZZY_INTERRUPT:
	{
		m_bForceNexeCmd = true;
	}
	case BCT_SKILL_ADDON:
	{
		if (m_pkCurCmd && m_pkCurCmd->CanHungUp(reType, bNeedHangCurrent))
		{
			m_kCurCmd.eGiveType = BCT_PLAYER_SHIFT;
			m_kCommands.insert(m_kCommands.begin(), m_kCurCmd);
			m_kCommands.insert(m_kCommands.begin(), kCmd);

			if (gUnit.HasProperty(1 << 15))
			{
				for (std::list<BeCommand>::iterator it = m_kCommands.begin(); it != m_kCommands.end();)
				{
					std::list<BeCommand>::iterator itDel = it;
					++it;
					if (itDel->eCmdType == BCT_SPELL)
					{
						m_kCommands.erase(itDel);
					}
				}
			}
		}
		else
		{
			std::list<BeCommand>::iterator it = m_kCommands.begin();
			for (; it != m_kCommands.end(); ++it)
			{
				if (it->eGiveType < reType)
				{
					break;
				}
			}
			m_kCommands.insert(it, kCmd);
		}
		break;
	}
	default:break;
	}
	if (m_kCommands.size())
	{
		m_kCommands.front().bForceExecute = m_bForceNexeCmd;
	}
}

void BeCommander::ExecuteCmd(int iDeltaTime)
{
	if (m_kCommands.size() && m_kCommands.front().bForceExecute)
	{
		if (GoNextCmd())
		{
			m_bForceNexeCmd = false;
		}
	}
	int iLoopCount = 20;
	while (iDeltaTime > 0)
	{
		--iLoopCount;
		if (iLoopCount <= 0)
		{
			iDeltaTime = 0;
			break;
		}
		BeExeResult eResult = BER_EXE_END;
		if (m_pkCurCmd)
		{
			eResult = m_pkCurCmd->Execute(iDeltaTime);
		}
		bool bSkip = (m_pkCurCmd ? m_pkCurCmd->CanSkip() : true) && m_kCommands.size();

		if (gUnit.HasUnitCarryFlag(BUCF_DIZZY) && m_pkCurCmd->GetType() != BCT_STOP)
		{
			eResult = BER_EXE_END;
		}
		if (eResult == BER_EXE_END)
		{
			if (GoNextCmd())
			{
				m_bForceNexeCmd = false;
			}
		}
		else if (bSkip)
		{
			if (GoNextCmd(true))
			{
				m_bForceNexeCmd = false;
			}
		}
		else if (eResult == BER_ALL_OVER)
		{
			gUnit.SetFlag(BUF_REMOVE);
			m_bForceNexeCmd = true;
			SafeDeleteCommand(m_pkCurCmd);
			m_kCommands.clear();
			break;
		}
	}
}

bool BeCommander::SwitchCmd(const BeCommand& kCmd, bool bConnect)
{
	switch (kCmd.eCmdType)
	{
	case BCT_STOP:
	{
		gUnit.SetAttackingUnitID(0);
		BeStopCommand* pkCmd = NULL;
		if (m_pkCurCmd && m_pkCurCmd->GetType() == BCT_STOP)
		{
			BeStopCommand* pkStop = (BeStopCommand*)m_pkCurCmd;
			if (!pkStop->IsDead() || gUnit.IsDead())
			{
				pkCmd = (BeStopCommand*)m_pkCurCmd;
			}
		}

		if (!pkCmd)
		{
			SafeDeleteCommand(m_pkCurCmd);
			pkCmd = BeStopCommand::NEW();
			pkCmd->AttachMain(pkAttachMain);
			pkCmd->AttachUnit(pkAttachUnit);
		}

		m_pkCurCmd = pkCmd;

		if (kCmd.iUnitID == -1)
		{
			if (gUnit.IsDividMan() || gUnit.IsGhost())
			{
				gUnitMgr.DelUnit(gUnit.GetID());
			}
			else
			{
				((BeStopCommand*)m_pkCurCmd)->SetDead();
			}
		}
		else
		{
			((BeStopCommand*)m_pkCurCmd)->SetStopTime(kCmd.iData);
		}
		break;
	}
	case BCT_HOLD:
	{
		gUnit.SetAttackingUnitID(0);
		SafeDeleteCommand(m_pkCurCmd);

		m_pkCurCmd = BeHoldCommand::NEW();
		m_pkCurCmd->AttachMain(pkAttachMain);
		m_pkCurCmd->AttachUnit(pkAttachUnit);
		break;
	}
	case BCT_MOVE:
	{
		gUnit.SetAttackingUnitID(0);
		BeMoveCommand* pkCmd = NULL;
		if (m_pkCurCmd && m_pkCurCmd->GetType() == BCT_MOVE)
		{
			pkCmd = (BeMoveCommand*)m_pkCurCmd;
		}

		if (!pkCmd)
		{
			SafeDeleteCommand(m_pkCurCmd);
			pkCmd = BeMoveCommand::NEW();
			pkCmd->AttachMain(pkAttachMain);
			pkCmd->AttachUnit(pkAttachUnit);
		}

		if (kCmd.iUnitID)
		{
			pkCmd->SetFollowID(kCmd.iUnitID, (float)kCmd.iData);
		}
		else
		{
			pkCmd->SetTargetPos(kCmd.kPos, (float)kCmd.iData, kCmd.iData2 != 0);
		}

		m_pkCurCmd = pkCmd;
		break;
	}
	case BCT_PATROL:
	{
		BePatrolCommand* pkCmd = NULL;
		if (m_pkCurCmd && m_pkCurCmd->GetType() == BCT_PATROL)
		{
			pkCmd = (BePatrolCommand*)m_pkCurCmd;
		}

		if (!pkCmd)
		{
			SafeDeleteCommand(m_pkCurCmd);
			pkCmd = BePatrolCommand::NEW();
			pkCmd->AttachMain(pkAttachMain);
			pkCmd->AttachUnit(pkAttachUnit);
		}

		pkCmd->SetPatrol(kCmd.kPos, TePos2(gUnit.GetPosX(), gUnit.GetPosY()));
		m_pkCurCmd = pkCmd;
		((BePatrolCommand*)m_pkCurCmd)->SetStopTime(kCmd.iData);
		break;
	}
	case BCT_ATTACK:
	{
		BeAttackCommand* pkCmd = NULL;
		if (m_pkCurCmd && (m_pkCurCmd->GetType() == BCT_ATTACK && kCmd.bForceAttackOnce == false))
		{
			pkCmd = (BeAttackCommand*)m_pkCurCmd;
		}

		if (!pkCmd)
		{
			SafeDeleteCommand(m_pkCurCmd);
			pkCmd = BeAttackCommand::NEW();
			pkCmd->AttachMain(pkAttachMain);
			pkCmd->AttachUnit(pkAttachUnit);
		}

		if (kCmd.iUnitID)
		{
			switch (kCmd.iData)
			{
			case BCTT_MAPITEM:pkCmd->SetTargetItem(kCmd.iUnitID); break;
			default:pkCmd->SetTargetID(kCmd.iUnitID, 0.0f, false, 0, 0, kCmd.iData2); break;
			}
		}
		else
		{
			pkCmd->SetTargetPos(kCmd.kPos, (float)kCmd.iData);
		}

		m_pkCurCmd = pkCmd;
		break;
	}
	case BCT_SPELL:
	{
		SeCalSkillLvlData kData;
		BeSkill* pkSkill = nullptr;
		pkSkill = gUnit.GetSkill(kCmd.iData);
		if (!pkSkill)
		{
			return false;
		}

		if (!gUnit.GetSkillLvlData(kData, kCmd.iData))
		{
			return false;
		}

		if (pkSkill->IsSwitch() && pkSkill->IsActive())
		{
		}
		else if (!pkSkill->CDComplete(&gUnit))
		{
			return false;
		}

		if (pkSkill->HasProperty(SKILL_SKILLPROPERTY_DISPLACE) && gUnit.HasUnitCarryFlag(BUCF_CANNOTMOVE)) {

			return false;
		}

		if (pkSkill->IsSwitch())
		{
			const SkillTable* pkSkillRes = pkSkill->GetResPtr();
			if (pkSkillRes)
			{
				SeCalSkillLvlData& rkLvlData = kData;
				pkSkill->SetActive(!pkSkill->IsActive());
				if (pkSkill->IsActive())
				{
					gUnit.SetMP(gUnit.GetMP() - rkLvlData.GetSkillManaSpend());
					pkSkill->SetLastUseTime(&gUnit, gTime);
					pkSkill->SetUICounter(1);

					gUnit.TrgOnSpell(pkSkill->GetTypeID(), pkSkill->GetLevel(), 0, 0, gUnit.GetPosX(), gUnit.GetPosY());

					TePtParam kParam;
					kParam.SetParam(BTP_pkTrgUnit, &gUnit);
					kParam.SetParam(BTP_iSkillTypeID, pkSkill->GetTypeID());
					kParam.SetParam(BTP_iSkillLevel, pkSkill->GetLevel());
					gTrgMgr.FireTrigger(BTE_SPELL_EFFECT, kParam);
				}
				else
				{
					pkSkill->SetUICounter(0);

					TePtParam kParam;
					kParam.SetParam(BTP_pkTrgUnit, &gUnit);
					kParam.SetParam(BTP_iSkillTypeID, pkSkill->GetTypeID());
					kParam.SetParam(BTP_iSkillLevel, pkSkill->GetLevel());
					gTrgMgr.FireTrigger(BTE_SPELL_CLOSE, kParam);
				}
			}
			break;
		}

		gUnit.SetAttackingUnitID(0);
		SafeDeleteCommand(m_pkCurCmd);
		BeSpellCommand* pkCmd = BeSpellCommand::NEW();
		pkCmd->AttachMain(pkAttachMain);
		pkCmd->AttachUnit(pkAttachUnit);
		{
			if (kCmd.iUnitID)
			{
				pkCmd->SpellTargetID(kCmd.iData, kCmd.iUnitID, kCmd.kPos, pkSkill->GetLevel(), 0, 1, kCmd.iData2, kCmd.eTargetType);
			}
			else
			{
				pkCmd->SpellTargetPos(kCmd.iData, kCmd.kPos, kCmd.kDirPos, pkSkill->GetLevel(), 0, 1, kCmd.iData2);
			}
		}
		m_pkCurCmd = pkCmd;

		break;

	}
	case BCT_USE_ITEM:
	{
		gUnit.SetAttackingUnitID(0);
		SafeDeleteCommand(m_pkCurCmd);

		BeItem* pkItem = gUnit.GetItemByID(kCmd.iData);
		if (!pkItem)
		{
			return false;
		}
		const ItemTable* pkItemRes = ItemTableMgr::Get()->GetItemTable(pkItem->GetTypeID());
		if (!pkItemRes)
		{
			return false;
		}

		int iSkillTypeID = gUnit.GetItemSkillTypeID(kCmd.iData);

		const SkillTable* pkRes = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID);
		if (!pkRes)
		{
			return false;
		}
		SeCalSkillLvlData kData;
		if (!gUnit.GetSkillLvlData(kData, iSkillTypeID))
		{
			return false;
		}

		BeSpellCommand* pkCmd = BeSpellCommand::NEW();
		pkCmd->AttachMain(pkAttachMain);
		pkCmd->AttachUnit(pkAttachUnit);

		if (pkRes->uiOperateType == SKILL_OPERATETYPE_LIJISHIFA)
		{
			pkCmd->SpellTargetID(iSkillTypeID, kCmd.iUnitID, kCmd.kPos, 1, kCmd.iData);
		}
		else
		{
			BeUnit* pkCmdUnit = gUnitMgr.GetUnitByID(kCmd.iUnitID);
			if (pkCmdUnit)
			{
				pkCmd->SpellTargetID(iSkillTypeID, kCmd.iUnitID, kCmd.kPos, 1, kCmd.iData);
			}
			else
			{
				BeMapItem* pkMapItem = gMapItemMgr.GetMapItemByID(kCmd.iUnitID);
				if (pkMapItem)
				{
					pkCmd->SpellTargetItem(iSkillTypeID, kCmd.iUnitID, 1, kCmd.iData);
				}
				else
				{
					pkCmd->SpellTargetPos(iSkillTypeID, kCmd.kPos, kCmd.kDirPos, 1, kCmd.iData, true, -1);
				}
			}
		}

		m_pkCurCmd = pkCmd;
		break;
	}
	case BCT_DROP_ITEM:
	{
		gUnit.SetAttackingUnitID(0);
		SafeDeleteCommand(m_pkCurCmd);
		BeDropItemCommand* pkCmd = BeDropItemCommand::NEW();
		pkCmd->AttachMain(pkAttachMain);
		pkCmd->AttachUnit(pkAttachUnit);
		if (kCmd.iUnitID)
		{
			pkCmd->SetTargetIDDropItem(kCmd.iUnitID, kCmd.iData);
		}
		else
		{
			pkCmd->SetTargetPosDropItem(kCmd.kPos, kCmd.iData);
		}

		m_pkCurCmd = pkCmd;
		break;
	}
	case BCT_PICK_ITEM:
	{
		gUnit.SetAttackingUnitID(0);
		BePickItemCommand* pkCmd = BePickItemCommand::NEW();
		pkCmd->AttachMain(pkAttachMain);
		pkCmd->AttachUnit(pkAttachUnit);

		if (pkCmd->SetItemID(kCmd.iData))
		{
			SafeDeleteCommand(m_pkCurCmd);
			m_pkCurCmd = pkCmd;
		}
		else
		{
			BePickItemCommand::DEL(pkCmd);
		}
		break;
	}
	default:
	{
		return false;
	}
	}
	///	gUnit.SetHighChangeFlag(SHFCF_CMD);
	m_kCurCmd = kCmd;
	m_kCurCmd.bForceExecute = false;

	return true;
}

bool BeCommander::GoNextCmd(bool bSkipCurrent)
{
	if (!gUnit.IsDead() && (gUnit.HasUnitCarryFlag(BUCF_DIZZY)))
	{
		bool bSkip = false;

		if (!bSkip)
		{
			if (m_pkCurCmd)
			{
				if (m_pkCurCmd->GetType() != BCT_STOP)
				{
					if (!m_kCommands.empty())
					{
						BeCommand& kCmd = m_kCommands.front();
						if (kCmd.eCmdType == BCT_STOP)
						{
							m_kCommands.pop_front();
						}
					}
					BeCommand kStop(BCT_STOP);
					SwitchCmd(kStop, false);
				}
			}
			else
			{
				BeCommand kStop(BCT_STOP);
				SwitchCmd(kStop, false);
			}

			return false;
		}
	}

	while (!m_kCommands.empty())
	{
		BeCommand kCmd = m_kCommands.front();
		m_kCommands.pop_front();

		bool bDelay = false;
		if (kCmd.eGiveType == BCT_PLAYER_SHIFT)
		{
			if ((kCmd.eCmdType == BCT_SPELL && gUnit.HasUnitCarryFlag(BUCF_ISFORBIDSKILL))
				|| (kCmd.eCmdType == BCT_USE_ITEM && gUnit.HasUnitCarryFlag(BUCF_ISFORBIDITEM))
				|| gUnit.HasUnitCarryFlag(BUCF_CANNOTCONTROL)
				|| gUnit.HasUnitCarryFlag(BUCF_DIZZY)
				|| gUnit.HasUnitCarryFlag(BUF_SPELL_SHARK)
				)
			{
				bDelay = true;
			}
		}

		if (bDelay)
		{
			m_kCommands.push_front(kCmd);
			if (bSkipCurrent)
			{
				return false;
			}
			else
			{
				kCmd = BeCommand(BCT_STOP, 0, 0.0f, 0.0f, 100);
			}
		}

		if (SwitchCmd(kCmd))
		{
			return true;
		}
	}

	if (m_pkCurCmd && (m_pkCurCmd->GetType() == BCT_MOVE || m_pkCurCmd->GetType() == BCT_MOVE_DIRECT))
	{
		BeCommand kStop(BCT_STOP, 0, 0.0f, 0.0f, 250);
		SwitchCmd(kStop, false);
	}
	else
	{
		BeCommand kStop(BCT_STOP);
		SwitchCmd(kStop, false);
	}
	return true;
}

void BeCommander::SafeDeleteCommand(BeExeCommand*& pkCommand)
{
	if (pkCommand)
	{
		switch (pkCommand->GetType())
		{
		case BCT_STOP:			BeStopCommand::DEL((BeStopCommand*)pkCommand);				break;
		case BCT_HOLD:			BeHoldCommand::DEL((BeHoldCommand*)pkCommand);				break;
		case BCT_MOVE:			BeMoveCommand::DEL((BeMoveCommand*)pkCommand);				break;
		case BCT_PATROL:		BePatrolCommand::DEL((BePatrolCommand*)pkCommand);			break;
		case BCT_ATTACK:		BeAttackCommand::DEL((BeAttackCommand*)pkCommand);			break;
		case BCT_SPELL:			BeSpellCommand::DEL((BeSpellCommand*)pkCommand);			break;
		case BCT_DROP_ITEM:		BeDropItemCommand::DEL((BeDropItemCommand*)pkCommand);		break;
		case BCT_PICK_ITEM:		BePickItemCommand::DEL((BePickItemCommand*)pkCommand);		break;
		default:
		{
		}
		break;
		}
		pkCommand = NULL;
	}
}

void BeCommander::ClearAllCommands()
{
	m_kCommands.clear();
}
