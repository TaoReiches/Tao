/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TaskActionSpell.h"
#include "TW_Main.h"
#include <TW_TriggerMgr.h>
#include "TW_TriggerEvent.h"
#include <Skill_table.hpp>
#include "TW_ShareUnitData.h"
#include "TW_Unit.h"
#include "TW_UnitMgr.h"
#include "TW_Item.h"
#include "TW_MapItem.h"
#include "TW_MapItemMgr.h"
#include "TW_Skill.h"

BeTaskActionSpell::BeTaskActionSpell()
{
	m_eType = BeTaskType::STT_ACTION_SPELL;

	m_iSkillTypeID = 0;
	m_iSkillLevel = 1;
	m_bExpendMP = true;
	m_iTargetID = 0;
	m_iTargetType = 0;
	m_ePhase = BeSpellPhase::BSP_INIT;
	m_iActionTime = 0;
	m_iPrepareTime = 0;
	m_iCastTime = 0;
	m_iEffectTime = 0;
	m_iShakesTime = 0;
	m_iItemID = 0;
	m_iItemOwnerPlayer = -1;
	m_iManaSpend = 0;
	m_iOtherID = 0;
	m_iItemTypeID = 0;
	m_pkSkillRes = NULL;
	m_iPersitTime = 0;
	m_iPersitDeltaTime = 100;
	m_iUsePlayer = -1;
	m_bSkipEffectTime = false;
}

BeTaskActionSpell::~BeTaskActionSpell()
{
	if (((m_ePhase == BeSpellPhase::BSP_EFFECT) || (m_ePhase == BeSpellPhase::BSP_FINISH) || (m_ePhase == BeSpellPhase::BSP_END)))
	{
		TwPtParam kParam;
		kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, &gUnit);
		kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, m_iSkillTypeID);
		kParam.SetParam(TwTrgParamID::BTP_iSkillLevel, m_iSkillLevel);
		kParam.SetParam(TwTrgParamID::BTP_iSpellTargetID, m_iTargetID);
		kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosX, m_kTargetPos.fX);
		kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosY, m_kTargetPos.fY);
		kParam.SetParam(TwTrgParamID::BTP_iTargetType, m_iTargetType);

		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SPELL_CMDEND, kParam);

		if (m_ePhase == BeSpellPhase::BSP_EFFECT)
		{
			gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SPELL_EFFECTEND, kParam);
		}

		//if ((m_pkSkillRes->uiSkillProperty & SKILL_SKILLPROPERTY_SENDBAR) != 0)
		//{
		//	BeShareUIWindowData	kWindowData;
		//	kWindowData.eWindowType = BWT_SPELL_EFFECT;
		//	kWindowData.iSkillID = m_iSkillTypeID;
		//	kWindowData.iRemoveTime = m_iEffectTime;
		//	kWindowData.iUnitID = gUnit->GetID();
		//	kWindowData.bRemove = true;
		//	//gMain->AddWindowData(kWindowData);
		//}
	}
	else if (m_ePhase == BeSpellPhase::BSP_CAST)
	{
		TwPtParam kParam;
		kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, &gUnit);
		kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, m_iSkillTypeID);
		kParam.SetParam(TwTrgParamID::BTP_iSkillLevel, m_iSkillLevel);
		kParam.SetParam(TwTrgParamID::BTP_iSpellTargetID, m_iTargetID);
		kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosX, m_kTargetPos.fX);
		kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosY, m_kTargetPos.fY);
		kParam.SetParam(TwTrgParamID::BTP_iTargetType, m_iTargetType);

		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SPELL_CASTEND, kParam);

		BeShareUIWindowData	kWindowData;
		kWindowData.eWindowType = BWT_SPELL_CAST;
		kWindowData.iSkillID = m_iSkillTypeID;
		kWindowData.iRemoveTime = 0;
		kWindowData.iUnitID = gUnit->GetID();
		kWindowData.bRemove = true;
		//gMain->AddWindowData(kWindowData);
	}

	gUnit->ClrFlag(TwUnitFlag::BUF_ISPERSISTSKILL);
}

