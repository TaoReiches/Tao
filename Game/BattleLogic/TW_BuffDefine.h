#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include <vector>
#include "TW_AttackAttr.h"
#include "TW_Pos2.h"

class BeUnit;
class SkillTable;
struct BeBufferData
{
	std::shared_ptr<BeUnit>		kUnit;
	int						iUnitID;
	int						iLevel;
	int						iNotInivsCampFlag;

	int						iOrgUnitID;

	int						iStartTime;
	unsigned int			uiRemoveTime;
	int						iAllLiveTime;

	int						iCDTime;
	int						iUpdateTime;

	int						iSkillTypeID;
	const SkillTable*		pkSkillRes;
	int						iConnectItemID;
	bool					bIsHaloBuffer;
	bool					bIsPersistBuffer;
	int						iData;
	float					fData;
	bool					bLastTimeUpdate;
	TePos2					kPos;
	int						iShieldDefendType;

	std::vector<int>			aiData;
	std::vector<float>		afData;

	bool					bHasDel;
	BeBufferData()
	{
		iLevel = 1;
		iNotInivsCampFlag = 0;

		iStartTime = 0;
		uiRemoveTime = -1;
		iSkillTypeID = 0;
		pkSkillRes = NULL;
		iAllLiveTime = 0;
		iCDTime = 0;
		iUpdateTime = 0;
		bIsHaloBuffer = false;
		bIsPersistBuffer = false;
		bLastTimeUpdate = false;
		iConnectItemID = 0;
		iData = 0;
		fData = 0.0f;
		bHasDel = false;
		iOrgUnitID = 0;
		iShieldDefendType = BAT_NORMAL;
	}
};