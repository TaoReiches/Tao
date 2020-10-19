#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

class	BeFormula
{
public:
	static	const	float	fNormalCrit;
	static	const	float	fSkillCrit;
	static	const	int		iHeroMaxLevel;
	static	const	int		iMonsterMaxLevel;
	static	const	int		iMaxBuyItem;

public:
	static	void			CheckAttackSpeed(float& fValue);

	static	void			CheckMoveSpeed(float& fValue);

	static	float			GetCritRate(float fValue);

	static	float			GetBaseSkillAdd(float fUpValue, float fHeroLevel);

	static	float			GetAmorForDamage(float fAmor);

	static	int				GetHeroLevelUpNeedExp(int iLevel);
};
