#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <string>

enum class BePlayerType
{
	BPT_NULL = 0,
	BPT_DIS_PLAYER,
	BPT_ROBOT,
	BPT_PLAYER,
};

class BePlayerInfo
{
public:
	BePlayerInfo()
	{
		Clear();
	}

	void Clear()
	{
		eType = BePlayerType::BPT_NULL;
		iID = 0;
		acName = "";
		iTypeID = 0;
		iCamp = 0;
		iGold = 0;
		iAllGold = 0;
		iGameTime = 0;
	}

	BePlayerType	eType;
	int				iID;
	std::string		acName;
	int				iTypeID;
	int				iCamp;
	int				iGold;
	int				iAllGold;
	int				iGameTime;
};
