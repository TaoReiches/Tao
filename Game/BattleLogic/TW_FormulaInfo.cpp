/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_FormulaInfo.h"

BeFormulaInfo g_kFormulaInfo;

BeFormulaInfo::BeFormulaInfo()
{
	memset(m_afAttackArmor, 0, sizeof(m_afAttackArmor));

	InitAttackArmorScale();
}

BeFormulaInfo::~BeFormulaInfo()
{
}

void BeFormulaInfo::InitAttackArmorScale(void)
{
}

float BeFormulaInfo::GetAttackArmorScale(int eAttack, int eArmor)
{
	if (eAttack < 0 || eAttack >= 9 || eArmor < 0 || eArmor >= 5)
	{
		return 0.0f;
	}

	return m_afAttackArmor[eAttack * 5 + eArmor];
}

int	BeFormulaInfo::GetHeroDeadPersistTime(int iHeroLevel)
{
	{
		if (iHeroLevel <= 8)
		{
			return (iHeroLevel * 3 + 1) * 1000;
		}
		else
		{
			return (iHeroLevel * 5 - 15) * 1000;
		}
	}
}

int BeFormulaInfo::GetBuyItemPrice(int iIndex)
{
	return 88888;
}

int BeFormulaInfo::GetAssistMoney(int iHeroLevel, int iAssistNum)
{
	{
		return (3 * min(iAssistNum, 4) + 4) * (10 + iHeroLevel) / iAssistNum;
	}
}

int BeFormulaInfo::GetAssistExp(int iHeroLevel, int iAssistNum)
{
	return (10 * min(iAssistNum, 4) + 5) * (2 + iHeroLevel) / iAssistNum;
}

int BeFormulaInfo::GetHeroKillAwardMoney(int iHeroLevel, int iAssistNum)
{
	{
		return 250 + iHeroLevel * 5;
	}
}

int BeFormulaInfo::GetHeroDeathDecMoney(int iHeroLevel)
{
	{
		return iHeroLevel * 30;
	}
}

int BeFormulaInfo::GetComboKillAwardMoney(int iKills)
{
	return (iKills - 2) * 50;
}


int BeFormulaInfo::GetComboKillDecMoney(int iKills)
{
	{
		return (iKills - 2) * 50;
	}
}

int	BeFormulaInfo::GetReliveHeroMoney(BeUnit* pkUnitRelive)
{
	return 100;
}

int BeFormulaInfo::GetRevengeMoney(int iKillNum)
{
	if (iKillNum >= 3)
	{
		return iKillNum * 50;
	}

	return 0;
}

int BeFormulaInfo::GetKillBossAward(void)
{
	int iTime = gTime < 4600 ? gTime : 4600;

	{
		return (int)(10 * (15 + iTime / 1000 / 60 / 2.0f));
	}

	return 0;
}

int BeFormulaInfo::GetKillYanMoAward(void)
{
	int iTime = gTime;

	{
		return 200;
	}
}


float BeFormulaInfo::GetAttackSpeedFromAGI(float fAGI)
{
	float fAttackSpeed = 0.0f;

	return fAttackSpeed;
}

