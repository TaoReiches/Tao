#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include "TW_Task.h"

enum class BeAttackItemState
{
	BAI_ATTACK = 0,
	BAI_TRACE,
	BAI_END,
};

class TwTaskMoveToPos;
class BeTaskActionAttack;

class BeTaskAttackItem : public TwTask
{
public:
	BeTaskAttackItem();
	~BeTaskAttackItem();

	void SetTargetID(int iID, float fDistance = 0.0f);
	int GetTargetID() const;
	bool IsAttacking() const;

	virtual TwExeResult Execute(int& iDeltaTime);

protected:
	std::unique_ptr<TwTaskMoveToPos>		m_pkMoveToPos;
	std::unique_ptr<BeTaskActionAttack>		m_pkActionAttack;

	BeAttackItemState	m_eState;
	int					m_iItemID;
};
