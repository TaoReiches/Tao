/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <algorithm>
#include "TW_FormulaInfo.h"

BeFormulaInfo g_kFormulaInfo;

BeFormulaInfo::BeFormulaInfo()
{
	std::memset(m_afAttackArmor, 0, sizeof(m_afAttackArmor));

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
	return (3 * std::min(iAssistNum, 4) + 4) * (10 + iHeroLevel) / iAssistNum;
}

int BeFormulaInfo::GetAssistExp(int iHeroLevel, int iAssistNum)
{
	return (10 * std::min(iAssistNum, 4) + 5) * (2 + iHeroLevel) / iAssistNum;
}

int BeFormulaInfo::GetHeroKillAwardMoney(int iHeroLevel, int iAssistNum)
{
	return 250 + iHeroLevel * 5;
}

int BeFormulaInfo::GetHeroDeathDecMoney(int iHeroLevel)
{
	return iHeroLevel * 30;
}

int BeFormulaInfo::GetComboKillAwardMoney(int iKills)
{
	return (iKills - 2) * 50;
}

int BeFormulaInfo::GetComboKillDecMoney(int iKills)
{
	return (iKills - 2) * 50;
}

int	BeFormulaInfo::GetReliveHeroMoney(std::shared_ptr<TwUnit> pkUnitRelive)
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

float BeFormulaInfo::GetAttackSpeedFromAGI(float fAGI)
{
	float fAttackSpeed = 0.0f;

	return fAttackSpeed;
}