bool BeTaskActionSpell::SpellTargetID(int iSkillTypeID, int iSkillLevel, bool bExpendMP, int iTargetID, const TwPos2& kPos, int iItemID, int iUsePlayer, int iTargetType)
{
	m_iSkillTypeID = iSkillTypeID;
	m_iSkillLevel = iSkillLevel;
	m_bExpendMP = bExpendMP;
	m_iTargetID = iTargetID;
	m_iTargetType = iTargetType;
	m_iUsePlayer = iUsePlayer;
	m_kTargetPos = kPos;

	if (iItemID != 0)
	{
		m_iItemID = iItemID;
		BeItem* pkItem = gUnit->GetItemByID(iItemID);
		if (pkItem)
		{
			if (!pkItem->GetValid())
			{
				return false;
			}
			m_iItemOwnerPlayer = pkItem->GetOwnerPlay();
			m_iItemTypeID = pkItem->GetTypeID();
		}
	}

	m_pkSkillRes = SkillTableMgr::Get()->GetSkillTable(m_iSkillTypeID);

	gUnit->SetTarFace(gUnit->GetFace());
	if (iTargetID && iTargetID != gUnit->GetID())
	{
		if (!m_pkSkillRes)
		{
			return false;
		}
		if (m_iTargetType == (int)BeCommandTargetType::BCTT_UNIT)
		{
			std::shared_ptr<TwUnit> pkTarget = gUnitMgr->GetUnitByID(m_iTargetID);
			if (pkTarget)
			{
				if (m_iTargetID != gUnit->GetID() && gUnit->GetClass() != UNIT_CLASSTYPE_BUILDING)
				{
					float fTarFace = atan2f(pkTarget->GetPosY() - gUnit->GetPosY(), pkTarget->GetPosX() - gUnit->GetPosX());
					gUnit->SetTarFace(fTarFace);
				}
			}
		}
	}

	return true;
}

bool BeTaskActionSpell::SpellTargetItem(int iSkillTypeID, int iSkillLevel, bool bExpendMP, const TwPos2& kPos, int iItemID, int iOtherID, int iUsePlayer)
{
	gUnit->SetAttackingUnitID(0);
	m_iSkillTypeID = iSkillTypeID;
	m_iSkillLevel = iSkillLevel;
	m_bExpendMP = bExpendMP;
	m_kTargetPos = kPos;
	m_iTargetID = 0;
	m_iUsePlayer = iUsePlayer;
	m_iOtherID = iOtherID;

	if (iItemID != 0)
	{
		m_iItemID = iItemID;

		BeItem* pkItem = gUnit->GetItemByID(iItemID);
		if (pkItem)
		{
			if (!pkItem->GetValid())
			{
				return false;
			}
			m_iItemOwnerPlayer = pkItem->GetOwnerPlay();
			m_iItemTypeID = pkItem->GetTypeID();
		}
	}

	m_pkSkillRes = SkillTableMgr::Get()->GetSkillTable(m_iSkillTypeID);

	gUnit->SetTarFace(gUnit->GetFace());

	if (iItemID)
	{
		auto pkMapItem = gMapItemMgr.GetMapItemByID(iOtherID);
		if (pkMapItem && gUnit->GetClass() != UNIT_CLASSTYPE_BUILDING)
		{
			float fTarFace = atan2f(pkMapItem->GetPosY() - gUnit->GetPosY(), pkMapItem->GetPosX() - gUnit->GetPosX());
			gUnit->SetTarFace(fTarFace);
		}
	}

	return true;
}

