#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Task.h"

class BeTaskActionDeath : public BeTask
{
public:
	BeTaskActionDeath();
	~BeTaskActionDeath() {};

	virtual BeExeResult Execute(int& iDeltaTime);

protected:
	bool m_bInit;
};
