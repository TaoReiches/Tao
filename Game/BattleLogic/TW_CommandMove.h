#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandExe.h"

class BeMoveCommand : public BeExeCommand
{
protected:
	BeMoveCommand();
	~BeMoveCommand() {};
public:
	void SetTargetPos(const TePos2& kPos, float fDistance, bool bTurn);
	TePos2 GetTargetPos() const;

	virtual BeExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(BeGiveCmdType eCmdType = BeGiveCmdType::BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;
};
