#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_MemoryPool.h"
#include "TW_Pos2.h"
#include "TW_TaskDefine.h"
#include "TW_AttackAttr.h"

#define MAX_BIRTHTIME 600

class SkillTable;

class BeTaskActionStand : public BeTask
{
	DECLARE_POOL(BeTaskActionStand);

protected:
	BeTaskActionStand();
	~BeTaskActionStand() {};

public:
	virtual BeExeResult Execute(int& iDeltaTime);
};

class BeTaskActionWalk : public BeTask
{
	DECLARE_POOL(BeTaskActionWalk);

protected:
	BeTaskActionWalk();
	~BeTaskActionWalk();

public:
	void SetTargetPos(const TePos2& kTargetPos, bool bTurn = false);
	bool IsBlocked(void);
	virtual BeExeResult Execute(int& iDeltaTime);

protected:
	bool	m_bStopAction;
	bool	m_bBlocked;
	//bool	m_bTurn;
	TePos2	m_kTargetPos;
	TePos2	m_kSrcPos;

	int		m_iWalkTime;
};

class BeTaskActionAttack : public BeTask
{
	DECLARE_POOL(BeTaskActionAttack);

protected:
	BeTaskActionAttack();
	~BeTaskActionAttack() {};

public:
	void	SetTargetID(int iID, bool bIsOrb = false, int iSkillTypeID = 0, int iSkillLevel = 0, bool bAttackPos = false);
	void	SetTargetItem(int iItemID);
	int		GetTargetID()			const;
	bool	IsAttacking(void)		const;
	bool	IsDamaged(void)			const;
	bool	IsCanCancel(void)		const;
	bool	IsPerDamageEnd(void)	const;
	int		GetAttackCount(void)	const;

	virtual BeExeResult Execute(int& iDeltaTime);

protected:
	bool			m_bAttacking;
	bool			m_bDamaged;
	int				m_iSkillTypeID;
	int				m_iSkillLevel;
	BeAttackingAttr m_kAttr;

	int				m_iItemID;
	bool			m_bCanCancel;
	int				m_iAttackCount;
	bool			m_bBeginAttack;
};

class BeItem;
class BeTaskActionSpell : public BeTask
{
	DECLARE_POOL(BeTaskActionSpell);

protected:
	BeTaskActionSpell();
	~BeTaskActionSpell();

public:
	bool SpellTargetID(int iSkillTypeID, int iSkillLevel, bool bExpendMP, int iTargetID, const TePos2& kPos, int iItemID = 0, int iUsePlayer = -1, int iTargetType = 0);
	bool SpellTargetItem(int iSkillTypeID, int iSkillLevel, bool bExpendMP, const TePos2& kPos, int iItemID = 0, int iOtherID = 0, int iUsePlayer = -1);
	bool SpellTargetPos(int iSkillTypeID, int iSkillLevel, bool bExpendMP, const TePos2& kPos, const TePos2& kDirPos, int iItemID = 0, int iUsePlayer = -1);

	bool CanSkip(void) const;
	BeSpellPhase GetPhase(void);
	virtual BeExeResult Execute(int& iDeltaTime);

protected:
	bool CanUseSpell(void);
	bool WillEffectBreak(void);
	bool UseUpItem(void);
	void  OperateItem(BeItem* pkItem);
	bool UseUpSkill(void);

	void OnSpellPrepare(void);
	void OnSpellCast(void);
	void OnSpellEffect(void);
	void OnSpellFinish(void);

protected:
	int 			m_iSkillTypeID;
	int				m_iItemID;
	int				m_iItemOwnerPlayer;
	int				m_iSkillLevel;
	bool			m_bExpendMP;
	int				m_iTargetID;
	int				m_iTargetType;

	int             m_iOtherID;
	int				m_iItemTypeID;
	int				m_iUsePlayer;
	TePos2			m_kTargetPos;
	TePos2			m_kDirectPos;

	int				m_iManaSpend;

	BeSpellPhase	m_ePhase;
	int				m_iActionTime;
	int				m_iPrepareTime;
	int				m_iCastTime;
	int				m_iEffectTime;
	int				m_iPersitTime;
	int				m_iPersitDeltaTime;
	int				m_iShakesTime;
	bool			m_bSkipEffectTime;

	const SkillTable* m_pkSkillRes;

public:
	const SkillTable* GetSkillRes() const
	{
		return m_pkSkillRes;
	}
};

class BeTaskActionDeath : public BeTask
{
	DECLARE_POOL(BeTaskActionDeath);

protected:
	BeTaskActionDeath();
	~BeTaskActionDeath() {};

public:
	virtual BeExeResult Execute(int& iDeltaTime);

protected:
	bool m_bInit;
};
