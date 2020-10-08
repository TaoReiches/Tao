#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_MemoryPool.h"

class BeLearnSkillData
{
	DECLARE_POOL_ARRAY(BeLearnSkillData);
public:
	int			iTypeID;
	int			iLearnLevel;
	int			iPos;

	BeLearnSkillData()
	{
		iTypeID = 0;
		iLearnLevel = 0;
		iPos = 0;
	}
};
