#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_LogicBase.h"

class BeUnit;

class BeFormulaInfo : public BeMainPtr
{
public:
	BeFormulaInfo();
	~BeFormulaInfo();

	float	GetAttackArmorScale(int eAttack, int eArmor);

	int		GetHeroDeadPersistTime(int iHeroLevel);		
	int		GetReliveHeroMoney(BeUnit* pkUnitRelive);	

	int GetAssistMoney(int iHeroLevel, int iAssistNum);
	int GetAssistExp(int iHeroLevel, int iAssistNum);
	int GetHeroDeathDecMoney(int iHeroLevel);
	int GetHeroKillAwardMoney(int iHeroLevel, int iAssistNum);
	int GetComboKillAwardMoney(int iKills);
	int GetComboKillDecMoney(int iKills);
	int GetRevengeMoney(int iKillNum);
	int GetKillBossAward(void);
	int GetKillYanMoAward(void);

	int GetBuyItemPrice(int iIndex);

	float GetAttackSpeedFromAGI(float fAGI);

	float GetRenJiFightPower(BeUnit* pkUnit);
	float GetRenJiFightPowerMax(BeUnit* pkUnit);

protected:
	void	InitAttackArmorScale(void);

protected:
	float	m_afAttackArmor[9 * 5];
};
