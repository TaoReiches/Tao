#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#define MAX_SELL_ITEMS		12
#define MAX_LEARN_SKILLS	7
#define MAX_MONSTER_POS		12
#define MAX_MISSION_NUMS	100
#define MAX_GAMEACBOSS_NUMS	30
#define LOW2HEIGHTMISRATE	0.25f
#define MAX_TACTIC_POINTS	120
#define MIN_TRG_WATI		20

enum class BeMainState
{
	BMS_NONE = 0,
	BMS_OVER,
	BMS_INITED,
	BMS_PAUSE,
	BMS_RACING,
	BMS_OVERWAIT,
};

enum class BeGenIDType
{
	GIT_ENTITY = 0,
	GIT_EFFECT,
	GIT_MAPITEM,
	GIT_CARRY,
	GIT_DOODAD,
	GIT_VISION,
	GIT_REGION,
	GIT_MAX,
};

enum class BeRaceGameModel
{
	BRGM_NORMAL = 0,
	BRGM_MINE,
	BRGM_RANDOM_HERO,
	BRGM_RANDOM_SKILL,
};

#define VALUE_A    0
#define VALUE_B    1
#define VALUE_C    2
#define VALUE_D    3
#define VALUE_E    4
#define VALUE_F    5
#define VALUE_G    6

enum TwMapActorType
{
	MAT_UNIT = 0,
	MAT_MAPITEM,

	MAT_NUM,
};
