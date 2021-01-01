/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TriggerFunc.h"
#include "TW_TriggerEvent.h"
#include "TW_TriggerMgr.h"
#include "TW_Carry.h"
#include "TW_Skill.h"
#include "TW_AttackAttr.h"
#include "TW_Buff.h"
#include "TW_Unit.h"

TwTriggerFunc::TwTriggerFunc()
{

}

TwTriggerFunc::~TwTriggerFunc()
{

}

BeCarry* TwTriggerFunc::GetTrgCarry(void)
{
	return (BeCarry*)(m_pkTriggerMgr->GetParamVoid(TwTrgParamID::BTP_pkCarray));
}

int	TwTriggerFunc::GetTrgSkillTypeID(void)
{
	int iSkillTypeID = m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iSkillTypeID);
	return iSkillTypeID;
}

int TwTriggerFunc::GetTrgSkillOrgTypeID(void)
{
	int iSkillTypeID = m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iSkillOrgTypeID);
	return iSkillTypeID;
}

int TwTriggerFunc::GetTrgSkillLevel(void)
{
	int iSkillLevel = m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iSkillLevel);
	if (!iSkillLevel)
	{
		const BeSkill* pkSkill = TrgGetPtr_Skill();
		if (pkSkill)
		{
			iSkillLevel = pkSkill->GetLevel();
		}
	}
	return iSkillLevel;
}

int	TwTriggerFunc::GetTrgSpellUsePlayer(void)
{
	return m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iSkillUsePlayer);
}

int	TwTriggerFunc::GetTrgSpellTargetID(void)
{
	return m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iSpellTargetID);
}

int* TwTriggerFunc::GetTrgPtrSpellTargetID(void)
{
	return (int*)(m_pkTriggerMgr->GetParamVoid(TwTrgParamID::BTP_piSpellTargetID));
}

float TwTriggerFunc::GetTrgSpellOrgPosX(void)
{
	return m_pkTriggerMgr->GetParamFloat(TwTrgParamID::BTP_fSpellOrgPosX);
}

float TwTriggerFunc::GetTrgSpellOrgPosY(void)
{
	return m_pkTriggerMgr->GetParamFloat(TwTrgParamID::BTP_fSpellOrgPosY);
}

float TwTriggerFunc::GetTrgSpellTargetPosX(void)
{
	return m_pkTriggerMgr->GetParamFloat(TwTrgParamID::BTP_fSpellTargetPosX);
}

float TwTriggerFunc::GetTrgSpellTargetPosY(void)
{
	return m_pkTriggerMgr->GetParamFloat(TwTrgParamID::BTP_fSpellTargetPosY);
}

float TwTriggerFunc::GetTrgSpellDirectPosX(void)
{
	return m_pkTriggerMgr->GetParamFloat(TwTrgParamID::BTP_fSpellDirectPosX);
}

float TwTriggerFunc::GetTrgSpellDirectPosY(void)
{
	return m_pkTriggerMgr->GetParamFloat(TwTrgParamID::BTP_fSpellDirectPosY);
}

std::shared_ptr<TwUnit> TwTriggerFunc::GetTrgUnit(bool bIncSuspend)
{
	return TrgGetPtr_Unit();
}

std::shared_ptr<TwUnit> TwTriggerFunc::GetTrgAttacker(void)
{
	return TrgGetPtr_Attacker();
}

std::shared_ptr<TwUnit> TwTriggerFunc::GetTrgTarget(void)
{
	return TrgGetPtr_Target();
}

float TwTriggerFunc::GetTrgAttackedDamge(void)
{
	return *(TrgGetPtr_Damage());
}

int TwTriggerFunc::GetTrgAttackedType(void)
{
	return m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iAttackType);
}

int TwTriggerFunc::GetTrgAttackedIsFirst(void)
{
	return GetTrgAttackedFlag() & BAF_FIRST;
}

int TwTriggerFunc::GetTrgAttackedFlag(void)
{
	return m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iFlag);
}
int* TwTriggerFunc::GetTrgAttackedFlagPtr(void)
{
	return (int*)m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iFlagPtr);
}
int	TwTriggerFunc::GetTrgTargetID()
{
	return m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iTargetID);
}
int	TwTriggerFunc::GetTrgAttackerPlayer(void)
{
	return m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iAttackerPlayer);
}

int TwTriggerFunc::GetTrgBufferTypeID(void)
{
	return TrgGetPtr_Buffer()->GetTypeID();
}

int TwTriggerFunc::GetTrgBufferLevel(void)
{
	return TrgGetPtr_Buffer()->GetLevel();
}

int TwTriggerFunc::GetTrgBufferInnerID(void)
{
	return TrgGetPtr_Buffer()->GetID();
}

int	TwTriggerFunc::GetTrgIntData(void)
{
	return m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iIntData);
}

float TwTriggerFunc::GetTrgFloatData(void)
{
	return m_pkTriggerMgr->GetParamFloat(TwTrgParamID::BTP_fFloatData);
}

