#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

static	const	float	fNormalCrit = 2.0f;
static	const	float	fSkillCrit = 1.5f;
static	const	int		iHeroMaxLevel = 18;
static	const	int		iMonsterMaxLevel = 999;
static	const	int		iMaxBuyItem = 6;

class	BeFormula
{
public:
	static	void			CheckAttackSpeed(float& fValue);

	static	void			CheckMoveSpeed(float& fValue);

	static	float			GetCritRate(float fValue);

	static	float			GetBaseSkillAdd(float fUpValue, float fHeroLevel);

	static	float			GetAmorForDamage(float fAmor);

	static	int				GetHeroLevelUpNeedExp(int iLevel);
};
