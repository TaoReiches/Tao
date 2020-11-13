#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include "TW_Task.h"
#include <TW_Pos2.h>
#include "TW_TaskActionWalk.h"
#include <TW_MapDefine.h>

enum class BeMoveRetryState
{
	BMRS_DITECT = 0,
	BMRS_MIDDLE,
	BMRS_FINNAL,
	BMRS_CHECK,
};

class BeTaskMoveToPos : public BeTask
{
public:
	BeTaskMoveToPos();
	~BeTaskMoveToPos();

	BeMoveResult GetMoveResult(void) const;
	void SetMoveResult(BeMoveResult eResult);
	void SetTargetPos(const TwPos2& kPos, float fDistance = 0.0f, bool bTurn = false);
	TwPos2 GetTargetPos() const;

	virtual BeExeResult Execute(int& iDeltaTime);

protected:
	BeMoveState			m_eMoveState;
	int					m_iStandTime;
	int					m_iWalkBlockTime;
	int 				m_iRetryTime;

	BeMoveResult		m_eState;
	BeMoveRetryState	m_eRetryState;
	TwFindResult		m_eFindPathRet;

	TwPos2				m_kTarPos;
	TwPos2				m_kMiddlePos;
	TwPos2				m_kDirectPos;
	bool				m_bSoliderCheck;
	float				m_fDistance;

	std::unique_ptr<BeTaskActionWalk>	m_pkWalk;
};
