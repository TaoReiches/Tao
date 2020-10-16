#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include "TW_LogicBase.h"
#include "TW_Command.h"
#include "TW_Task.h"

class BeExeCommand : public BeMainPtr, public BeUnitPtr
{
public:
	BeExeCommand();
	virtual ~BeExeCommand();

	BeCommandType		GetType() const;
	BeTaskType			GetTaskType() const;

	virtual BeExeResult Execute(int& iDeltaTime) = 0;
	virtual bool CanInterrupt() const = 0;
	virtual bool CanCancel() const = 0;
	virtual bool CanHungUp(BeGiveCmdType eCmdType = BeGiveCmdType::BCT_IMMEDIATE, bool bNeedHangCurrent = true) const = 0;
	virtual bool CanSkip(void) const;
	void SafeDeleteTask(std::unique_ptr<BeTask>& pkTask);

	void AttachMain(void* pkMain)
	{
		pkAttachMain = pkMain;
		if (m_pkCurTask)
		{
			m_pkCurTask->AttachMain(pkMain);
		}
	}

	void AttachUnit(void* pkUnit)
	{
		pkAttachUnit = pkUnit;
		if (m_pkCurTask)
		{
			m_pkCurTask->AttachUnit(pkUnit);
		}
	}

protected:
	BeCommandType				m_eCmdType;
	std::unique_ptr<BeTask>		m_pkCurTask;
};
