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

	virtual BeExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(BeGiveCmdType eCmdType = BeGiveCmdType::BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;

protected:
	int		m_iItemID;
	TePos2	m_kTargetPos;
};
