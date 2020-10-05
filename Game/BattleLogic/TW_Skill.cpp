/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Skill.h"

#ifdef _WIN32
enum UnitAutoSpellType;
#else
//int UnitAuotSpellType;
#endif


BeSkill::BeSkill(int iTypeID) : BeCarry(iTypeID)
{
	m_eType = BCT_SKILL;
	memset(&m_kData, 0, sizeof(BeSkillData));
	m_pkRes = NULL;
	m_bCanForbid = true;
	m_bAddOnSkill = false;
	m_iCurPileNums = 0;
	m_iParentSkill = 0;
	m_eSkillType = OtherSkill;
	m_iLastTrgTime = 0;
	m_bImbaAutoUse = false;
	m_iSkillUICounter = 0;
	m_iAddOnSkillStartTime = 0;
}

BeSkill::~BeSkill()
{
}

bool BeSkill::Initialize(int iTypeID)
{
	BeCarry::Initialize(iTypeID);
	m_pkRes = gMain.GetResSkill(iTypeID);

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
	SetUIShowPos(m_pkRes->iUIShowPos - 1);

	return true;
}

int	BeSkill::GetCDTime(BeUnit* pkUnit)
{
	SeCalSkillLvlData kData;
	if (!pkUnit->GetSkillLvlData(kData, GetTypeID()))
	{
		return 0;
	}

	return gData.GetCoolDown(kData);
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

	TePtParam kParam;
	kParam.SetParam(BTP_pkTrgUnit, &gUnit);
	kParam.SetParam(BTP_pkSkill, this);

	if (m_kData.iCurLevel != iOldLevel)
	{
		gTrgMgr.FireTrigger(BTE_SKILL_LEVEL_CHANGE, kParam);
	}

	if (iOldLevel > 0 && m_kData.iCurLevel != iOldLevel)
	{
		BeTDEventArgs kArgs = BeTDEventArgs();
		kArgs.iEventType = BeTDEventArgs::BeTDEVENT_SPELL_LEVELCHANGE;
		kArgs.kSpellLevelChange.iUnitID = gUnit.GetID();
		kArgs.kSpellLevelChange.iSkillTypeID = GetTypeID();
		kArgs.kSpellLevelChange.iSkillLevel = GetLevel();
		gLevelMain.GetEventMgr()->FireEvent(kArgs);
	}

	if (GetUIShowPos() == 3)
	{
		gUnit.SetTabInfoFlag(BTCF_BIGSKILL);
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

	if (!gData.IsWTF())
	{
		if (bLearnSkill)
		{
			pkUnit->SetCommonCDLastUseTime(iID, iTime);
		}
		else
		{
			SeCalSkillLvlData	rkData;
			if (!pkUnit->GetSkillLvlData(rkData, iID))
			{
				return;
			}
			int iCDTime = rkData.GetCoolDown();
			int iPilePoint = rkData.GetPilePointNums();

			if (iPilePoint != 0 && m_iCurPileNums > 0)
			{
				if (m_iCurPileNums != iPilePoint)
				{
					int DecTime = iCDTime - (GetLastUseTime(pkUnit) + iCDTime - (int)gTime);
					pkUnit->SetCommonCDLastUseTime(iID, iTime - DecTime);
				}
				else
				{
					pkUnit->SetCommonCDLastUseTime(iID, iTime);
				}

				SetCurPileNums(--m_iCurPileNums);

				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, pkUnit);
				kParam.SetParam(BTP_pkSkill, this);

				gTrgMgr.FireTrigger(BTE_SKILL_PILENUM, kParam);
			}
			else
			{
				pkUnit->SetCommonCDLastUseTime(iID, iTime);
			}
		}
	}
}

