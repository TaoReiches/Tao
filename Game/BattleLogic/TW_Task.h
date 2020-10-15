#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_LogicBase.h"
#include "TW_TaskDefine.h"

class BeUnit;

class BeTask : public BeMainPtr, public BeUnitPtr
{
protected:
	BeTask(void);
	virtual ~BeTask(void);

public:
	BeTaskType GetType();
	virtual BeExeResult Execute(int& iDeltaTime) = 0;

protected:
	BeTaskType	m_eType;
	int m_iTaskTime;
};
