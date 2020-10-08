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

#define MAX_QUEST_KEY		5
#define MAX_RANDOMSKILL_TIMES 5
#define ITEM_MAX_SKILL		6
#define MAX_CONTENT_DATA	7

#define SSD_PRE_UPDATE_FRAME	50
#define SERVERMODE_MAXPLAYER	20
#define MAX_ZITEM_NUM	48

enum BeMainState
{
	BMS_NONE = 0,
	BMS_OVER,
	BMS_INITED,
	BMS_PAUSE,
	BMS_RACING,
	BMS_OVERWAIT,
};

enum BeGenIDType
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

enum BeAwardNoticeType
{
	BANT_NONE = 0,
	BANT_GOT,
	BANT_QUEST,
};

enum BeQuestStatus
{
	BQS_NULL = 0,
	BQS_ACCEPT,
	BQS_CANNT_FINISH,
	BQS_CAN_FINISH,
	BQS_PAUSE,
	BQS_FINISH,
	BQS_FAILED,
};

enum BeQuestType
{
	BQT_NULL = 0,
	BQT_ITEM,
	BQT_MONSTER,
	BQT_ALIVE,
	BQT_TIME_OPTION,
	BQT_TIME_LIMIT,
};

enum BeQuestAwardType
{
	BQAT_ITEM = 0,
	BQAT_GOLD,
	BQAT_EXP,
};

enum BeRaceGameModel
{
	BRGM_NORMAL = 0,
	BRGM_MINE,
	BRGM_RANDOM_HERO,
	BRGM_RANDOM_SKILL,
};

struct BeQuestInfo
{
	BeQuestInfo()
	{
		iMapID = 0;
		iStatus = 0;
		iNPC = 0;
		iType = 0;
		iNum = 0;
		iNeedNum = 0;
		iTime = 0;
		iPauseTime = 0;
		iMissionRate = 0;
		iMissionMaxPoint = 0;
		iMissionPoint = 0;
		memset(acMissionName, 0, sizeof(acMissionName));
		memset(acName, 0, sizeof(acName));
		memset(acContent, 0, sizeof(acContent));
		memset(acKeyWord, 0, sizeof(char) * MAX_QUEST_KEY * 32);
	}
	int iMapID;
	int iStatus;
	int iNPC;
	int iType;
	int iNum;
	int iNeedNum;
	int iTime;
	int iPauseTime;
	int iMissionRate;
	int iMissionMaxPoint;
	int iMissionPoint;

	char acMissionName[32];
	char acName[64];
	char acContent[512];
	char acKeyWord[MAX_QUEST_KEY][32];
};

#define VALUE_A    0
#define VALUE_B    1
#define VALUE_C    2
#define VALUE_D    3
#define VALUE_E    4
#define VALUE_F    5
#define VALUE_G    6

#define VALIDFUNC(val) ((val >= 0 && (val < sizeof(afParseFunc) / sizeof(ParseFunc)) && afParseFunc[val])?true:false)

enum MapActorType
{
	MAT_UNIT = 0,
	MAT_MAPITEM,

	MAT_NUM,
};
