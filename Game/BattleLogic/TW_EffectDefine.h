#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Command.h"

enum BeEffectFlag
{
	BEF_REMOVE = (1 << 0),
	BEF_CANTSHOW = (1 << 1),

	BEF_IGNORE_MAGIC_IMMUNE = (1 << 2),
	BEF_IGNORE_INVISIBLE = (1 << 3),
	BEF_IGNORE_INVINCIBLE = (1 << 4),
	BEF_ATTRMISSLE = (1 << 5),
	BEF_IGNORE_WITHSTAND = (1 << 6),
	BEF_REMOVE_WHENUNITDEAD = (1 << 7),
	BEF_IN_MAP_SHOW_MOVE = (1 << 8),
	BEF_IN_MAP_SHOW_CYCLE = (1 << 9),

	BEF_CANT_FORBID = (1 << 10),
	BEF_MUST_CREATE = (1 << 11),

	BEF_NODEATHACTION = (1 << 12),
	BEF_NOT_SETPOS = (1 << 13),
	BEF_VISION = (1 << 14),
	BEF_SHOW_IN_WARFOG = (1 << 15),
	BEF_FLY_FUST = (1 << 16),
	BEF_NOMODELFILE = (1 << 17),
	BEF_IGNORE_PHYSIC_IMMUNE = (1 << 18),
	BEF_CYC = (1 << 19),
};

enum BeEffectTargetType
{
	BET_POSITION = BeCommandTargetType::BCTT_MAP,
	BET_UNIT = BeCommandTargetType::BCTT_UNIT,
	BET_ITEM = BeCommandTargetType::BCTT_MAPITEM,
	BET_EFFECT = BeCommandTargetType::BCTT_EFFECT,
};

enum BeEffectColorType
{
	FONT_COLOR_GREEN = 0xFF00FF00,
	FONT_COLOR_GREEN_MID = 0xFF58D03B,
	FONT_COLOR_GREEN_LOW = 0xFF80FF80,
	FONT_COLOR_PURE_RED = 0xFFFF0000,
	FONT_COLOR_RED_LOW = 0xFF8E48E5,
	FONT_COLOR_RED_MID = 0xFFFF8080,
	FONT_COLOR_RED_HIGH = 0xFFFF8C00,
	FONT_COLOR_BLUE_LOW = 0xFF8080FF,
	FONT_COLOR_GRAY = 0xFF808080,
	FONT_COLOR_GRAY_HIGH = 0xFFC0C0C0,
};

enum BeEffectCurveStyle
{
	BECS_LEFTBIG = 0,
	BECS_RIGHTBIG,
	BECS_MIDBIG,
	BECS_WAVE,
};

enum BeEffectCurveType
{
	BECT_CURVELEFT = 0,
	BECT_CURVERIGHT,
	BECT_CURVERMID,
	BECT_CURVESLOWLEFT,
	BECT_CURVESLOWRIGHT,
};

enum BeEffeAttachType
{
	BATTAT_NORMAL = 0,
	BATTAT_WING,
	BATTAT_L_WEAPON,
	BATTAT_R_WEAPON,
	BATTAT_RIDER,
};