bool BeTaskActionSpell::SpellTargetPos(int iSkillTypeID, int iSkillLevel, bool bExpendMP, const TwPos2& kPos, const TwPos2& kDirPos, int iItemID, int iUsePlayer)
{
	gUnit->SetAttackingUnitID(0);
	m_iSkillTypeID = iSkillTypeID;
	m_iSkillLevel = iSkillLevel;
	m_bExpendMP = bExpendMP;
	m_kTargetPos = kPos;
	m_iUsePlayer = iUsePlayer;
	m_iTargetID = 0;
	m_kDirectPos = kDirPos;
	m_iItemID = iItemID;

	m_pkSkillRes = SkillTableMgr::Get()->GetSkillTable(m_iSkillTypeID);

	if ((m_pkSkillRes->uiOperateType == SKILL_OPERATETYPE_IMMEDIATELY))
	{
		m_kTargetPos.fX = 0.0f;
		m_kTargetPos.fY = 0.0f;
	}

	if (!m_pkSkillRes)
	{
		gUnit->SetTarFace(gUnit->GetFace());
	}
	else if (gUnit->GetClass() != UNIT_CLASSTYPE_BUILDING && m_kTargetPos.fX != 0.0f && m_kTargetPos.fY != 0.0f)
	{
		float fTarFace = atan2f(kPos.fY - gUnit->GetPosY(), kPos.fX - gUnit->GetPosX());
		gUnit->SetTarFace(fTarFace);
	}

	return true;
}
bool BeTaskActionSpell::CanSkip(void) const
{
	return false;
}
BeSpellPhase BeTaskActionSpell::GetPhase(void)
{
	return m_ePhase;
}

void BeTaskActionSpell::OnSpellPrepare(void)
{
	TwPtParam kParam;
	kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, &gUnit);
	kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, m_iSkillTypeID);
	kParam.SetParam(TwTrgParamID::BTP_iSkillLevel, m_iSkillLevel);
	kParam.SetParam(TwTrgParamID::BTP_iSpellTargetID, m_iTargetID);
	kParam.SetParam(TwTrgParamID::BTP_iSkillUsePlayer, m_iUsePlayer);
	kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosX, m_kTargetPos.fX);
	kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosY, m_kTargetPos.fY);
	kParam.SetParam(TwTrgParamID::BTP_fSpellDirectPosX, m_kDirectPos.fX);
	kParam.SetParam(TwTrgParamID::BTP_fSpellDirectPosY, m_kDirectPos.fY);
	kParam.SetParam(TwTrgParamID::BTP_iTargetType, m_iTargetType);

	gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SPELL_PREPARE, kParam);
}

void BeTaskActionSpell::OnSpellCast(void)
{
	TwPtParam kParam;
	kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, &gUnit);
	kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, m_iSkillTypeID);
	kParam.SetParam(TwTrgParamID::BTP_iSkillLevel, m_iSkillLevel);
	kParam.SetParam(TwTrgParamID::BTP_iSpellTargetID, m_iTargetID);
	kParam.SetParam(TwTrgParamID::BTP_iSkillUsePlayer, m_iUsePlayer);
	kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosX, m_kTargetPos.fX);
	kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosY, m_kTargetPos.fY);
	kParam.SetParam(TwTrgParamID::BTP_iTargetType, m_iTargetType);

	if (m_iItemID == 0)
	{
		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SPELL_CAST, kParam);
	}
	else
	{
		kParam.SetParam(TwTrgParamID::BTP_iSkillLevel, 1);
		kParam.SetParam(TwTrgParamID::BTP_iItemID, m_iItemID);
		kParam.SetParam(TwTrgParamID::BTP_iOtherID, m_iOtherID);

		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_USE_ITEMCAST, kParam);
	}
}

