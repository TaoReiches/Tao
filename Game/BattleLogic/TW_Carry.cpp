/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Carry.h"
#include "TW_UnitDefine.h"
#include "TW_TriggerMgr.h"
#include "TW_TriggerEvent.h"
#include "TW_Main.h"
#include "Skill_table.hpp"

BeCarry::BeCarry(int iID) :m_iID(iID)
{
	m_eType = BCT_BASE;
	m_iTypeID = 0;
	m_iCarryFlag = 0;
	m_iImmunityFlag = 0;
	m_iOrbSkill = 0;
	m_iOrbLevel = 0;
	m_iMissileModel = 0;
}

BeCarry::~BeCarry(void)
{
}

bool BeCarry::Initialize(int iTypeID)
{
	m_iTypeID = iTypeID;
	return true;
}

void BeCarry::ClearAttr(void)
{
	m_akNormalAttr.clear();
	m_akBackNormalAttr.clear();
	m_akAttackedAttr.clear();
}

void BeCarry::SetNormalAttr(BeNormalAttType eType, float fValue, bool bForceChange, bool bAdd)
{
	if (eType < NAT_ABS_ALL || eType >= NAT_MAX_NUM)
	{
		return;
	}

	SetFlag(BCF_HAS_NORMAL_ATTR);
	for (std::vector<BeNormalAtt>::iterator itr = m_akNormalAttr.begin(); itr != m_akNormalAttr.end(); ++itr)
	{
		BeNormalAtt& rkAttr = *itr;
		if (rkAttr.eType == eType)
		{
			if (bForceChange)
			{
				rkAttr.fValue = fValue;
				return;
			}
			else if (bAdd)
			{
				rkAttr.fValue += fValue;
				return;
			}
			if (abs(rkAttr.fValue) >= abs(fValue))
			{
				if (!m_akBackNormalAttr.empty())
				{
					bool bHasType = false;
					for (std::vector<BeNormalAtt>::iterator itr2 = m_akBackNormalAttr.begin(); itr2 != m_akBackNormalAttr.end(); ++itr2)
					{
						BeNormalAtt& rkAttr2 = *itr2;
						if (rkAttr2.eType == eType)
						{
							bHasType = true;
							if (gTime - (int)rkAttr2.fValue > 3000)
							{
								rkAttr.fValue = fValue;
								m_akBackNormalAttr.erase(itr2);
								return;
							}
						}
					}

					if (!bHasType)
					{
						m_akBackNormalAttr.push_back(BeNormalAtt(eType, (float)gTime));
					}
				}
				else
				{
					m_akBackNormalAttr.push_back(BeNormalAtt(eType, (float)gTime));
				}
			}
			else
			{
				if (!m_akBackNormalAttr.empty())
				{
					for (std::vector<BeNormalAtt>::iterator itr2 = m_akBackNormalAttr.begin(); itr2 != m_akBackNormalAttr.end(); ++itr2)
					{
						BeNormalAtt& rkAttr2 = *itr2;
						if (rkAttr2.eType == eType)
						{
							rkAttr2.fValue = (float)gTime;
							break;
						}
					}
				}

				rkAttr.fValue = fValue;
			}
			return;
		}
	}

	m_akNormalAttr.push_back(BeNormalAtt(eType, fValue));
}

std::vector<BeNormalAtt>& BeCarry::GetNormalAttr(void)
{
	return m_akNormalAttr;
}

float	BeCarry::GetNormalAttrValue(BeNormalAttType eType)
{
	if (!m_akNormalAttr.empty())
	{
		for (int iIndex = 0; iIndex < m_akNormalAttr.size(); iIndex++)
		{
			BeNormalAtt& kAttr = m_akNormalAttr[iIndex];
			if (kAttr.eType == eType)
			{
				return kAttr.fValue;
			}
		}
	}
	return 0.0f;
}

