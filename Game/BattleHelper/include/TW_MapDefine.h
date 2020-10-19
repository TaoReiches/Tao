#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

enum TeGridFlag
{
	TGF_TERRAIN = (1 << 0),
	TGF_DOODAD = (1 << 1),
	TGF_UNIT = (1 << 2),
	TGF_TEMP = (1 << 3),
	TGF_SKILL = (1 << 4),
	TGF_GUTDOODAD = (1 << 5),

	TGF_SOLIDER = (1 << 6),
	TGF_SOLIDER_COLLION = (1 << 7),

	TGF_FIXED_OTS = (TGF_TERRAIN | TGF_DOODAD | TGF_SKILL | TGF_GUTDOODAD),
	TGF_UNIT_OTS = (TGF_UNIT | TGF_TEMP),
};

enum class TeFindResult
{
	TFR_NONE = 0,
	TFR_NOT_ARRIVE,
	TFR_DIRECT,
	TFR_ARRIVE,
	TFR_ARRIVED,
};
