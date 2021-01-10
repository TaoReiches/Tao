#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Task.h"

class BeTaskActionDeath : public TwTask
{
public:
	BeTaskActionDeath();
	~BeTaskActionDeath() {};

	virtual TwExeResult Execute(int& iDeltaTime);

protected:
	bool m_bInit;
};