void BeTaskActionSpell::OnSpellEffect(void)
{
	if (m_iSkillTypeID != 'SA02' && m_iSkillTypeID != 'SA06')
	{
		gUnit->TrgOnSpell(m_iSkillTypeID, m_iSkillLevel, m_iItemID, m_iTargetID, m_kTargetPos.fX, m_kTargetPos.fY);
	}

	m_iPersitTime = 100;
	m_iPersitDeltaTime = 100;

	if (m_iTargetType == (int)BeCommandTargetType::BCTT_UNIT && m_iTargetID)
	{
		std::shared_ptr<TwUnit> pkTarget = gUnitMgr->GetUnitByID(m_iTargetID);
		if (pkTarget)
		{
			m_kTargetPos.fX = pkTarget->GetPosX();
			m_kTargetPos.fY = pkTarget->GetPosY();
		}
	}

	TwPtParam kParam;
	kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, &gUnit);
	kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, m_iSkillTypeID);
	kParam.SetParam(TwTrgParamID::BTP_iSpellTargetID, m_iTargetID);
	kParam.SetParam(TwTrgParamID::BTP_piSpellTargetID, &m_iTargetID);
	kParam.SetParam(TwTrgParamID::BTP_iSkillUsePlayer, m_iUsePlayer);
	kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosX, m_kTargetPos.fX);
	kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosY, m_kTargetPos.fY);
	kParam.SetParam(TwTrgParamID::BTP_fSpellDirectPosX, m_kDirectPos.fX);
	kParam.SetParam(TwTrgParamID::BTP_fSpellDirectPosY, m_kDirectPos.fY);
	kParam.SetParam(TwTrgParamID::BTP_piPersistTime, &m_iPersitTime);
	kParam.SetParam(TwTrgParamID::BTP_piPersistDeltaTime, &m_iPersitDeltaTime);
	kParam.SetParam(TwTrgParamID::BTP_iTargetType, m_iTargetType);

	if (m_iItemID == 0)
	{
		kParam.SetParam(TwTrgParamID::BTP_iSkillLevel, m_iSkillLevel);

		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SPELL_EFFECT, kParam);
	}
	else
	{
		int iItemTypeID = m_iItemTypeID;
		BeItem* pkItem = gUnit->GetItemByID(m_iItemID);
		if (pkItem)
		{
			iItemTypeID = pkItem->GetTypeID();
			pkItem->SetBuyTime(0);
			kParam.SetParam(TwTrgParamID::BTP_iItemTypeID, iItemTypeID);
		}
		else
		{
			kParam.SetParam(TwTrgParamID::BTP_iItemTypeID, iItemTypeID);
		}
		kParam.SetParam(TwTrgParamID::BTP_iSkillLevel, 1);
		kParam.SetParam(TwTrgParamID::BTP_iItemID, m_iItemID);
		kParam.SetParam(TwTrgParamID::BTP_iItemOwnerPlayer, m_iItemOwnerPlayer);
		kParam.SetParam(TwTrgParamID::BTP_iOtherID, m_iOtherID);

		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_USE_ITEM, kParam);
	}
}

void BeTaskActionSpell::OnSpellFinish(void)
{
	TwPtParam kParam;
	kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, &gUnit);
	kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, m_iSkillTypeID);
	kParam.SetParam(TwTrgParamID::BTP_iSpellTargetID, m_iTargetID);
	kParam.SetParam(TwTrgParamID::BTP_iSkillUsePlayer, m_iUsePlayer);
	kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosX, m_kTargetPos.fX);
	kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosY, m_kTargetPos.fY);
	kParam.SetParam(TwTrgParamID::BTP_iTargetType, m_iTargetType);

	if (m_iItemID == 0)
	{
		kParam.SetParam(TwTrgParamID::BTP_iSkillLevel, m_iSkillLevel);

		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SPELL_FINISH, kParam);
	}
	else
	{
		BeItem* pkItem = gUnit->GetItemByID(m_iItemID);
		if (pkItem)
		{
			if (pkItem->IsCanSplice())
			{
				if (pkItem->GetPileCount() > 1)
				{
					pkItem->SetPileCount(pkItem->GetPileCount() - 1);
					pkItem->SetUseCount(pkItem->GetOrgUseCount());
				}
				else
				{
					gUnit->DelItemByPos(pkItem->GetPackagePos());
				}
			}
		}

		if (pkItem)
		{
			if (pkItem->IsCanSplice())
			{
				if (pkItem->GetPileCount() > 1)
				{
					pkItem->SetPileCount(pkItem->GetPileCount() - 1);
					pkItem->SetUseCount(pkItem->GetOrgUseCount());
				}
				else
				{
					gUnit->DelItemByPos(pkItem->GetPackagePos());
				}
			}
		}

		kParam.SetParam(TwTrgParamID::BTP_iSkillLevel, 1);
		kParam.SetParam(TwTrgParamID::BTP_iItemID, m_iItemID);
		kParam.SetParam(TwTrgParamID::BTP_iOtherID, m_iOtherID);

		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_USEITEM_FINISH, kParam);
	}
}

