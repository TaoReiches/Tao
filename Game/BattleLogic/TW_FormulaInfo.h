#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_LogicBase.h"

class TwUnit;

class BeFormulaInfo : public TwMainPtr
{
public:
	BeFormulaInfo();
	~BeFormulaInfo();

	float	GetAttackArmorScale(int eAttack, int eArmor);

	int		GetHeroDeadPersistTime(int iHeroLevel);
	int		GetReliveHeroMoney(std::shared_ptr<TwUnit> pkUnitRelive);

	int		GetAssistMoney(int iHeroLevel, int iAssistNum);
	int		GetAssistExp(int iHeroLevel, int iAssistNum);
	int		GetHeroDeathDecMoney(int iHeroLevel);
	int		GetHeroKillAwardMoney(int iHeroLevel, int iAssistNum);
	int		GetComboKillAwardMoney(int iKills);
	int		GetComboKillDecMoney(int iKills);
	int		GetRevengeMoney(int iKillNum);
	int		GetBuyItemPrice(int iIndex);
	float	GetAttackSpeedFromAGI(float fAGI);

protected:
	void	InitAttackArmorScale(void);

protected:
	float	m_afAttackArmor[9 * 5];
};