float BeFormulaInfo::GetRenJiFightPower(BeUnit* pkUnit)
{
	if (!pkUnit)
	{
		return 0.0f;
	}

	float fLastValue = pkUnit->GetUD_Float(UDK_FightPower);
	int iLastTime = pkUnit->GetUD_Int(UDK_FightPowerTime);
	if ((int)gTime - iLastTime < 500)
	{
		return fLastValue;
	}

	const std::map<unsigned int, RenJiCustomDataTable*>& rkTable = RenJiCustomDataTableMgr::Get()->GetRenJiCustomDataTableMap();

	float fResult = 0.0f;
	fResult += pkUnit->GetHP() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_HP);
	fResult += pkUnit->GetMP() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_MP);
	fResult += (pkUnit->GetBaseDamage() + pkUnit->GetAddDamage()) * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_ATTACKSPEED);
	fResult += ((float)pkUnit->GetOrgAttackCD() / (float)pkUnit->GetAttackCD() - 1.0f) * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_ATTACKSPEED);
	fResult += pkUnit->GetMagicDamage() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_SKILLDAMAGE);
	fResult += pkUnit->GetArmor() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_ARMOR);
	fResult += pkUnit->GetMagicArmor() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_MAGICRESIST);
	fResult += pkUnit->GetRegenHP() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_HPREGEN);
	fResult += pkUnit->GetRegenMP() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_MPREGEN);
	fResult += pkUnit->GetMoveSpeed() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_MOVESPEED);
	fResult += pkUnit->GetLeech() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_LEECH);
	fResult += (pkUnit->GetMoveSpeed() - pkUnit->GetBaseMoveSpeed()) * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_MOVESPEED);
	fResult += pkUnit->GetDecCDTime() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_DECCD);
	fResult += pkUnit->GetDecArmor() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_DECARMOR);
	fResult += TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_DECMAGICRESIST);

	for (int i = 0; i < iMaxHeroSkillNum; i++)
	{
		BeSkill* pkSKill = pkUnit->GetSkillByUIPos(i);
		if (pkSKill)
		{
			if (pkSKill->GetOperateType() != SKILL_OPERATETYPE_BEIDONG && pkSKill->GetOperateType() != SKILL_OPERATETYPE_KAIGUAN)
			{
				if (!pkSKill->CDComplete(pkUnit) || !pkUnit->UnitCanUseSkill(pkSKill->GetID()))
				{
					continue;
				}
			}

			float fValue = TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_FINALSKILL);
			if (i != 3)
			{
				fValue = TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_NORMALSKILL);
			}

			int iLevel = pkSKill->GetLevel();
			switch (iLevel)
			{
			case 1:
				if (i != 3)
				{
					fValue *= 0.4f;
				}
				else
				{
					fValue *= 0.6f;
				}
				break;
			case 2:
				if (i != 3)
				{
					fValue *= 0.6f;
				}
				else
				{
					fValue *= 1.0f;
				}
				break;
			case 3:
				if (i != 3)
				{
					fValue *= 0.8f;
				}
				break;
			case 4:
				if (i != 3)
				{
					fValue *= 1.0f;
				}
				break;
			default:
				break;
			}

			fResult += fValue;
		}
	}

	float fPercent = 1.0f;
	UnitGroup kAroundGroup;
	gUnitMgr.GetAreaGroup(kAroundGroup, pkUnit->GetPosX(), pkUnit->GetPosY(), 800.0f, pkUnit->GetPlayer(), 0xFFFFFFFF, false);
	for (int i = 0; i < (int)kAroundGroup.Size(); i++)
	{
		BeUnit* pkTemp = kAroundGroup[i];
		if (pkTemp)
		{
			if (pkTemp->GetClass() == UNIT_CLASSTYPE_SOLIDER && pkTemp->GetPlayer() >= MAX_ACTIVEPLAYERS)
			{
				if (pkTemp->GetCamp() == pkUnit->GetCamp())
				{
					fPercent += TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_SELFSOLDIER);
				}
				else
				{
					fPercent += TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_ENEMYSOLDIER);
				}
			}
			else if (pkTemp->IsHero() && pkTemp->GetPlayer() < MAX_ACTIVEPLAYERS && pkTemp->GetPlayer() != pkUnit->GetPlayer())
			{
				if (pkTemp->GetCamp() == pkUnit->GetCamp())
				{
					fPercent += TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_SELFHERO);
				}
				else
				{
					fPercent += TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_ENEMYHERO);
				}
			}
			else if (pkTemp->GetClass() == UNIT_CLASSTYPE_MACHINE)
			{
				if (pkTemp->GetCamp() == pkUnit->GetCamp())
				{
					fPercent += TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_SELFBUILDING);
				}
				else
				{
					fPercent += TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_ENEMYBUILDING);
				}
			}
		}
	}

	fResult *= fPercent;

	pkUnit->SetUD_Float(UDK_FightPower, fResult);
	pkUnit->SetUD_Int(UDK_FightPowerTime, (int)gTime);

	return fResult;
}

