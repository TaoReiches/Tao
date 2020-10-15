#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Task.h"

enum class BeAttackItemState
{
	BAI_ATTACK = 0,
	BAI_TRACE,
	BAI_END,
};

class BeTaskMoveToPos;
class BeTaskActionAttack;

class BeTaskAttackItem : public BeTask
{
public:
	BeTaskAttackItem();
	~BeTaskAttackItem();

	void SetTargetID(int iID, float fDistance = 0.0f);
	int GetTargetID() const;
	bool IsAttacking() const;

	virtual BeExeResult Execute(int& iDeltaTime);

protected:
	BeTaskMoveToPos* m_pkMoveToPos;
	BeTaskActionAttack* m_pkActionAttack;

	BeAttackItemState	m_eState;
	int					m_iItemID;
};