bool BeSkill::CDComplete(BeUnit* pkUnit)
{
	if (IsSwitch() && IsActive())
	{
		return true;
	}

	if (m_iCurPileNums > 0 || (m_iSkillUICounter > 0 && m_iSkillUICounter != 234
		&& GetTypeID() != 'SM46'))
	{
		return true;
	}

	SeCalSkillLvlData kData;
	if (!pkUnit->GetSkillLvlData(kData, GetTypeID()))
	{
		return false;
	}

	return pkUnit->CommonCDComplete(m_iTypeID, gData.GetCoolDown(kData));
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
		if (m_iCurPileNums < rkData.GetPilePointNums() && pkUnit->CommonCDComplete(GetTypeID(), gData.GetCoolDown(rkData)))
		{
			pkUnit->SetCommonCDLastUseTime(GetShareCDSkill(), (int)gTime);
			SetCurPileNums(++m_iCurPileNums);

			TePtParam kParam;
			kParam.SetParam(BTP_pkTrgUnit, pkUnit);
			kParam.SetParam(BTP_pkSkill, this);

			gTrgMgr.FireTrigger(BTE_SKILL_PILENUM, kParam);
		}
	}

	if (HasProperty(SKILL_SKILLPROPERTY_CDCHUFA))
	{
		if (gData.GetCoolDown(rkData) > 0 && CDComplete(pkUnit))
		{
			TePtParam kParam;
			kParam.SetParam(BTP_pkTrgUnit, pkUnit);
			kParam.SetParam(BTP_pkSkill, this);
			kParam.SetParam(BTP_iItemPos, iItemPos + 1);

			gTrgMgr.FireTrigger(BTE_SKILL_ONCDCOMPLETE, kParam);
		}
	}
}

int BeSkill::GetLastUseTime(const BeUnit* pkUnit) const
{
	return pkUnit->GetCommonCDLastUseTime(GetShareCDSkill());
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

const std::string& BeSkill::GetSkillLvlTip(void)
{
	return m_kSkillLvlTipDec;
}

const std::string& BeSkill::GetSkillLvlLearnTip(void)
{
	return m_kSkillLvlLearnTipDec;
}

void BeSkill::SetActive(bool bActive)
{
	if ((m_pkRes->uiSkillProperty & SKILL_SKILLPROPERTY_ZIDONGCHIXU) != 0)
	{
		m_kData.bIsActive = bActive;

		BeUnit* pkUnit = (BeUnit*)pkAttachUnit;
		if (pkUnit)
		{
			pkUnit->UpdateAttribute(true);
		}
	}
	else if (IsSwitch())
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
	if (gRandNum.RandFloat(RANDINFO, 0.0f, 1.0f) > fRate)
	{
		return;
	}

	UnitGroup kGroup;
	gUnitMgr.GetAreaGroup(kGroup, gUnit.GetPosX(), gUnit.GetPosY(), 600.0f, &gUnit, BUDP_ENEMY, BUSP_NORMAL);
	if (kGroup.IsEmpty())
	{
		return;
	}

	kGroup.Clear();
	gUnitMgr.GetAreaGroup(kGroup, gUnit.GetPosX(), gUnit.GetPosY(), 600.0f, &gUnit, BUDP_ENEMY, BUSP_HERO);
	if (kGroup.IsEmpty())
	{
		if (gRandNum.RandFloat(RANDINFO, 0.0f, 1.0f) > 0.20f)
		{
			return;
		}
	}
}

void BeSkill::HandleEvent(int iEvent, TePtParam& kParam)
{
	HandleVector::iterator iter = m_kHandles.begin();
	for (; iter != m_kHandles.end(); iter++)
	{
		if ((*iter).first == iEvent)
		{
			(this->*((*iter).second))(iEvent, kParam);
		}
	}
}

void BeSkill::HandleDamageCDEvent(int iEvent, TePtParam& kParam)
{
}

void BeSkill::HandleBeDamagedCDEvent(int iEvent, TePtParam& kParam)
{
	BeUnit* pkAttacker = (BeUnit*)kParam.GetParamVoid(BTP_pkAttacker);
	if (!pkAttacker || !pkAttacker->IsHero())
	{
		return;
	}
}

extern BeUnit* CreateMajia(BeUnit* pkTrgUnit);

void BeSkill::HandleDamageEvent(int iEvent, TePtParam& kParam)
{
	this->SetLastTrgTime(gTime);
}

void BeSkill::RegisterHandle(int iEvent, HandleFunc fuc)
{
	m_kHandles.push_back(std::make_pair(iEvent, fuc));
}
