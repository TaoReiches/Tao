#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Task.h"

class BeAttackingAttr;

class BeTaskActionAttack : public TwTask
{
public:
	BeTaskActionAttack();
	~BeTaskActionAttack() {};

	void	SetTargetID(int iID, bool bIsOrb = false, int iSkillTypeID = 0, int iSkillLevel = 0, bool bAttackPos = false);
	int		GetTargetID()			const;
	bool	IsAttacking(void)		const;
	bool	IsDamaged(void)			const;
	bool	IsCanCancel(void)		const;
	bool	IsPerDamageEnd(void)	const;
	int		GetAttackCount(void)	const;

	virtual TwExeResult Execute(int& iDeltaTime);

protected:
	bool				m_bAttacking;
	bool				m_bDamaged;
	int					m_iSkillTypeID;
	int					m_iSkillLevel;
	BeAttackingAttr*	m_kAttr;
	bool				m_bCanCancel;
	int					m_iAttackCount;
	bool				m_bBeginAttack;
};

