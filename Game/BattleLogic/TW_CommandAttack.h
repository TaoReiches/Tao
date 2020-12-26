#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandExe.h"

class BeAttackCommand : public BeExeCommand
{
public:
	BeAttackCommand();
	~BeAttackCommand() {};

	void SetTargetID(int iID, float fDistance, bool bIsOrb = false, int iSkillTypeID = 0, int iSkillLevel = 0, int iAttackCountLimit = -1);
	void SetTargetPos(const TwPos2& kPos, float fRange = 0.0f);
	void SetTargetItem(int iItemID);

	virtual BeExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(TwGiveCmdType eCmdType = TwGiveCmdType::BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;
	virtual bool CanSkip(void) const;
};
