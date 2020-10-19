#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include <vector>
#include "TW_Pos2.h"

class BeUnit;
class SkillTable;

class BeBufferData
{
public:
	std::shared_ptr<BeUnit>	kUnit;
	int						iUnitID;
	int						iLevel;
	int						iOrgUnitID;
	unsigned int			iStartTime;
	unsigned int			uiRemoveTime;
	unsigned int			iAllLiveTime;
	unsigned int			iCDTime;
	unsigned int			iUpdateTime;
	int						iSkillTypeID;
	const SkillTable*		pkSkillRes;
	int						iConnectItemID;
	int						iData;
	float					fData;
	bool					bLastTimeUpdate;
	TePos2					kPos;

	std::vector<int>		aiData;
	std::vector<float>		afData;
	bool					bHasDel;

	BeBufferData()
	{
		iLevel = 1;
		iUnitID = 0;
		iStartTime = 0;
		uiRemoveTime = -1;
		iSkillTypeID = 0;
		pkSkillRes = nullptr;
		iAllLiveTime = 0;
		iCDTime = 0;
		iUpdateTime = 0;
		bLastTimeUpdate = false;
		iConnectItemID = 0;
		iData = 0;
		fData = 0.0f;
		bHasDel = false;
		iOrgUnitID = 0;
	}
};
