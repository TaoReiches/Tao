#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include "TW_Task.h"
#include "TW_TaskActionWalk.h"

class BeTaskMoveToPos;

class BeTaskMoveToUnit : public BeTask
{
public:
	BeTaskMoveToUnit();
	~BeTaskMoveToUnit();

	BeMoveResult GetMoveResult(void);
	void SetTargetID(int iID, float fDistance = 0.0f);
	int GetTargetID() const;

	virtual BeExeResult Execute(int& iDeltaTime);

protected:
	int									m_iTargetID;
	std::unique_ptr<BeTaskMoveToPos>	m_pkMoveToPos;
	float								m_fDistance;
};
