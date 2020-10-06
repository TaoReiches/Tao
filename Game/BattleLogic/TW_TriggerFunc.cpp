/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TriggerEvent.h"

BeCarry* BeMain::GetTrgCarry(void)
{
	return (BeCarry*)(m_pkTriggerMgr->GetParamVoid(BTP_pkCarray));
}

int	BeMain::GetTrgSkillTypeID(void)
{
	int iSkillTypeID = m_pkTriggerMgr->GetParamInt(BTP_iSkillTypeID);
	return iSkillTypeID;
}

int BeMain::GetTrgSkillOrgTypeID(void)
{
	int iSkillTypeID = m_pkTriggerMgr->GetParamInt(BTP_iSkillOrgTypeID);
	return iSkillTypeID;
}

int BeMain::GetTrgSkillLevel(void)
{
	int iSkillLevel = m_pkTriggerMgr->GetParamInt(BTP_iSkillLevel);
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

int	BeMain::GetTrgSpellUsePlayer(void)
{
	return m_pkTriggerMgr->GetParamInt(BTP_iSkillUsePlayer);
}

int	BeMain::GetTrgSpellTargetID(void)
{
	return m_pkTriggerMgr->GetParamInt(BTP_iSpellTargetID);
}

int* BeMain::GetTrgPtrSpellTargetID(void)
{
	return (int*)(m_pkTriggerMgr->GetParamVoid(BTP_piSpellTargetID));
}

float BeMain::GetTrgSpellOrgPosX(void)
{
	return m_pkTriggerMgr->GetParamFloat(BTP_fSpellOrgPosX);
}

float BeMain::GetTrgSpellOrgPosY(void)
{
	return m_pkTriggerMgr->GetParamFloat(BTP_fSpellOrgPosY);
}

float BeMain::GetTrgSpellTargetPosX(void)
{
	return m_pkTriggerMgr->GetParamFloat(BTP_fSpellTargetPosX);
}

float BeMain::GetTrgSpellTargetPosY(void)
{
	return m_pkTriggerMgr->GetParamFloat(BTP_fSpellTargetPosY);
}

float BeMain::GetTrgSpellDirectPosX(void)
{
	return m_pkTriggerMgr->GetParamFloat(BTP_fSpellDirectPosX);
}

float BeMain::GetTrgSpellDirectPosY(void)
{
	return m_pkTriggerMgr->GetParamFloat(BTP_fSpellDirectPosY);
}

BeUnit* BeMain::GetTrgUnit(bool bIncSuspend)
{
	return TrgGetPtr_Unit();
}

BeUnit* BeMain::GetTrgAttacker(void)
{
	return TrgGetPtr_Attacker();
}

BeUnit* BeMain::GetTrgTarget(void)
{
	return TrgGetPtr_Target();
}

float BeMain::GetTrgAttackedDamge(void)
{
	return *(TrgGetPtr_Damage());
}

int BeMain::GetTrgAttackedType(void)
{
	return m_pkTriggerMgr->GetParamInt(BTP_iAttackType);
}

int BeMain::GetTrgAttackedIsFirst(void)
{
	return GetTrgAttackedFlag() & BAF_FIRST;
}

int BeMain::GetTrgAttackedFlag(void)
{
	return m_pkTriggerMgr->GetParamInt(BTP_iFlag);
}
int* BeMain::GetTrgAttackedFlagPtr(void)
{
	return (int*)m_pkTriggerMgr->GetParamInt(BTP_iFlagPtr);
}
int	BeMain::GetTrgTargetID()
{
	return m_pkTriggerMgr->GetParamInt(BTP_iTargetID);
}
int	BeMain::GetTrgAttackerPlayer(void)
{
	return m_pkTriggerMgr->GetParamInt(BTP_iAttackerPlayer);
}

int BeMain::GetTrgBufferTypeID(void)
{
	return TrgGetPtr_Buffer()->GetTypeID();
}

int BeMain::GetTrgBufferLevel(void)
{
	return TrgGetPtr_Buffer()->GetLevel();
}

int BeMain::GetTrgBufferInnerID(void)
{
	return TrgGetPtr_Buffer()->GetID();
}

int	BeMain::GetTrgIntData(void)
{
	return m_pkTriggerMgr->GetParamInt(BTP_iIntData);
}

int BeMain::GetTrgMissionPlayer(void)
{
	return m_pkTriggerMgr->GetParamInt(BTP_iMissionPlayer);
}

int BeMain::GetTrgMissionID(void)
{
	return m_pkTriggerMgr->GetParamInt(BTP_iMissionID);
}

float BeMain::GetTrgFloatData(void)
{
	return m_pkTriggerMgr->GetParamFloat(BTP_fFloatData);
}

bool BeMain::GetTrgIsLearnSkill(void)
{
	return m_pkTriggerMgr->GetParamInt(BTP_iIsLearnSkill) == 1 ? true : false;
}

BeUnit* BeMain::TrgGetPtr_Unit(void)
{
	return (BeUnit*)(m_pkTriggerMgr->GetParamVoid(BTP_pkTrgUnit));
}

BeMapItem* BeMain::TrgGetPtr_MapItem(void)
{
	return (BeMapItem*)(m_pkTriggerMgr->GetParamVoid(BTP_pkMapItem));
}

BeUnit* BeMain::TrgGetPtr_Attacker(void)
{
	return (BeUnit*)(m_pkTriggerMgr->GetParamVoid(BTP_pkAttacker));
}

BeUnit* BeMain::TrgGetPtr_Target(void)
{
	return (BeUnit*)(m_pkTriggerMgr->GetParamVoid(BTP_pkTarget));
}

BeSkill* BeMain::TrgGetPtr_Skill(void)
{
	int iItemPos = m_pkTriggerMgr->GetParamInt(BTP_iItemPos) - 1;
	if (iItemPos >= 0 && iItemPos < UNIT_MAX_ITEM)
	{
		BeItem* pkItem = TrgGetPtr_Item();
		if (!pkItem)
		{
			return NULL;
		}
	}
	return (BeSkill*)(m_pkTriggerMgr->GetParamVoid(BTP_pkSkill));
}

int BeMain::TrgGetItemPos(void)
{
	return (m_pkTriggerMgr->GetParamInt(BTP_iItemPos) - 1);
}

int	BeMain::TrgGetItemTypeID(void)
{
	return gTrgMgr.GetParamInt(BTP_iItemTypeID);
}

int	BeMain::TrgGetItemID(void)
{
	return gTrgMgr.GetParamInt(BTP_iItemID);
}

int	BeMain::TrgGetTriggerItemID(void)
{
	return gTrgMgr.GetParamInt(BTP_iTriggerItemID);
}

BeBuffer* BeMain::TrgGetPtr_Buffer(void)
{
	return (BeBuffer*)(m_pkTriggerMgr->GetParamVoid(BTP_pkBuffer));
}

BeEffect* BeMain::TrgGetPtr_Effect(void)
{
	return (BeEffect*)(m_pkTriggerMgr->GetParamVoid(BTP_pkEffect));
}

BeItem* BeMain::TrgGetPtr_Item(void)
{
	BeItem* pkItem = NULL;
	int iItemPos = m_pkTriggerMgr->GetParamInt(BTP_iItemPos);
	BeUnit* pkUnit = TrgGetPtr_Unit();
	if (pkUnit)
	{
		pkItem = pkUnit->GetItemByPos(iItemPos - 1);
	}
	return pkItem;
}

float BeMain::TrgGetDamageFace(void)
{
	return m_pkTriggerMgr->GetParamFloat(BTP_fDamageFace);
}

float* BeMain::TrgGetPtr_Damage(void)
{
	return (float*)(m_pkTriggerMgr->GetParamVoid(BTP_pfDamage));
}

float BeMain::GetTrgAttackedRawDamge(void)
{
	return m_pkTriggerMgr->GetParamFloat(BTP_fRawDamage);
}


BeAttackingAttr* BeMain::TrgGetPtr_AttackAttr(void)
{
	return (BeAttackingAttr*)(m_pkTriggerMgr->GetParamVoid(BTP_pkAttackAttr));
}

int* BeMain::TrgGetPtr_Persist(void)
{
	return (int*)(m_pkTriggerMgr->GetParamVoid(BTP_piPersistTime));
}

int* BeMain::TrgGetPtr_PersistDelta(void)
{
	return (int*)(m_pkTriggerMgr->GetParamVoid(BTP_piPersistDeltaTime));
}

int BeMain::TrgGetTargetType(void)
{
	return (m_pkTriggerMgr->GetParamInt(BTP_iTargetType));
}
