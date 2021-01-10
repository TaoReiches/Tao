#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Task.h"

class BeTaskActionStand : public TwTask
{
public:
	BeTaskActionStand();
	~BeTaskActionStand() {};

	virtual BeExeResult Execute(int& iDeltaTime);
};
