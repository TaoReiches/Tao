#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include "TW_Task.h"
#include "TW_Pos2.h"
#include "TW_TaskActionWalk.h"

enum class BeAttackToPosState
{
	BAP_MOVE = 0,
	BAP_REFRESH,
	BAP_TRACE,
	BAP_ATTACK,
	BAP_END,
};

class BeTaskActionAttack;
class BeTaskMoveToPos;

class BeTaskAttackToPos : public TwTask
{
public:
	BeTaskAttackToPos();
	~BeTaskAttackToPos();

	void SetTargetPos(const TwPos2& kPos, float fRange = 0.0f);
	virtual BeExeResult Execute(int& iDeltaTime);
	TwMoveResult GetMoveResult(void);
	bool IsCanCancel() const;

protected:
	TwPos2						m_kTarPos;
	float						m_fRange;
	int							m_iRetryTime;
	BeAttackToPosState			m_eState;
	unsigned int				m_dwMoveTime;
	unsigned int				m_dwOutSightTime;
	int							m_iStartTime;
	int							m_iStartAttackTime;
	std::unique_ptr<BeTaskActionAttack> m_pkAttack;
	std::unique_ptr<BeTaskMoveToPos> m_pkMoveToPos;
};
