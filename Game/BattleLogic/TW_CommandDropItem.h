#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandExe.h"

class BeDropItemCommand : public BeExeCommand
{
public:
	BeDropItemCommand();
	~BeDropItemCommand() {};

	void SetTargetIDDropItem(int iID, int iItemID);
	void SetTargetPosDropItem(const TePos2& kPos, int iItemID);

	virtual BeExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(BeGiveCmdType eCmdType = BeGiveCmdType::BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;

protected:
	int		m_iTargetID;
	int     m_iItemID;
	TePos2	m_kTargetPos;
};