void BeCarry::ApplyNormalAttr(float afChange[BeCarryType::BCT_NUM][BeNormalAttType::NAT_MAX_NUM][2], bool bGhost, int iImmunityFlag)
{
	float fMaxSpeed = 0.0f;
	for (std::vector<BeNormalAtt>::iterator itr = m_akNormalAttr.begin(); itr != m_akNormalAttr.end(); ++itr)
	{
		BeNormalAtt& rkAttr = *itr;

		if (IsImmunity(rkAttr, iImmunityFlag))
		{
			continue;
		}
		if (rkAttr.eType == NAT_ABS_ALL)
		{
			afChange[m_eType][NAT_ABS_STR][0] += rkAttr.fValue;
			afChange[m_eType][NAT_ABS_AGI][0] += rkAttr.fValue;
			afChange[m_eType][NAT_ABS_INT][0] += rkAttr.fValue;
		}
		else if (rkAttr.eType == NAT_PER_SKILLBLAST)
		{
			float fRadio = 1.0f - afChange[m_eType][NAT_PER_SKILLBLAST][0];
			fRadio *= 1.0f - rkAttr.fValue;
			afChange[m_eType][NAT_PER_SKILLBLAST][0] = 1.0f - fRadio;
		}
		else if (
			rkAttr.eType == NAT_DEC_MPCOST
			|| rkAttr.eType == NAT_PER_CDTIME
			)
		{
			if (m_eType == BCT_EQUIP || m_eType == BCT_SUITING || m_eType == BCT_GENIUS)
			{
				if (rkAttr.fValue > 0.0f)
				{
					afChange[m_eType][rkAttr.eType][0] += rkAttr.fValue;
				}
				else
				{
					afChange[m_eType][rkAttr.eType][1] += rkAttr.fValue;
				}
			}
			else
			{
				if (rkAttr.fValue > afChange[m_eType][rkAttr.eType][0])
				{
					afChange[m_eType][rkAttr.eType][0] = rkAttr.fValue;
				}
				else if (rkAttr.fValue < afChange[m_eType][rkAttr.eType][1])
				{
					afChange[m_eType][rkAttr.eType][1] = rkAttr.fValue;
				}
			}

		}
		else if (rkAttr.eType == NAT_ABS_ARMOR || rkAttr.eType == NAT_PER_ARMOR || rkAttr.eType == NAT_PER_ATTACK_SPEED || rkAttr.eType == NAT_ABS_DAMAGE
			|| rkAttr.eType == NAT_ABS_REGEN_HP
			|| rkAttr.eType == NAT_PER_REGEN_HP
			|| rkAttr.eType == NAT_ABS_REGEN_MP
			|| rkAttr.eType == NAT_PER_REGEN_MP
			|| rkAttr.eType == NAT_MAGIC_ARMOR
			)
		{
			if ((m_eType == BCT_ITEM || m_eType == BCT_SKILL) && bGhost)
			{
			}
			else
			{
				if (rkAttr.fValue > 0)
				{
					afChange[m_eType][rkAttr.eType][0] += rkAttr.fValue;
				}
				else
				{
					afChange[m_eType][rkAttr.eType][1] += rkAttr.fValue;
				}
			}
		}
		else if (rkAttr.eType == NAT_PER_BAOJI) {

			if (rkAttr.fValue > 0)
			{
				if (afChange[m_eType][rkAttr.eType][0] == 0) {

					afChange[m_eType][rkAttr.eType][0] = 1;
				}

				afChange[m_eType][rkAttr.eType][0] *= rkAttr.fValue;
			}

		}
		else if (rkAttr.eType == NAT_PER_MOVE_SPEED) {

			if (afChange[m_eType][rkAttr.eType][1] == 0) {

				afChange[m_eType][rkAttr.eType][1] = 1;
			}

			if (rkAttr.fValue > 0)
			{
				afChange[m_eType][rkAttr.eType][0] += rkAttr.fValue;
			}
			else if (rkAttr.fValue < 0)
			{
				afChange[m_eType][rkAttr.eType][1] *= 1 + rkAttr.fValue;
			}

		}
		else
		{
			if (rkAttr.fValue > 0)
			{
				afChange[m_eType][rkAttr.eType][0] += rkAttr.fValue;
			}
			else
			{
				afChange[m_eType][rkAttr.eType][1] += rkAttr.fValue;
			}
		}
	}
}