bool BeTaskActionSpell::CanUseSpell(void)
{
	SeCalSkillLvlData	rkData;
	if (!gUnit->GetSkillLvlData(rkData, m_iSkillTypeID, m_iSkillLevel))
	{
		return false;
	}
	int iManaSpend = rkData.GetSkillManaSpend();
	if (m_bExpendMP && gUnit->GetMP() < iManaSpend)
	{
		return false;
	}

	if (m_iItemID)
	{
		if (gUnit->HasUnitCarryFlag(BUCF_ISFORBIDITEM))
		{
			return false;
		}

		auto pkItem = gUnit->GetItemByID(m_iItemID);
		if (pkItem)
		{
			//auto pkSkill = pkItem->GetSkillByTypeID(m_iSkillTypeID);
			//if (pkSkill)
			//{
			//	int iSkillCD = rkData.GetSkillCoolDown();

			//	//if (!gUnit->CommonCDComplete(pkItem->GetCDSkillID(), iSkillCD))
			//	//{
			//	//	return false;
			//	//}
			//}
			//else
			//{
			//	return false;
			//}
		}
		else
		{
			return false;
		}

		if (m_iTargetID)
		{
			std::shared_ptr<TwUnit> pkTarget = gUnitMgr->GetUnitByID(m_iTargetID);
			if (!pkTarget)
			{
				return false;
			}
			if (!gUnit->IsSkillTargetType(m_pkSkillRes, pkTarget))
			{
				return false;
			}
		}
	}
	else
	{
		BeSkill* pkSkill = gUnit->GetSkill(m_iSkillTypeID);
		if (!pkSkill)
		{
			return false;
		}
		if (pkSkill && pkSkill->IsSwitch() && pkSkill->IsActive())
		{
			return true;
		}

		int iSkillCD = rkData.GetSkillCoolDown();
		int iSkillCount = pkSkill->GetUICounter();

		if (pkSkill->GetCurPileNums() > 0 || (iSkillCount > 0 && iSkillCount != 234))
		{
		}
		//else if (!gUnit->CommonCDComplete(m_iSkillTypeID, iSkillCD))
		//{
		//	return false;
		//}


		UnitUseSkillResultType eResult = gUnit->UnitCanUseSkill(m_iSkillTypeID, NULL, true, true, m_kTargetPos.fX, m_kTargetPos.fY);
		if (eResult != UUSRT_OK)
		{
			return false;
		}

		if (gUnit->IsForbidSkill(pkSkill))
		{
			if (m_ePhase < BeSpellPhase::BSP_CAST)
			{
				return false;
			}
		}
	}

	return true;
}

bool BeTaskActionSpell::WillEffectBreak(void)
{
	if (gUnit->HasUnitCarryFlag(BUCF_DIZZY))
	{
		return true;
	}

	BeSkill* pkSkill = gUnit->GetSkill(m_iSkillTypeID);
	if (pkSkill)
	{
		if (gUnit->IsForbidSkill(pkSkill))
		{
			return true;
		}
	}

	return false;
}

bool BeTaskActionSpell::UseUpItem()
{
	if (m_bExpendMP)
	{
		m_iManaSpend = m_pkSkillRes->iManaSpend[m_iSkillLevel];
		gUnit->SetMP(gUnit->GetMP() - m_iManaSpend);
	}

	BeItem* pkItem = gUnit->GetItemByID(m_iItemID);
	if (pkItem)
	{
		OperateItem(pkItem);
		return true;
	}
	return false;
}

