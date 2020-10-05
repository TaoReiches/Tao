#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

struct BeMainPtr
{
	BeMainPtr(void) :pkAttachMain(nullptr)
	{
	}

	void AttachMain(void* pkMain)
	{
		pkAttachMain = pkMain;
	}

	void* pkAttachMain;
};

struct BeUnitPtr
{
	BeUnitPtr(void) :pkAttachUnit(nullptr)
	{
	}

	void AttachUnit(void* pkUnit)
	{
		pkAttachUnit = pkUnit;
	}

	void* pkAttachUnit;
};