bool BeCarry::IsImmunity(const BeNormalAtt& rkAttr, int iImmunityFlag)
{
	if (rkAttr.fValue >= 0.0f)
	{
		if ((iImmunityFlag & BUIF_BEDEMAGED_PER_IM) && rkAttr.eType == NAT_PER_BEDEMAGED)
		{
			return true;
		}

		return false;
	}

	if (iImmunityFlag & BUIF_DAMAGE_DEC_IM)
	{
		if (rkAttr.eType == NAT_ABS_DAMAGE || rkAttr.eType == NAT_PER_DAMAGE_BASE || rkAttr.eType == NAT_PER_DAMAGE_ADD)
		{
			return true;
		}
	}

	if (iImmunityFlag & BUIF_SPEED_SLOW_IM)
	{
		if (rkAttr.eType == NAT_ABS_MOVE_SPEED || rkAttr.eType == NAT_PER_MOVE_SPEED)
		{
			return true;
		}
	}

	if (iImmunityFlag & BUIF_ARMOR_DEC_IM)
	{
		if (rkAttr.eType == NAT_ABS_ARMOR || rkAttr.eType == NAT_PER_ARMOR)
		{
			return true;
		}
	}

	return false;
}

void BeCarry::SetMissileModel(int iMissileModel)
{
	m_iMissileModel = iMissileModel;
}

int BeCarry::GetMissileModel(void)
{
	return m_iMissileModel;
}

bool BeCarry::GetAttackMissPhysic(void)
{
	return false;
}

void BeCarry::SetAttackedAttr(BeAttackedAttr& rkAttackedAttr)
{
	for (std::vector<BeAttackedAttr>::iterator itr = m_akAttackedAttr.begin(); itr != m_akAttackedAttr.end(); ++itr)
	{
		BeAttackedAttr& rkAttr = *itr;
		if (rkAttr.eType == rkAttackedAttr.eType)
		{
			rkAttr = rkAttackedAttr;
			return;
		}
	}

	m_akAttackedAttr.push_back(rkAttackedAttr);
	SetFlag(BCF_HAS_NORMAL_ATTR);
}

std::vector<BeAttackedAttr>& BeCarry::GetAttackedAttr(void)
{
	return m_akAttackedAttr;
}

float BeCarry::GetAttackedAvoidPhysic(void)
{
	if (!m_akAttackedAttr.empty())
	{
		for (std::vector<BeAttackedAttr>::iterator itr = m_akAttackedAttr.begin(); itr != m_akAttackedAttr.end(); ++itr)
		{
			BeAttackedAttr& rkAttr = *itr;
			if (rkAttr.eType == AAT_AVOID_PHYSIC)
			{
				return rkAttr.kAvoidPhysic.fRate;
			}
		}
	}

	return 0.0f;
}

BeResistPhysicAttr* BeCarry::GetShortRangeAttackedResistPhysicAttr(void)
{
	BeResistPhysicAttr* pkTemp = NULL;
	if (!m_akAttackedAttr.empty())
	{
		float fMaxResistValue = 0.0f;
		for (std::vector<BeAttackedAttr>::iterator itr = m_akAttackedAttr.begin(); itr != m_akAttackedAttr.end(); ++itr)
		{
			BeAttackedAttr& rkAttr = *itr;
		}
	}

	return pkTemp;
}

BeResistPhysicAttr* BeCarry::GetEjectAttackedResistPhysicAttr(void)
{
	BeResistPhysicAttr* pkTemp = NULL;
	if (!m_akAttackedAttr.empty())
	{
		float fMaxResistValue = 0.0f;
		for (std::vector<BeAttackedAttr>::iterator itr = m_akAttackedAttr.begin(); itr != m_akAttackedAttr.end(); ++itr)
		{
			BeAttackedAttr& rkAttr = *itr;
		}
	}

	return pkTemp;
}