void BeTaskActionSpell::OperateItem(BeItem* pkItem)
{
	if (!pkItem)
	{
		return;
	}

	if (pkItem->GetOrgUseCount() != -1)
	{
		pkItem->SetUseCount(pkItem->GetUseCount() - 1);
	}

	//gUnit->SetCommonCDLastUseTime(pkItem->GetCDSkillID(), (int)gTime);
	for (int i = 0; i < 6; i++)
	{
		BeItem* pkOldItem = gUnit->GetItemByPos(i);
		if (pkOldItem && pkOldItem->GetCDSkillID() == pkItem->GetCDSkillID())
		{
			{
				pkOldItem->SetLastUseTime((int)gTime);
			}
		}
	}

	if (pkItem->GetUseCount() == 0 && pkItem->IsUsable())
	{
		if (pkItem->IsCanSplice() && pkItem->GetPileCount() > 1)
		{
			pkItem->SetPileCount(pkItem->GetPileCount() - 1);

			pkItem->SetUseCount(pkItem->GetOrgUseCount());
		}
		else
		{
			int iItemID = pkItem->GetID();
			gUnit->DelItemByPos(pkItem->GetPackagePos());
			pkItem = gUnit->GetItemByID(iItemID);
		}
	}

	if (pkItem && pkItem->GetUseCount() == 0 && pkItem->IsUsable())
	{
		pkItem->SetUseCount(pkItem->GetOrgUseCount());
	}
}

bool BeTaskActionSpell::UseUpSkill()
{
	BeSkill* pkSkill = gUnit->GetSkill(m_iSkillTypeID);
	if (!pkSkill)
	{
		return false;
	}
	SeCalSkillLvlData	rkData;
	if (!gUnit->GetSkillLvlData(rkData, m_iSkillTypeID, m_iSkillLevel))
	{
		return false;
	}
	if (pkSkill->IsSwitch() && pkSkill->IsActive())
	{
		return true;
	}
	if (pkSkill->GetTypeID() != 'S209')
	{
		pkSkill->SetLastUseTime(gUnit, (int)gTime);
	}

	if (m_bExpendMP)
	{
		int iManaSpend = rkData.GetSkillManaSpend();
		m_iManaSpend = iManaSpend;
		gUnit->SetMP(gUnit->GetMP() - iManaSpend);
	}

	return true;
}

