#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Task.h"

class BeTaskActionStand : public BeTask
{
protected:
	BeTaskActionStand();
	~BeTaskActionStand() {};

public:
	virtual BeExeResult Execute(int& iDeltaTime);
};