float BeCarry::GetShortRangeAttackedResistPhysic(void)
{
	if (!m_akAttackedAttr.empty())
	{
		float fMaxResistValue = 0.0f;
		for (std::vector<BeAttackedAttr>::iterator itr = m_akAttackedAttr.begin(); itr != m_akAttackedAttr.end(); ++itr)
		{
			BeAttackedAttr& rkAttr = *itr;
		}
		return fMaxResistValue;
	}

	return 0.0f;
}

float BeCarry::GetEjectAttackedResistPhysic(void)
{
	if (!m_akAttackedAttr.empty())
	{
		float fMaxResistValue = 0.0f;
		for (std::vector<BeAttackedAttr>::iterator itr = m_akAttackedAttr.begin(); itr != m_akAttackedAttr.end(); ++itr)
		{
			BeAttackedAttr& rkAttr = *itr;
		}
		return fMaxResistValue;
	}

	return 0.0f;
}


float BeCarry::GetAttackedAntiPhysic(void)
{
	if (!m_akAttackedAttr.empty())
	{
		for (std::vector<BeAttackedAttr>::iterator itr = m_akAttackedAttr.begin(); itr != m_akAttackedAttr.end(); ++itr)
		{
			BeAttackedAttr& rkAttr = *itr;
			if (rkAttr.eType == AAT_ANTI_PHYSIC)
			{
				return rkAttr.kAntiPhysic.fAntiPhysic;
			}
		}
	}

	return 0.0f;
}

float BeCarry::GetAttackedAntiMagic(void)
{
	if (!m_akAttackedAttr.empty())
	{
		for (std::vector<BeAttackedAttr>::iterator itr = m_akAttackedAttr.begin(); itr != m_akAttackedAttr.end(); ++itr)
		{
			BeAttackedAttr& rkAttr = *itr;
			if (rkAttr.eType == AAT_ANTI_MAGIC)
			{
				return rkAttr.kAntiMagic.fAntiMagic;
			}
		}
	}

	return 0.0f;
}

float BeCarry::GetAttackedAntiLeech(void)
{
	if (!m_akAttackedAttr.empty())
	{
		for (std::vector<BeAttackedAttr>::iterator itr = m_akAttackedAttr.begin(); itr != m_akAttackedAttr.end(); ++itr)
		{
			BeAttackedAttr& rkAttr = *itr;
			if (rkAttr.eType == AAT_ANTI_LEECH)
			{
				return rkAttr.kAntiLeech.fAntiLeech;
			}
		}
	}

	return 0.0f;
}

float BeCarry::GetAttackedReboundSkill(void)
{
	float fPercent = 0.0f;

	if (!m_akAttackedAttr.empty())
	{
		for (std::vector<BeAttackedAttr>::iterator itr = m_akAttackedAttr.begin(); itr != m_akAttackedAttr.end(); ++itr)
		{
			BeAttackedAttr& rkAttr = *itr;
			if (rkAttr.eType == AAT_REBOUND_SKILL)
			{
				fPercent += rkAttr.kReboundSkill.fRebound;
			}
		}
	}

	return fPercent;
}

float BeCarry::GetAttackedReboundSkillAll(void)
{
	float fPercent = 0.0f;

	if (!m_akAttackedAttr.empty())
	{
		for (std::vector<BeAttackedAttr>::iterator itr = m_akAttackedAttr.begin(); itr != m_akAttackedAttr.end(); ++itr)
		{
			BeAttackedAttr& rkAttr = *itr;
			if (rkAttr.eType == AAT_REBOUND_SKILL_ALL)
			{
				fPercent += rkAttr.kReboundSkillAll.fRebound;
			}
		}
	}

	return fPercent;
}

