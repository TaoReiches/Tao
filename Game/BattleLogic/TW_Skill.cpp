/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Skill.h"
#include "TW_Main.h"
#include "TW_Unit.h"
#include "TW_TriggerEvent.h"
#include "TW_UnitMgr.h"
#include "TW_IRandom.h"
#include "TW_TriggerMgr.h"

BeSkill::BeSkill(int iTypeID) : BeCarry(iTypeID)
{
	m_eType = BCT_SKILL;
	m_pkRes.reset();
	m_bCanForbid = true;
	m_bAddOnSkill = false;
	m_iCurPileNums = 0;
	m_iParentSkill = 0;
	m_eSkillType = OtherSkill;
	m_iLastTrgTime = 0;
	m_iSkillUICounter = 0;
	m_iAddOnSkillStartTime = 0;
}

BeSkill::~BeSkill()
{
}

bool BeSkill::Initialize(int iTypeID)
{
	BeCarry::Initialize(iTypeID);
	m_pkRes = SkillTableMgr::Get()->GetSkillTable(iTypeID);
	if (!m_pkRes)
	{
		return false;
	}
	SetActive(false);

	SetLevel(1);
	m_bCanForbid = true;
	m_bAddOnSkill = false;
	m_iParentSkill = 0;
	SetUIShowPos(m_pkRes->iUIShowPos);

	return true;
}

int	BeSkill::GetCDTime(BeUnit* pkUnit)
{
	SeCalSkillLvlData kData;
	if (!pkUnit->GetSkillLvlData(kData, GetTypeID()))
	{
		return 0;
	}

	return 10;
}

void BeSkill::SetLevel(int iLevel)
{
	if (!m_pkRes)
	{
		return;
	}

	if (iLevel > m_pkRes->iSkillMaxLevel)
	{
		iLevel = m_pkRes->iSkillMaxLevel;
	}
	if (iLevel < 1)
	{
		iLevel = 1;
	}
	int iOldLevel = m_kData.iCurLevel;
	m_kData.iCurLevel = iLevel;
	ClearAttr();
	InitAttrFromSkill(m_iTypeID, iLevel);

	TwPtParam kParam;
	kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, &gUnit);
	kParam.SetParam(TwTrgParamID::BTP_pkSkill, this);

	if (m_kData.iCurLevel != iOldLevel)
	{
		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_LEVEL_CHANGE, kParam);
	}
}

void BeSkill::ReInitAttr(void)
{
	ClearAttr();
	InitAttrFromSkill(m_iTypeID, m_kData.iCurLevel);
}

void BeSkill::SetLastUseTime(BeUnit* pkUnit, int iTime, bool bLearnSkill)
{
	int iID = m_iTypeID;

	//{
	//	if (bLearnSkill)
	//	{
	//		pkUnit->SetCommonCDLastUseTime(iID, iTime);
	//	}
	//	else
	//	{
	//		SeCalSkillLvlData	rkData;
	//		if (!pkUnit->GetSkillLvlData(rkData, iID))
	//		{
	//			return;
	//		}
	//		int iCDTime = rkData.GetCoolDown();
	//		int iPilePoint = rkData.GetPilePointNums();

	//		if (iPilePoint != 0 && m_iCurPileNums > 0)
	//		{
	//			if (m_iCurPileNums != iPilePoint)
	//			{
	//				int DecTime = iCDTime - (GetLastUseTime(pkUnit) + iCDTime - (int)gTime);
	//				pkUnit->SetCommonCDLastUseTime(iID, iTime - DecTime);
	//			}
	//			else
	//			{
	//				pkUnit->SetCommonCDLastUseTime(iID, iTime);
	//			}

	//			SetCurPileNums(--m_iCurPileNums);

	//			TePtParam kParam;
	//			kParam.SetParam(BTP_pkTrgUnit, pkUnit);
	//			kParam.SetParam(BTP_pkSkill, this);

	//			gTrgMgr.FireTrigger(BTE_SKILL_PILENUM, kParam);
	//		}
	//		else
	//		{
	//			pkUnit->SetCommonCDLastUseTime(iID, iTime);
	//		}
	//	}
	//}
}

