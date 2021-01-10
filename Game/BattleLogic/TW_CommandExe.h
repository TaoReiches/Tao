#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include "TW_LogicBase.h"
#include "TW_Command.h"
#include "TW_Task.h"

class BeExeCommand : public TwMainPtr, public TwUnitPtr
{
public:
	BeExeCommand();
	virtual ~BeExeCommand();

	TwCommandType		GetType() const;
	BeTaskType			GetTaskType() const;

	virtual BeExeResult Execute(int& iDeltaTime) = 0;
	virtual bool CanInterrupt() const = 0;
	virtual bool CanCancel() const = 0;
	virtual bool CanHungUp(TwGiveCmdType eCmdType = TwGiveCmdType::BCT_IMMEDIATE, bool bNeedHangCurrent = true) const = 0;
	virtual bool CanSkip(void) const;
	void SafeDeleteTask(std::unique_ptr<TwTask>& pkTask);

	void AttachMain(std::shared_ptr<TwMain> pkMain)
	{
		pkAttachMain = pkMain;
		if (m_pkCurTask)
		{
			m_pkCurTask->AttachMain(pkMain);
		}
	}

	void AttachUnit(std::shared_ptr<TwUnit> pkUnit)
	{
		pkAttachUnit = pkUnit;
		if (m_pkCurTask)
		{
			m_pkCurTask->AttachUnit(pkUnit);
		}
	}

protected:
	TwCommandType				m_eCmdType;
	std::unique_ptr<TwTask>		m_pkCurTask;
};
