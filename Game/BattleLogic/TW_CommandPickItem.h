#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandExe.h"

class BePickItemCommand : public BeExeCommand
{
public:
	BePickItemCommand();
	~BePickItemCommand() {};

	bool SetItemID(int iItemID);

	virtual TwExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(TwGiveCmdType eCmdType = TwGiveCmdType::BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;

protected:
	int		m_iItemID;
	TwPos2	m_kTargetPos;
};