float BeCarry::GetAttackedReboundPhysicShortRange(void)
{
	float fValue = 0.0f;

	if (!m_akAttackedAttr.empty())
	{
		for (std::vector<BeAttackedAttr>::iterator itr = m_akAttackedAttr.begin(); itr != m_akAttackedAttr.end(); ++itr)
		{
			BeAttackedAttr& rkAttr = *itr;
			if (rkAttr.eType == AAT_REBOUND_PHYSIC_SHORTRANGE)
			{
				fValue += rkAttr.kReboundPhysicShortRange.fPhysicValue;
			}
		}
	}

	return fValue;
}

void BeCarry::GetAttackedReboundPhysicBoth(float& fShorRange, float& fEject)
{
	if (!m_akAttackedAttr.empty())
	{
		for (std::vector<BeAttackedAttr>::iterator itr = m_akAttackedAttr.begin(); itr != m_akAttackedAttr.end(); ++itr)
		{
			BeAttackedAttr& rkAttr = *itr;
			if (rkAttr.eType == AAT_REBOUND_PHYSIC_BOTH)
			{
				fShorRange += rkAttr.kReboundPhysicBoth.fShortRangeValue;
				fEject += rkAttr.kReboundPhysicBoth.fEjectValue;
			}
		}
	}
}

void BeCarry::SetNormalAttrByData(int eType, float fValue, bool bAdd)
{
	if (eType >= NAT_ABS_ALL && eType < NAT_MAX_NUM)
	{
		SetNormalAttr((BeNormalAttType)(eType), fValue, false, bAdd);
	}
}

void BeCarry::InitNormalAttr(const SkillTable* pkSkillRes, int iSkillLevel, int iSkillTypeID)
{
	if (iSkillLevel < 1 || iSkillLevel > 5)
	{
		return;
	}
	iSkillLevel -= 1;
	for (int j = 0; j < MAX_CONTENT_DATA; j++)
	{
		int eType = 0;
		float fValue = 0.0f;
		switch (j)
		{
		case 0:
			eType = pkSkillRes->uiDataContentA;
			fValue = pkSkillRes->fValueA[iSkillLevel];
			break;
		case 1:
			eType = pkSkillRes->uiDataContentB;
			fValue = pkSkillRes->fValueB[iSkillLevel];
			break;
		case 2:
			eType = pkSkillRes->uiDataContentC;
			fValue = pkSkillRes->fValueC[iSkillLevel];
			break;
		case 3:
			eType = pkSkillRes->uiDataContentD;
			fValue = pkSkillRes->fValueD[iSkillLevel];
			break;
		case 4:
			eType = pkSkillRes->uiDataContentE;
			fValue = pkSkillRes->fValueE[iSkillLevel];
			break;
		case 5:
			eType = pkSkillRes->uiDataContentF;
			fValue = pkSkillRes->fValueF[iSkillLevel];
			break;
		case 6:
			eType = pkSkillRes->uiDataContentG;
			fValue = pkSkillRes->fValueG[iSkillLevel];
			break;
		default:
			break;
		}

		if (eType == 0)
		{
			continue;
		}

		if (eType >= NAT_ABS_ALL && eType < NAT_MAX_NUM)
		{
			SetNormalAttrByData(eType, fValue, true);
		}
	}
}

void BeCarry::InitAttrFromSkill(int iSkillTypeID, int iSkillLevel)
{
	const SkillTable* pkSkillRes = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID);
	if (!pkSkillRes)
	{
		return;
	}
	if (iSkillLevel <= 0 || iSkillLevel > pkSkillRes->iSkillMaxLevel)
	{
		iSkillLevel = 1;
	}

	InitNormalAttr(pkSkillRes, iSkillLevel, iSkillTypeID);

	TePtParam kParam;
	kParam.SetParam(BTP_pkCarray, this);
	kParam.SetParam(BTP_iSkillTypeID, iSkillTypeID);
	kParam.SetParam(BTP_iSkillLevel, iSkillLevel);
	kParam.SetParam(BTP_pkTrgUnit, pkAttachUnit);

	gTrgMgr.FireTrigger(BTE_INIT_SKILL_ATTR, kParam);
}
