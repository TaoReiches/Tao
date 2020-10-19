/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <math.h>
#include "TW_Formula.h"

const	float	BeFormula::fNormalCrit = 2.0f;
const	float	BeFormula::fSkillCrit = 1.5f;
const	int		BeFormula::iHeroMaxLevel = 18;
const	int		BeFormula::iMonsterMaxLevel = 999;
const	int		BeFormula::iMaxBuyItem = 6;

void	BeFormula::CheckAttackSpeed(float& fValue)
{
	if (fValue > 2.5f)
	{
		fValue = 2.5f;
	}
	if (fValue < 0.4f)
	{
		fValue = 0.4f;
	}
	if (fValue < 1.0f)
	{
		float	fTemp = tanf(fValue) * 0.38f + 0.4f;
		fValue = fTemp;
	}
}

float	BeFormula::GetCritRate(float fValue)
{
	float fTemp = sqrtf(fValue * 0.01f + 0.1849f) - 0.43f;

	return fTemp;
}

float	BeFormula::GetBaseSkillAdd(float fUpValue, float fHeroLevel)
{
	float	fTemp = fUpValue * ceilf(fHeroLevel / 3.0f);

	return fTemp;
}

void	BeFormula::CheckMoveSpeed(float& fValue)
{
	float	fTemp = fValue;
	if (fValue < 175)
	{
		fTemp = fValue * 0.5f + 102;
	}
	else if (fValue > 175 && fValue < 250)
	{
		fTemp = fValue * 0.8f + 50;
	}

	if (fTemp < 100)
	{
		fTemp = 100;
	}

	if (fTemp > 522)
	{
		fTemp = 522;
	}

	fValue = fTemp;
}

float	BeFormula::GetAmorForDamage(float fAmor)
{
	float	fTemp = 0.0f;
	if (fAmor < 0.0f)
	{
		fTemp = 2.0f - (100.0f / (100.0f - fAmor));
	}
	else
	{
		fTemp = 100.0f / (100.0f + fAmor);
	}

	return fTemp;
}

int		BeFormula::GetHeroLevelUpNeedExp(int iLevel)
{
	static	const	int		iHeroLevelExp[] = { 0, 200, 280, 360, 440, 520, 600, 680, 760,
		840, 920, 1000, 1080, 1160, 1240, 1320, 1400, 1480 };
	static	int	iMaxMember = sizeof(iHeroLevelExp) / sizeof(int);

	iLevel -= 1;
	if (iLevel < 0 || iLevel >= iMaxMember)
	{
		return	1560;
	}

	return	iHeroLevelExp[iLevel];
}