float BeFormulaInfo::GetRenJiFightPowerMax(BeUnit* pkUnit)
{
	if (!pkUnit)
	{
		return 0.0f;
	}

	float fLastValue = pkUnit->GetUD_Float(UDK_FightPowerMax);
	int iLastTime = pkUnit->GetUD_Int(UDK_FightPowerMaxTime);
	if ((int)gTime - iLastTime < 500)
	{
		return fLastValue;
	}

	float fResult = 0.0f;
	fResult += pkUnit->GetMaxHP() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_HP);
	fResult += pkUnit->GetMaxMP() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_MP);
	fResult += (pkUnit->GetBaseDamage() + pkUnit->GetAddDamage()) * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_ATTACKSPEED);
	fResult += ((float)pkUnit->GetOrgAttackCD() / (float)pkUnit->GetAttackCD() - 1.0f) * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_ATTACKSPEED);
	fResult += pkUnit->GetMagicDamage() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_SKILLDAMAGE);
	fResult += pkUnit->GetArmor() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_ARMOR);
	fResult += pkUnit->GetMagicArmor() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_MAGICRESIST);
	fResult += pkUnit->GetRegenHP() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_HPREGEN);
	fResult += pkUnit->GetRegenMP() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_MPREGEN);
	fResult += pkUnit->GetMoveSpeed() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_MOVESPEED);
	fResult += pkUnit->GetLeech() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_LEECH);
	fResult += (pkUnit->GetMoveSpeed() - pkUnit->GetBaseMoveSpeed()) * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_MOVESPEED);
	fResult += pkUnit->GetDecCDTime() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_DECCD);;

	fResult += pkUnit->GetDecArmor() * TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_DECARMOR);
	fResult += TableMgr::Get()->GetRenJiCustomValue(RENJICUSTOMDATA_TYPE_DECMAGICRESIST);

	for (int i = 0; i < iMaxHeroSkillNum; i++)
	{
		BeSkill* pkSKill = pkUnit->GetSkillByUIPos(i);
		if (pkSKill)
		{
			float fValue = 20000;
			if (i != 3)
			{
				fValue = 10000;
			}

			int iLevel = pkSKill->GetLevel();
			switch (iLevel)
			{
			case 1:
				if (i != 3)
				{
					fValue *= 0.4f;
				}
				else
				{
					fValue *= 0.6f;
				}
				break;
			case 2:
				if (i != 3)
				{
					fValue *= 0.6f;
				}
				else
				{
					fValue *= 1.0f;
				}
				break;
			case 3:
				if (i != 3)
				{
					fValue *= 0.8f;
				}
				break;
			case 4:
				if (i != 3)
				{
					fValue *= 1.0f;
				}
				break;
			default:
				break;
			}

			fResult += fValue;
		}
	}

	float fPercent = 1.0f;
	UnitGroup kAroundGroup;
	gUnitMgr.GetAreaGroup(kAroundGroup, pkUnit->GetPosX(), pkUnit->GetPosY(), 800.0f, pkUnit->GetPlayer(), 0xFFFFFFFF, false);
	for (int i = 0; i < (int)kAroundGroup.Size(); i++)
	{
		BeUnit* pkTemp = kAroundGroup[i];
		if (pkTemp)
		{
			if (pkTemp->GetClass() == UNIT_CLASSTYPE_SOLIDER && pkTemp->GetPlayer() >= MAX_ACTIVEPLAYERS)
			{
				if (pkTemp->GetCamp() == pkUnit->GetCamp())
				{
					fPercent += 0.02f;
				}
				else
				{
					fPercent -= 0.02f;
				}
			}
			else if (pkTemp->IsHero() && pkTemp->GetPlayer() < MAX_ACTIVEPLAYERS && pkTemp->GetPlayer() != pkUnit->GetPlayer())
			{
				if (pkTemp->GetCamp() == pkUnit->GetCamp())
				{
					fPercent += 0.2f;
				}
				else
				{
					fPercent -= 0.2f;
				}
			}
			else if (pkTemp->GetClass() == UNIT_CLASSTYPE_MACHINE)
			{
				if (pkTemp->GetCamp() == pkUnit->GetCamp())
				{
					fPercent += 0.4f;
				}
				else
				{
					fPercent += 0.4f;
				}
			}
		}
	}

	fResult *= fPercent;

	pkUnit->SetUD_Float(UDK_FightPowerMax, fResult);
	pkUnit->SetUD_Int(UDK_FightPowerMaxTime, (int)gTime);

	return fResult;
}
