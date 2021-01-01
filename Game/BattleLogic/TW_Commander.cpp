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
#include "TW_CommandStop.h"
#include "TW_CommandMove.h"
#include "TW_MemoryObject.h"
#include "TW_CommandAttack.h"
#include "TW_CommandSpell.h"
#include "TW_CommandDropItem.h"
#include "TW_CommandPickItem.h"
#include "TW_TriggerMgr.h"
#include <iterator>

TwCommander::TwCommander()
{
    m_pkCurCmd = nullptr;
    m_bForceNexeCmd = false;
    m_kCommands.clear();
    pkAttachUnit = nullptr;
}

TwCommander::~TwCommander()
{
	SafeDeleteCommand(m_pkCurCmd);
}

void TwCommander::OnDelete(void)
{
	SafeDeleteCommand(m_pkCurCmd);
}

BeExeCommand* TwCommander::GetCurCmd(void)
{
	return m_pkCurCmd;
}

int	TwCommander::GetCommandCount() const
{
	return (int)m_kCommands.size();
}

TwCommand& TwCommander::GetCurCommand(void)
{
	return m_kCurCmd;
}

std::list<TwCommand>& TwCommander::GetCommands(void)
{
	return m_kCommands;
}

void TwCommander::SetCommands(const std::list<TwCommand>& rkCommands)
{
	m_kCommands.clear();
	std::copy(rkCommands.begin(), rkCommands.end(), std::back_inserter(m_kCommands));
}

bool TwCommander::IsIdle(void) const
{
	if (m_kCommands.empty() && ((m_pkCurCmd && m_pkCurCmd->GetType() == TwCommandType::BCT_STOP) || !m_pkCurCmd))
	{
		return true;
	}
	return false;
}

