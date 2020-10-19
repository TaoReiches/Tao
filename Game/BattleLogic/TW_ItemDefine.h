#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

struct BeItemData
{
	int					iItemTypeID;
	int					iItemProperty;
	int					iOwnerPlay;
	unsigned int		iBuyTime;
	bool				bNeedRefreshBuyTime;
	unsigned int		uiRecycleTime;
	unsigned int		uiRefreshTime;
	int					iPackagePos;
	int					iPileCount;
	int					iUseCount;
	int					iSpecialNums;
	int					iValidTime;
	int					iLastValidTime;
	bool				bValid;
	bool				bCanSell;
	bool				bCanShare;
	int					iData;
	int					iLastOwnerPlay;
	unsigned int		iLastUseTime;
	float				fPosX;
	float				fPosY;
	float				fPosZ;
	float				fTarFace;
	float				fFace;
	bool				bLevelUp;

	BeItemData()
	{
		iItemTypeID = 0;
		iItemProperty = 0;
		iBuyTime = 0;
		bNeedRefreshBuyTime = false;
		uiRecycleTime = -1;
		uiRefreshTime = -1;
		iOwnerPlay = -1;
		iPackagePos = 0;
		iPileCount = 0;
		iUseCount = 0;
		iSpecialNums = 0;
		iValidTime = 0;
		iLastValidTime = 0;
		bValid = true;
		bCanSell = true;
		bCanShare = false;
		iData = 0;
		iLastUseTime = 0;
		iLastOwnerPlay = -1;
		fPosX = 0.0f;
		fPosY = 0.0f;
		fPosZ = 0.0f;
		fTarFace = 0.0f;
		fFace = 0.0f;
		bLevelUp = false;
	}
};