TwExeResult BeTaskActionSpell::Execute(int& iDeltaTime)
{
	TwTask::Execute(iDeltaTime);
	{
		gUnit->ClrFlag(TwUnitFlag::BUF_MOVING);
	}
	if (!m_pkSkillRes)
	{
		return TwExeResult::BER_EXE_END;
	}

	int iLoopCount = 10;
	while (iDeltaTime > 0)
	{
		--iLoopCount;
		if (iLoopCount <= 0)
		{
			iDeltaTime = 0;
			break;
		}
		switch (m_ePhase)
		{
		case BeSpellPhase::BSP_INIT:
		{
			if (!CanUseSpell())
			{
				return TwExeResult::BER_EXE_END;
			}

			m_iPrepareTime = 0;

			m_iActionTime = 0;
			m_ePhase = BeSpellPhase::BSP_PREPARE;

			OnSpellPrepare();
			break;
		}
		case BeSpellPhase::BSP_PREPARE:
		{
			if (!CanUseSpell())
			{
				return TwExeResult::BER_EXE_END;
			}

			SeCalSkillLvlData	rkData;
			if (!gUnit->GetSkillLvlData(rkData, m_iSkillTypeID, m_iSkillLevel))
			{
				return TwExeResult::BER_EXE_END;
			}
			m_iCastTime = rkData.GetCastTime();
			m_iEffectTime = rkData.GetEffectTime();
			m_iShakesTime = rkData.GetShakesTime();

			if (m_iCastTime < 0)
			{
				if (m_iItemID)
				{
					m_iCastTime = 0;
				}
				else
				{
					m_iCastTime = 100;
				}
			}

			if (m_iEffectTime < 0)
			{
				if (m_iItemID)
				{
					m_iEffectTime = 0;
					m_bSkipEffectTime = true;

				}
				else
				{
					m_iEffectTime = 100;
					m_bSkipEffectTime = true;
				}
			}

			m_iActionTime = 0;
			m_ePhase = BeSpellPhase::BSP_CAST;

			gUnit->SetUnitAction(BUA_SPELL, m_pkSkillRes->uiSpellAction);

			OnSpellCast();

			if (m_iCastTime > 500)
			{
				BeShareUIWindowData	kWindowData;
				kWindowData.eWindowType = BWT_SPELL_CAST;
				kWindowData.iSkillID = m_iSkillTypeID;
				kWindowData.iRemoveTime = m_iCastTime;
				kWindowData.iUnitID = gUnit->GetID();
				//gMain->AddWindowData(kWindowData);
			}

			break;
		}
		case BeSpellPhase::BSP_CAST:
		{
			if (!CanUseSpell())
			{
				return TwExeResult::BER_EXE_END;
			}

			if ((m_iActionTime + iDeltaTime) >= m_iCastTime)
			{
				int iTime = m_iCastTime - m_iActionTime;
				iDeltaTime -= iTime;

				gUnit->IncActionCurTime(iTime);

				m_iActionTime = 0;
				m_ePhase = BeSpellPhase::BSP_EFFECT;

				bool bEnterCD = false;
				if (m_iItemID)
				{
					bEnterCD = UseUpItem();
				}
				else
				{
					bEnterCD = UseUpSkill();
				}

				if (!bEnterCD)
				{
					return TwExeResult::BER_EXE_END;
				}

				OnSpellEffect();

				//do
				//{
				//	if ((m_pkSkillRes->uiSkillProperty & SKILL_SKILLPROPERTY_SENDBAR) != 0)
				//	{
				//		BeShareUIWindowData	kWindowData;
				//		kWindowData.eWindowType = BWT_SPELL_EFFECT;
				//		kWindowData.iSkillID = m_iSkillTypeID;
				//		kWindowData.iRemoveTime = m_iEffectTime;
				//		kWindowData.iUnitID = gUnit->GetID();
				//		//gMain->AddWindowData(kWindowData);
				//	}
				//} while (0);

				break;
			}

			m_iActionTime += iDeltaTime;
			gUnit->IncActionCurTime(iDeltaTime);
			iDeltaTime = 0;
			break;
		}
		case BeSpellPhase::BSP_EFFECT:
		{
			if (WillEffectBreak() || (gUnit->GetSkill(m_iSkillTypeID) == NULL && m_iItemID == 0))
			{
				m_iActionTime = 0;
				return TwExeResult::BER_EXE_END;
			}

			if ((m_iActionTime + iDeltaTime) >= m_iEffectTime)
			{
				int iTime = m_iEffectTime - m_iActionTime;
				iDeltaTime -= iTime;

				gUnit->IncActionCurTime(iTime);

				m_iActionTime = 0;
				m_ePhase = BeSpellPhase::BSP_SHAKES;
				break;
			}
			else
			{
				gUnit->SetFlag(TwUnitFlag::BUF_ISPERSISTSKILL);
			}

			m_iActionTime += iDeltaTime;
			gUnit->IncActionCurTime(iDeltaTime);
			iDeltaTime = 0;
			break;
		}

		case BeSpellPhase::BSP_SHAKES:
		{
			if (gUnit->GetSkill(m_iSkillTypeID) == NULL && m_iItemID == 0)
			{
				m_iActionTime = 0;
				return TwExeResult::BER_EXE_END;
			}

			if ((m_iActionTime + iDeltaTime) >= m_iShakesTime)
			{
				int iTime = m_iShakesTime - m_iActionTime;
				iDeltaTime -= iTime;

				gUnit->IncActionCurTime(iTime);

				m_iActionTime = 0;
				m_ePhase = BeSpellPhase::BSP_FINISH;
				break;
			}

			gUnit->SetFlag(TwUnitFlag::BUF_SPELL_SHARK);
			m_iActionTime += iDeltaTime;
			gUnit->IncActionCurTime(iDeltaTime);
			iDeltaTime = 0;
			break;
		}
		case BeSpellPhase::BSP_FINISH:
		{
			gUnit->ClrFlag(TwUnitFlag::BUF_ISPERSISTSKILL);
			gUnit->ClrFlag(TwUnitFlag::BUF_SPELL_SHARK);
			OnSpellFinish();
			m_ePhase = BeSpellPhase::BSP_END;
			return TwExeResult::BER_EXE_END;
		}
		case BeSpellPhase::BSP_END:
		{
			return TwExeResult::BER_EXE_END;
		}
		default:break;
		}
	}
	return TwExeResult::BER_TIME_OUT;
}