void TwCommander::GiveCommand(TwCommand kCmd, TwGiveCmdType eType, int iUnitID, bool bNeedHangCurrent)
{
	kCmd.eGiveType = eType;

	TwGiveCmdType& reType = kCmd.eGiveType;
	switch (reType)
	{
	case TwGiveCmdType::BCT_PLAYER_SHIFT:
	case TwGiveCmdType::BCT_SYSTEM_SHIFT:
	{
		m_kCommands.push_back(kCmd);
		break;
	}
	case TwGiveCmdType::BCT_SYSTEM_CLEAR:
	{
		for (std::list<TwCommand>::iterator it = m_kCommands.begin(); it != m_kCommands.end();)
		{
			std::list<TwCommand>::iterator itDel = it;
			++it;

			if (itDel->eGiveType == TwGiveCmdType::BCT_SYSTEM_SHIFT)
			{
				continue;
			}

			m_kCommands.erase(itDel);
		}

		m_kCommands.insert(m_kCommands.begin(), kCmd);
		m_bForceNexeCmd = true;
		break;
	}
	case TwGiveCmdType::BCT_PLAYER_CMD:
	{
		for (std::list<TwCommand>::iterator it = m_kCommands.begin(); it != m_kCommands.end();)
		{
			std::list<TwCommand>::iterator itDel = it;
			++it;
			if ((int)itDel->eCmdType < (int)reType)
			{
				if (itDel->eGiveType == TwGiveCmdType::BCT_SYSTEM_SHIFT)
				{
					continue;
				}
				m_kCommands.erase(itDel);
			}
		}

		m_kCommands.push_back(kCmd);
		break;
	}
	case TwGiveCmdType::BCT_PLAYER_ACTION:
	{
		if (!m_pkCurCmd)
		{
			m_bForceNexeCmd = true;
		}

		else if (!m_pkCurCmd->CanCancel())
		{
			m_bForceNexeCmd = false;
		}
		else if (m_pkCurCmd->CanInterrupt() && kCmd.eCmdType == TwCommandType::BCT_STOP)
		{
			m_bForceNexeCmd = true;
		}
		else if (kCmd.eCmdType == TwCommandType::BCT_STOP)
		{
			m_bForceNexeCmd = true;
			reType = TwGiveCmdType::BCT_PLAYER_CMD;

			if (!m_pkCurCmd->CanCancel())
			{
				m_bForceNexeCmd = false;
			}
		}
		else if (kCmd.eCmdType == TwCommandType::BCT_MOVE || kCmd.eCmdType == TwCommandType::BCT_MOVE_DIRECT)
		{
			if (m_pkCurCmd->GetType() == TwCommandType::BCT_MOVE || m_pkCurCmd->GetType() == TwCommandType::BCT_MOVE_DIRECT)
			{
				m_bForceNexeCmd = true;
				reType = TwGiveCmdType::BCT_PLAYER_CMD;
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

		if (reType != TwGiveCmdType::BCT_PLAYER_INSERT || !m_bForceNexeCmd)
		{
			for (std::list<TwCommand>::iterator it = m_kCommands.begin(); it != m_kCommands.end();)
			{
				std::list<TwCommand >::iterator itDel = it;
				++it;
				TwGiveCmdType eCmpType = reType;
				if (reType == TwGiveCmdType::BCT_PLAYER_ACTION)
				{
					eCmpType = TwGiveCmdType::BCT_PLAYER_CMD;
				}
				if (kCmd.eCmdType == TwCommandType::BCT_STOP)
				{
					eCmpType = TwGiveCmdType::BCT_SKILL_ADDON;
				}

				if (m_pkCurCmd && !m_pkCurCmd->CanInterrupt())
				{
					eCmpType = TwGiveCmdType::BCT_SKILL_ADDON;
				}
				if (itDel->eGiveType < eCmpType)
				{
					if (itDel->eGiveType == TwGiveCmdType::BCT_SYSTEM_SHIFT)
					{
						continue;
					}

					if (itDel->eCmdType == TwCommandType::BCT_MOVE_DIRECT && (kCmd.eCmdType != TwCommandType::BCT_MOVE_DIRECT && kCmd.eCmdType != TwCommandType::BCT_STOP))
					{
						continue;
					}
					m_kCommands.erase(itDel);
				}
			}

			if (kCmd.eCmdType != TwCommandType::BCT_SPELL && kCmd.eCmdType != TwCommandType::BCT_USE_ITEM)
			{
				reType = TwGiveCmdType::BCT_PLAYER_REDUCE;
			}

			m_kCommands.push_back(kCmd);

			if (kCmd.eCmdType == TwCommandType::BCT_SPELL)
			{
				BeSkill* pkSkill = gUnit->GetSkill(kCmd.iData);
				if (pkSkill)
				{
					auto& pkSkillRes = pkSkill->GetSkillRes();
					if (pkSkillRes)
					{
						if (pkSkillRes->uiAfterActionType == SKILL_AFTERACTIONTYPE_ATTACKTARGET)
						{
							std::shared_ptr<BeUnit> pkTarget = gUnitMgr->GetUnitByID(kCmd.iUnitID);
							if (pkTarget)
							{
								TwCommand	kComm(TwCommandType::BCT_ATTACK, pkTarget->GetID());
								m_kCommands.push_back(kComm);
							}
						}
						else if (pkSkillRes->uiAfterActionType == SKILL_AFTERACTIONTYPE_DOLAST)
						{
							if (m_kCurCmd.eCmdType == TwCommandType::BCT_MOVE)
							{
								m_kCommands.push_back(m_kCurCmd);
							}
							else if (m_kCurCmd.eCmdType == TwCommandType::BCT_ATTACK) 
                            {
								std::shared_ptr<BeUnit> pkTarget = gUnitMgr->GetUnitByID(m_kCurCmd.iUnitID);

								if (pkTarget && !pkTarget->IsDead()) {
									m_kCommands.push_back(m_kCurCmd);
								}
							}
						}
					}
				}
			}
			if (kCmd.eCmdType == TwCommandType::BCT_USE_ITEM)
			{
				//do
				//{
				//	auto pkItem = gUnit->GetItemByID(kCmd.iData);
				//	if (!pkItem)
				//	{
				//		break;
				//	}
				//	int iSkillTypeID = gUnit->GetItemSkillTypeID(kCmd.iData);
				//	auto pkSkill = pkItem->GetSkillByTypeID(iSkillTypeID);
				//	if (!pkSkill)
				//	{
				//		break;
				//	}
				//	const	SkillTable* pkSkillRes = pkSkill->GetResPtr();
				//	if (pkSkillRes)
				//	{
				//		if (pkSkillRes->uiAfterActionType == SKILL_AFTERACTIONTYPE_ATTACKTARGET)
				//		{
				//			std::shared_ptr<BeUnit> pkTarget = gUnitMgr->GetUnitByID(kCmd.iUnitID);
				//			if (pkTarget)
				//			{
				//				BeCommand	kComm(BeCommandType::BCT_ATTACK, pkTarget->GetID());
				//				m_kCommands.push_back(kComm);
				//			}
				//		}
				//		else if (pkSkillRes->uiAfterActionType == SKILL_AFTERACTIONTYPE_DOLAST)
				//		{
				//			if (m_kCurCmd.eCmdType == BeCommandType::BCT_MOVE || m_kCurCmd.eCmdType == BeCommandType::BCT_ATTACK)
				//			{
				//				m_kCommands.push_back(m_kCurCmd);
				//			}
				//		}
				//	}
				//} while (0);
			}
			break;
		}
	}
	case TwGiveCmdType::BCT_PLAYER_INSERT:
	{
		if (!m_pkCurCmd)
		{
			m_kCommands.insert(m_kCommands.begin(), kCmd);
			m_bForceNexeCmd = true;
			break;
		}
		else if (m_pkCurCmd->CanHungUp(reType, bNeedHangCurrent))
		{
			m_kCurCmd.eGiveType = TwGiveCmdType::BCT_PLAYER_SHIFT;
			std::list<TwCommand>::iterator it = m_kCommands.begin();
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
				std::list<TwCommand>::iterator it = m_kCommands.begin();
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
	case TwGiveCmdType::BCT_PLAYER_HUNG:
	{
		std::list<TwCommand>::iterator it = m_kCommands.begin();
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
	case TwGiveCmdType::BCT_DEATH:
	{
		m_kCommands.clear();
		m_kCommands.insert(m_kCommands.begin(), kCmd);
		m_bForceNexeCmd = true;
		break;
	}
	case TwGiveCmdType::BCT_DIZZY_INTERRUPT:
	{
		m_bForceNexeCmd = true;
	}
	case TwGiveCmdType::BCT_SKILL_ADDON:
	{
		if (m_pkCurCmd && m_pkCurCmd->CanHungUp(reType, bNeedHangCurrent))
		{
			m_kCurCmd.eGiveType = TwGiveCmdType::BCT_PLAYER_SHIFT;
			m_kCommands.insert(m_kCommands.begin(), m_kCurCmd);
			m_kCommands.insert(m_kCommands.begin(), kCmd);

			if (gUnit->HasProperty(1 << 15))
			{
				for (std::list<TwCommand>::iterator it = m_kCommands.begin(); it != m_kCommands.end();)
				{
					std::list<TwCommand>::iterator itDel = it;
					++it;
					if (itDel->eCmdType == TwCommandType::BCT_SPELL)
					{
						m_kCommands.erase(itDel);
					}
				}
			}
		}
		else
		{
			std::list<TwCommand>::iterator it = m_kCommands.begin();
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

void TwCommander::ExecuteCmd(int iDeltaTime)
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
		BeExeResult eResult = BeExeResult::BER_EXE_END;
		if (m_pkCurCmd)
		{
			eResult = m_pkCurCmd->Execute(iDeltaTime);
		}
		bool bSkip = (m_pkCurCmd ? m_pkCurCmd->CanSkip() : true) && m_kCommands.size();

		if (gUnit->HasUnitCarryFlag(BUCF_DIZZY) && m_pkCurCmd->GetType() != TwCommandType::BCT_STOP)
		{
			eResult = BeExeResult::BER_EXE_END;
		}
		if (eResult == BeExeResult::BER_EXE_END)
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
		else if (eResult == BeExeResult::BER_ALL_OVER)
		{
			gUnit->SetFlag(BUF_REMOVE);
			m_bForceNexeCmd = true;
			SafeDeleteCommand(m_pkCurCmd);
			m_kCommands.clear();
			break;
		}
	}
}

bool TwCommander::SwitchCmd(const TwCommand& kCmd, bool bConnect)
{
	switch (kCmd.eCmdType)
	{
	case TwCommandType::BCT_STOP:
	{
		gUnit->SetAttackingUnitID(0);
		BeStopCommand* pkCmd = NULL;
		if (m_pkCurCmd && m_pkCurCmd->GetType() == TwCommandType::BCT_STOP)
		{
			BeStopCommand* pkStop = (BeStopCommand*)m_pkCurCmd;
			if (!pkStop->IsDead() || gUnit->IsDead())
			{
				pkCmd = (BeStopCommand*)m_pkCurCmd;
			}
		}

		if (!pkCmd)
		{
			SafeDeleteCommand(m_pkCurCmd);
			pkCmd = mpStopCommand.alloc();
			pkCmd->AttachMain(pkAttachMain);
			pkCmd->AttachUnit(pkAttachUnit);
		}

		m_pkCurCmd = pkCmd;

		if (kCmd.iUnitID == -1)
		{
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
	case TwCommandType::BCT_MOVE:
	{
		gUnit->SetAttackingUnitID(0);
		BeMoveCommand* pkCmd = NULL;
		if (m_pkCurCmd && m_pkCurCmd->GetType() == TwCommandType::BCT_MOVE)
		{
			pkCmd = (BeMoveCommand*)m_pkCurCmd;
		}

		if (!pkCmd)
		{
			SafeDeleteCommand(m_pkCurCmd);
			pkCmd = mpMoveCommand.alloc();
			pkCmd->AttachMain(pkAttachMain);
			pkCmd->AttachUnit(pkAttachUnit);
		}
		{
			pkCmd->SetTargetPos(kCmd.kPos, (float)kCmd.iData, kCmd.iData2 != 0);
		}

		m_pkCurCmd = pkCmd;
		break;
	}
	case TwCommandType::BCT_ATTACK:
	{
		BeAttackCommand* pkCmd = nullptr;
		if (m_pkCurCmd && (m_pkCurCmd->GetType() == TwCommandType::BCT_ATTACK && kCmd.bForceAttackOnce == false))
		{
			pkCmd = (BeAttackCommand*)m_pkCurCmd;
		}

		if (!pkCmd)
		{
			SafeDeleteCommand(m_pkCurCmd);
			pkCmd = mpAttackCommand.alloc();
			pkCmd->AttachMain(pkAttachMain);
			pkCmd->AttachUnit(pkAttachUnit);
		}

		if (kCmd.iUnitID)
		{
            pkCmd->SetTargetID(kCmd.iUnitID, 0.0f, false, 0, 0, kCmd.iData2);
		}
		else
		{
			pkCmd->SetTargetPos(kCmd.kPos, (float)kCmd.iData);
		}

		m_pkCurCmd = pkCmd;
		break;
	}
	case TwCommandType::BCT_SPELL:
	{
		SeCalSkillLvlData kData;
		BeSkill* pkSkill = nullptr;
		pkSkill = gUnit->GetSkill(kCmd.iData);
		if (!pkSkill)
		{
			return false;
		}

		if (!gUnit->GetSkillLvlData(kData, kCmd.iData))
		{
			return false;
		}

		if (pkSkill->IsSwitch() && pkSkill->IsActive())
		{
		}
		else if (!pkSkill->CDComplete(gUnit))
		{
			return false;
		}

		if (pkSkill->IsSwitch())
		{
			auto& pkSkillRes = pkSkill->GetSkillRes();
			if (pkSkillRes)
			{
				SeCalSkillLvlData& rkLvlData = kData;
				pkSkill->SetActive(!pkSkill->IsActive());
				if (pkSkill->IsActive())
				{
					gUnit->SetMP(gUnit->GetMP() - rkLvlData.GetSkillManaSpend());
					pkSkill->SetLastUseTime(gUnit, gTime);
					pkSkill->SetUICounter(1);

					gUnit->TrgOnSpell(pkSkill->GetTypeID(), pkSkill->GetLevel(), 0, 0, gUnit->GetPosX(), gUnit->GetPosY());

					TwPtParam kParam;
					kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, &gUnit);
					kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, pkSkill->GetTypeID());
					kParam.SetParam(TwTrgParamID::BTP_iSkillLevel, pkSkill->GetLevel());
					gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SPELL_EFFECT, kParam);
				}
				else
				{
					pkSkill->SetUICounter(0);

					TwPtParam kParam;
					kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, &gUnit);
					kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, pkSkill->GetTypeID());
					kParam.SetParam(TwTrgParamID::BTP_iSkillLevel, pkSkill->GetLevel());
					gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SPELL_CLOSE, kParam);
				}
			}
			break;
		}

		gUnit->SetAttackingUnitID(0);
		SafeDeleteCommand(m_pkCurCmd);
		BeSpellCommand* pkCmd = mpSpellCommand.alloc();
		pkCmd->AttachMain(pkAttachMain);
		pkCmd->AttachUnit(pkAttachUnit);
		{
			if (kCmd.iUnitID)
			{
				pkCmd->SpellTargetID(kCmd.iData, kCmd.iUnitID, kCmd.kPos, pkSkill->GetLevel(), 0, 1, kCmd.iData2, (int)kCmd.eTargetType);
			}
			else
			{
				pkCmd->SpellTargetPos(kCmd.iData, kCmd.kPos, kCmd.kDirPos, pkSkill->GetLevel(), 0, 1, kCmd.iData2);
			}
		}
		m_pkCurCmd = pkCmd;

		break;

	}
	case TwCommandType::BCT_USE_ITEM:
	{
		gUnit->SetAttackingUnitID(0);
		SafeDeleteCommand(m_pkCurCmd);

		BeItem* pkItem = gUnit->GetItemByID(kCmd.iData);
		if (!pkItem)
		{
			return false;
		}
		auto& pkItemRes = ItemTableMgr::Get()->GetItemTable(pkItem->GetTypeID());
		if (!pkItemRes)
		{
			return false;
		}

		int iSkillTypeID = gUnit->GetItemSkillTypeID(kCmd.iData);

		auto& pkRes = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID);
		if (!pkRes)
		{
			return false;
		}
		SeCalSkillLvlData kData;
		if (!gUnit->GetSkillLvlData(kData, iSkillTypeID))
		{
			return false;
		}

		BeSpellCommand* pkCmd = mpSpellCommand.alloc();
		pkCmd->AttachMain(pkAttachMain);
		pkCmd->AttachUnit(pkAttachUnit);

		if (pkRes->uiOperateType == SKILL_OPERATETYPE_IMMEDIATELY)
		{
			pkCmd->SpellTargetID(iSkillTypeID, kCmd.iUnitID, kCmd.kPos, 1, kCmd.iData);
		}
		else
		{
			std::shared_ptr<BeUnit> pkCmdUnit = gUnitMgr->GetUnitByID(kCmd.iUnitID);
			if (pkCmdUnit)
			{
				pkCmd->SpellTargetID(iSkillTypeID, kCmd.iUnitID, kCmd.kPos, 1, kCmd.iData);
			}
			else
			{
				auto pkMapItem = gMapItemMgr.GetMapItemByID(kCmd.iUnitID);
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
	case TwCommandType::BCT_DROP_ITEM:
	{
		gUnit->SetAttackingUnitID(0);
		SafeDeleteCommand(m_pkCurCmd);
		BeDropItemCommand* pkCmd = mpDropItemCommand.alloc();
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
	case TwCommandType::BCT_PICK_ITEM:
	{
		gUnit->SetAttackingUnitID(0);
		BePickItemCommand* pkCmd = mpPickItemCommand.alloc();
		pkCmd->AttachMain(pkAttachMain);
		pkCmd->AttachUnit(pkAttachUnit);

		if (pkCmd->SetItemID(kCmd.iData))
		{
			SafeDeleteCommand(m_pkCurCmd);
			m_pkCurCmd = pkCmd;
		}
		else
		{
			mpPickItemCommand.free(pkCmd);
		}
		break;
	}
	default:
	{
		return false;
	}
	}
	///	gUnit->SetHighChangeFlag(SHFCF_CMD);
	m_kCurCmd = kCmd;
	m_kCurCmd.bForceExecute = false;

	return true;
}

bool TwCommander::GoNextCmd(bool bSkipCurrent)
{
	if (!gUnit->IsDead() && (gUnit->HasUnitCarryFlag(BUCF_DIZZY)))
	{
		bool bSkip = false;

		if (!bSkip)
		{
			if (m_pkCurCmd)
			{
				if (m_pkCurCmd->GetType() != TwCommandType::BCT_STOP)
				{
					if (!m_kCommands.empty())
					{
						TwCommand& kCmd = m_kCommands.front();
						if (kCmd.eCmdType == TwCommandType::BCT_STOP)
						{
							m_kCommands.pop_front();
						}
					}
					TwCommand kStop(TwCommandType::BCT_STOP);
					SwitchCmd(kStop, false);
				}
			}
			else
			{
				TwCommand kStop(TwCommandType::BCT_STOP);
				SwitchCmd(kStop, false);
			}

			return false;
		}
	}

	while (!m_kCommands.empty())
	{
		TwCommand kCmd = m_kCommands.front();
		m_kCommands.pop_front();

		bool bDelay = false;
		if (kCmd.eGiveType == TwGiveCmdType::BCT_PLAYER_SHIFT)
		{
			if ((kCmd.eCmdType == TwCommandType::BCT_SPELL && gUnit->HasUnitCarryFlag(BUCF_ISFORBIDSKILL))
				|| (kCmd.eCmdType == TwCommandType::BCT_USE_ITEM && gUnit->HasUnitCarryFlag(BUCF_ISFORBIDITEM))
				|| gUnit->HasUnitCarryFlag(BUCF_CANNOTCONTROL)
				|| gUnit->HasUnitCarryFlag(BUCF_DIZZY)
				|| gUnit->HasUnitCarryFlag(BUF_SPELL_SHARK)
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
				kCmd = TwCommand(TwCommandType::BCT_STOP, 0, 0.0f, 0.0f, 100);
			}
		}

		if (SwitchCmd(kCmd))
		{
			return true;
		}
	}

	if (m_pkCurCmd && (m_pkCurCmd->GetType() == TwCommandType::BCT_MOVE || m_pkCurCmd->GetType() == TwCommandType::BCT_MOVE_DIRECT))
	{
		TwCommand kStop(TwCommandType::BCT_STOP, 0, 0.0f, 0.0f, 250);
		SwitchCmd(kStop, false);
	}
	else
	{
		TwCommand kStop(TwCommandType::BCT_STOP);
		SwitchCmd(kStop, false);
	}
	return true;
}

void TwCommander::SafeDeleteCommand(BeExeCommand*& pkCommand)
{
	if (pkCommand)
	{
		switch (pkCommand->GetType())
		{
		    case TwCommandType::BCT_STOP:           mpStopCommand.free((BeStopCommand*)pkCommand);				break;
		    case TwCommandType::BCT_MOVE:           mpMoveCommand.free((BeMoveCommand*)pkCommand);				break;
		    case TwCommandType::BCT_ATTACK:         mpAttackCommand.free((BeAttackCommand*)pkCommand);			break;
		    case TwCommandType::BCT_SPELL:          mpSpellCommand.free((BeSpellCommand*)pkCommand);			break;
		    case TwCommandType::BCT_DROP_ITEM:      mpDropItemCommand.free((BeDropItemCommand*)pkCommand);		break;
		    case TwCommandType::BCT_PICK_ITEM:      mpPickItemCommand.free((BePickItemCommand*)pkCommand);		break;
		    default:
		    break;
		}
		pkCommand = nullptr;
	}
}

void TwCommander::ClearAllCommands()
{
	m_kCommands.clear();
}
