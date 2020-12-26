#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandExe.h"

class BeMoveCommand : public BeExeCommand
{
public:
	BeMoveCommand();
	~BeMoveCommand() {};

	void SetTargetPos(const TwPos2& kPos, float fDistance, bool bTurn);
	TwPos2 GetTargetPos() const;

	virtual BeExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(TwGiveCmdType eCmdType = TwGiveCmdType::BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;
};