bool TwTriggerFunc::GetTrgIsLearnSkill(void)
{
	return m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iIsLearnSkill) == 1 ? true : false;
}

std::shared_ptr<TwUnit> TwTriggerFunc::TrgGetPtr_Unit(void)
{
	return std::shared_ptr<TwUnit>((TwUnit*)m_pkTriggerMgr->GetParamVoid(TwTrgParamID::BTP_pkTrgUnit));
}

BeMapItem* TwTriggerFunc::TrgGetPtr_MapItem(void)
{
	return (BeMapItem*)(m_pkTriggerMgr->GetParamVoid(TwTrgParamID::BTP_pkMapItem));
}

std::shared_ptr<TwUnit> TwTriggerFunc::TrgGetPtr_Attacker(void)
{
	return std::shared_ptr<TwUnit>((TwUnit*)m_pkTriggerMgr->GetParamVoid(TwTrgParamID::BTP_pkAttacker));
}

std::shared_ptr<TwUnit> TwTriggerFunc::TrgGetPtr_Target(void)
{
	return std::shared_ptr<TwUnit>((TwUnit*)m_pkTriggerMgr->GetParamVoid(TwTrgParamID::BTP_pkTarget));
}

BeSkill* TwTriggerFunc::TrgGetPtr_Skill(void)
{
	int iItemPos = m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iItemPos) - 1;
	if (iItemPos >= 0 && iItemPos < 6)
	{
		BeItem* pkItem = TrgGetPtr_Item();
		if (!pkItem)
		{
			return NULL;
		}
	}
	return (BeSkill*)(m_pkTriggerMgr->GetParamVoid(TwTrgParamID::BTP_pkSkill));
}

int TwTriggerFunc::TrgGetItemPos(void)
{
	return (m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iItemPos) - 1);
}

int	TwTriggerFunc::TrgGetItemTypeID(void)
{
	return m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iItemTypeID);
}

int	TwTriggerFunc::TrgGetItemID(void)
{
	return m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iItemID);
}

int	TwTriggerFunc::TrgGetTriggerItemID(void)
{
	return m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iTriggerItemID);
}

BeBuffer* TwTriggerFunc::TrgGetPtr_Buffer(void)
{
	return (BeBuffer*)(m_pkTriggerMgr->GetParamVoid(TwTrgParamID::BTP_pkBuffer));
}

BeEffect* TwTriggerFunc::TrgGetPtr_Effect(void)
{
	return (BeEffect*)(m_pkTriggerMgr->GetParamVoid(TwTrgParamID::BTP_pkEffect));
}

BeItem* TwTriggerFunc::TrgGetPtr_Item(void)
{
	BeItem* pkItem = NULL;
	int iItemPos = m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iItemPos);
	std::shared_ptr<TwUnit> pkUnit = TrgGetPtr_Unit();
	if (pkUnit)
	{
		pkItem = pkUnit->GetItemByPos(iItemPos - 1);
	}
	return pkItem;
}

float TwTriggerFunc::TrgGetDamageFace(void)
{
	return m_pkTriggerMgr->GetParamFloat(TwTrgParamID::BTP_fDamageFace);
}

float* TwTriggerFunc::TrgGetPtr_Damage(void)
{
	return (float*)(m_pkTriggerMgr->GetParamVoid(TwTrgParamID::BTP_pfDamage));
}

float TwTriggerFunc::GetTrgAttackedRawDamge(void)
{
	return m_pkTriggerMgr->GetParamFloat(TwTrgParamID::BTP_fRawDamage);
}


BeAttackingAttr* TwTriggerFunc::TrgGetPtr_AttackAttr(void)
{
	return (BeAttackingAttr*)(m_pkTriggerMgr->GetParamVoid(TwTrgParamID::BTP_pkAttackAttr));
}

int* TwTriggerFunc::TrgGetPtr_Persist(void)
{
	return (int*)(m_pkTriggerMgr->GetParamVoid(TwTrgParamID::BTP_piPersistTime));
}

int* TwTriggerFunc::TrgGetPtr_PersistDelta(void)
{
	return (int*)(m_pkTriggerMgr->GetParamVoid(TwTrgParamID::BTP_piPersistDeltaTime));
}

int TwTriggerFunc::TrgGetTargetType(void)
{
	return (m_pkTriggerMgr->GetParamInt(TwTrgParamID::BTP_iTargetType));
}

TwPtCondResult TwTriggerFunc::DefaultBuffer_Condition(int iOriginTypeID)
{
    return TwPtCondResult::PTCR_OK;
}

bool TwTriggerFunc::DefaultSkill_ActionParam(std::shared_ptr<TwUnit>& pkTrgUnit, unsigned int& uiSkillTypeID, int& iSkillLevel, const SkillTable* pkSkillRes)
{
    return true;
}

void TwTriggerFunc::InitSkillEventTrigger(int iTypeID)
{

}

void TwTriggerFunc::InitItemEventTrigger(int iTypeID)
{

}
