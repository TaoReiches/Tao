#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>

class TwMain;
class TwUnit;

class BeMainPtr
{
public:
    BeMainPtr(void) :pkAttachMain(nullptr)
    {
    }
    
    void AttachMain(std::shared_ptr<TwMain> pkMain)
    {
    	pkAttachMain = pkMain;
    }
    
    std::shared_ptr<TwMain> pkAttachMain;
};

class BeUnitPtr
{
public:
	BeUnitPtr(void) :pkAttachUnit(nullptr)
	{
	}

	void AttachUnit(std::shared_ptr<TwUnit> pkUnit)
	{
		pkAttachUnit = pkUnit;
	}

	std::shared_ptr<TwUnit> pkAttachUnit;
};
