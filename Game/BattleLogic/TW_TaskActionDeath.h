#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Task.h"

class BeTaskActionDeath : public BeTask
{
protected:
	BeTaskActionDeath();
	~BeTaskActionDeath() {};

public:
	virtual BeExeResult Execute(int& iDeltaTime);

protected:
	bool m_bInit;
};