bool BeSkill::CDComplete(BeUnit* pkUnit)
{
	if (IsSwitch() && IsActive())
	{
		return true;
	}

	if (m_iCurPileNums > 0)
	{
		return true;
	}

	SeCalSkillLvlData kData;
	if (!pkUnit->GetSkillLvlData(kData, GetTypeID()))
	{
		return false;
	}

	// return pkUnit->CommonCDComplete(m_iTypeID, gData.GetCoolDown(kData));
}

void BeSkill::Update(BeUnit* pkUnit, int iItemPos)
{
	SeCalSkillLvlData kData;
	if (!pkUnit->GetSkillLvlData(kData, GetTypeID()))
	{
		return;
	}
	SeCalSkillLvlData& rkData = kData;

	if (rkData.GetPilePointNums() > 0)
	{
		//if (m_iCurPileNums < rkData.GetPilePointNums() && pkUnit->CommonCDComplete(GetTypeID(), gData.GetCoolDown(rkData)))
		{
			//pkUnit->SetCommonCDLastUseTime(GetShareCDSkill(), (int)gTime);
			SetCurPileNums(++m_iCurPileNums);

			TwPtParam kParam;
			kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, pkUnit);
			kParam.SetParam(TwTrgParamID::BTP_pkSkill, this);

			gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_PILENUM, kParam);
		}
	}

	if (HasProperty(SKILL_SKILLPROPERTY_CDTRIGGER))
	{
		//if (gData.GetCoolDown(rkData) > 0 && CDComplete(pkUnit))
		{
			TwPtParam kParam;
			kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, pkUnit);
			kParam.SetParam(TwTrgParamID::BTP_pkSkill, this);
			kParam.SetParam(TwTrgParamID::BTP_iItemPos, iItemPos + 1);

			gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_ONCDCOMPLETE, kParam);
		}
	}
}

int BeSkill::GetLastUseTime(const BeUnit* pkUnit) const
{
	//return pkUnit->GetCommonCDLastUseTime(GetShareCDSkill());
    return 0;
}

int BeSkill::GetCurPileNums(void) const
{
	return m_iCurPileNums;
}

void BeSkill::SetCurPileNums(int iPileNums)
{
	m_iCurPileNums = iPileNums;

	SetUICounter(iPileNums);
}

void BeSkill::SetActive(bool bActive)
{
	//if ((m_pkRes->uiSkillProperty & SKILL_SKILLPROPERTY_ZIDONGCHIXU) != 0)
	//{
	//	m_kData.bIsActive = bActive;

	//	BeUnit* pkUnit = (BeUnit*)pkAttachUnit;
	//	if (pkUnit)
	//	{
	//		pkUnit->UpdateAttribute(true);
	//	}
	//}
	//else if (IsSwitch())
	{
		m_kData.bIsActive = bActive;
	}
}

void BeSkill::DefaultAutoUse(void)
{
	float fRate = 0.2f;
	if (gUnit.GetHP() < gUnit.GetMaxHP() * 0.2f)
	{
		fRate = 0.5f;
	}
	if (gRandNum.RandFloat(0.0f, 1.0f) > fRate)
	{
		return;
	}

	UnitGroup kGroup;
	gUnitMgr.GetAreaGroup(kGroup, gUnit.GetPosX(), gUnit.GetPosY(), 600.0f, &gUnit, BUDP_ENEMY, BUSP_NORMAL);
	if (kGroup.empty())
	{
		return;
	}

	kGroup.clear();
	gUnitMgr.GetAreaGroup(kGroup, gUnit.GetPosX(), gUnit.GetPosY(), 600.0f, &gUnit, BUDP_ENEMY, BUSP_HERO);
	if (kGroup.empty())
	{
		if (gRandNum.RandFloat(0.0f, 1.0f) > 0.20f)
		{
			return;
		}
	}
}
