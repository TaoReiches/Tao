#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

struct BeSkillData
{
	int		iCurLevel;
	bool	bIsActive;
	int		iLastTrgTime;
	int		iUIShowPos;
	BeSkillData()
	{
		iCurLevel = 0;
		bIsActive = false;
		iLastTrgTime = 0;
		iUIShowPos = -1;
	}
};
