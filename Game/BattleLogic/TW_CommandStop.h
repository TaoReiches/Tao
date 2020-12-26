#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandExe.h"

class BeStopCommand : public BeExeCommand
{
public:
	BeStopCommand();
	~BeStopCommand() {};

	bool IsDead();
	void SetDead();
	void SetStopTime(int iTime)
	{
		m_iStopTime = iTime;
	}

	virtual BeExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(TwGiveCmdType eCmdType = TwGiveCmdType::BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;

protected:
	bool	m_bDead;
	int		m_iStopTime;
};
