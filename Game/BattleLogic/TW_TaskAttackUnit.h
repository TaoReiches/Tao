#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include "TW_Task.h"

enum class BeAttackUnitState
{
	BAU_ATTACK = 0,
	BAU_REFRESH,
	BAU_TRACE,
	BAU_END,
};

class BeTaskMoveToUnit;
class BeTaskActionAttack;

class BeTaskAttackUnit : public TwTask
{
public:
	BeTaskAttackUnit();
	~BeTaskAttackUnit();

	void SetTargetID(int iID, float fDistance = 0.0f, bool bIsOrb = false, int iSkillTypeID = 0, int iSkillLevel = 0, int iAttackCountLimit = -1);
	int GetTargetID() const;
	bool IsAttacking() const;
	bool IsDamaged() const;
	bool IsCanCancel() const;
	bool CanSkip(void) const;

	virtual BeExeResult Execute(int& iDeltaTime);

protected:
	bool                m_bIsOrb;
	int					m_iSkillTypeID;
	int					m_iSkillLevel;
	int					m_iTargetUnitID;
	int					m_iMoveTime;
	BeAttackUnitState	m_eState;
	std::unique_ptr<BeTaskMoveToUnit>		m_pkMoveToUnit;
	std::unique_ptr <BeTaskActionAttack>	m_pkActionAttack;
};
