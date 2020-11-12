#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include "TW_Task.h"
#include "TW_Pos2.h"

enum class BeSpellPhase
{
	BSP_INIT = 0,
	BSP_PREPARE,
	BSP_CAST,
	BSP_EFFECT,
	BSP_SHAKES,
	BSP_FINISH,
	BSP_END,
};

class BeItem;
class SkillTable;

class BeTaskActionSpell : public BeTask
{
public:
	BeTaskActionSpell();
	~BeTaskActionSpell();

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

	std::shared_ptr<const SkillTable> m_pkSkillRes;

public:
	inline const std::shared_ptr<const SkillTable>& GetSkillRes() const
	{
		return m_pkSkillRes;
	}
};
