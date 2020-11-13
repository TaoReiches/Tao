#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <cstring>
#include "TW_Pos2.h"

enum class BeGiveCmdType
{
	BCT_NONE = 0,

	BCT_SYSTEM_SHIFT,
	BCT_PLAYER_SHIFT,
	BCT_PLAYER_REDUCE,
	BCT_PLAYER_CMD,
	BCT_PLAYER_INSERT,
	BCT_PLAYER_ACTION,
	BCT_PLAYER_HUNG,
	BCT_SKILL_ADDON,
	BCT_DIZZY_INTERRUPT,
	BCT_DEATH,
	BCT_SYSTEM_CLEAR,

	BCT_IMMEDIATE = BCT_PLAYER_ACTION,
	BCT_NEXT_EXE = BCT_PLAYER_HUNG,
	BCT_INTERRUPT_CURRENT = BCT_DIZZY_INTERRUPT,
	BCT_HUNG_CURRENT = BCT_PLAYER_INSERT,
	BCT_PUSH_BACK = BCT_PLAYER_SHIFT,
};

enum class BeCommandType
{
	BCT_STOP = 0,
	BCT_HOLD,
	BCT_MOVE,
	BCT_PATROL,
	BCT_ATTACK,
	BCT_SPELL,
	BCT_MOVE_DIRECT,

	BCT_USE_ITEM,
	BCT_DROP_ITEM,
	BCT_PICK_ITEM,

	BCT_NUMS,
};

enum class BeCommandTargetType
{
	BCTT_NONE = 0,
	BCTT_MAP = 1,
	BCTT_UNIT = 2,
	BCTT_MAPITEM,

	BCTT_EFFECT,
	BCTT_CMD,
};

class BeCommand
{
public:
	BeCommand(BeCommandType cmd = BeCommandType::BCT_STOP, int unit = 0,
		float x = 0.0f, float y = 0.0f, int data = 0,
		bool bShiftDelete = false, int data2 = 0,
		BeCommandTargetType eTType = BeCommandTargetType::BCTT_UNIT,
		bool bForceAttackOnceIn = false,
		float fDirX = 0.0f, float fDirY = 0.0f)
	{
		eCmdType = cmd;
		iUnitID = unit;
		kPos.fX = x;
		kPos.fY = y;
		iData = data;
		iData2 = data2;
		bForceAttackOnce = bForceAttackOnceIn;
		bShiftCanDelete = bShiftDelete;
		eTargetType = eTType;
		eGiveType = BeGiveCmdType::BCT_NONE;
		bForceExecute = false;
		kDirPos.fX = fDirX;
		kDirPos.fY = fDirY;
	}

	BeCommandType	eCmdType;
	int				iUnitID;
	TwPos2			kPos;
	TwPos2			kDirPos;
	int				iData;
	int				iData2;
	bool			bForceAttackOnce;
	bool			bShiftCanDelete;
	BeCommandTargetType		eTargetType;
	BeGiveCmdType			eGiveType;
	bool			bForceExecute;
};
