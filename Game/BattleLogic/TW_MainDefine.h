#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#define SIDEA_SOILDER (gMain.GetSoldierPlayerByGroup(SRPC_CAMPA))
#define SIDEB_SOILDER (gMain.GetSoldierPlayerByGroup(SRPC_CAMPB))
#define SIDEC_SOILDER (gMain.GetSoldierPlayerByGroup(SRPC_CAMPC))
#define SIDED_SOILDER (gMain.GetSoldierPlayerByGroup(SRPC_CAMPD))
#define NEUTRAL_PASSIVE (gMain.GetSoldierPlayerByGroup(SRPC_NEUTRAL_PASSIVE))
#define NEUTRAL_ENEMY (gMain.GetSoldierPlayerByGroup(SRPC_NEUTRAL_ENEMY))
#define NEUTRAL_ALLY (gMain.GetSoldierPlayerByGroup(SRPC_NEUTRAL_ALLY))

#define	MAX_PLAYERS			(MAX_ACTIVEPLAYERS+6)
#define MAX_GROUPS			6
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

enum BeMapID
{
	BMID_DALUANDOU = 'M004',
	BMID_SINGLE_MATCH = 'M002',
	BMID_ZHANCHANG_MATCH_1 = 'M200',
	BMID_ZHANCHANG_MATCH_2 = 'M201',
	BMID_ZHANCHANG_MATCH_3 = 'M202',
};

enum BeMainState
{
	BMS_NONE = 0,
	BMS_OVER,
	BMS_INITED,
	BMS_PAUSE,
	BMS_RACING,
	BMS_OVERWAIT,
};

enum BeMain8V8State
{
	BMS_8V8_NONE = 0,
	BMS_8V8_FORBIDJOIN_1 = 1,
	BMS_8V8_FORBIDJOIN_2 = 2,
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

enum BePlayerType
{
	BPT_NULL = 0,
	BPT_DIS_PLAYER,
	BPT_ROBOT,
	BPT_PLAYER,
};

enum BePlayerCharacter
{
	BPC_Radical = 0,
	BPC_BRAVE,
	BPC_POSITIVE,
	BPC_NEGATIVE,
	BPC_CONSERVATION,
	BPC_WEAK,
	BPC_NULL,
};

enum BeAwardNoticeType
{
	BANT_NONE = 0,
	BANT_GOT,
	BANT_QUEST,
};

struct BeZItemInfo
{
	BeZItemInfo() {
		iUniqueID = 0;
		iItemType = 0;
		iStrengLV = 0;
		memset(iAttrType, 0, sizeof(iAttrType));
		memset(fAttrValue, 0, sizeof(fAttrValue));
		memset(iJewelIds, 0, sizeof(iJewelIds));
	}

	int iUniqueID;
	int	iItemType;
	int iStrengLV;
	int iAttrType[MAX_EQUIP_ATTR_NUM];
	float fAttrValue[MAX_EQUIP_ATTR_NUM];
	int iJewelIds[MAX_EQUIP_JEWEL_NUM];
};

struct BeRevengeInfo
{
	BeRevengeInfo()
	{
		Clear();
	}

	void Clear()
	{
		iKillPlayer = -1;
		iKillNums = 0;
	}

	int		iKillPlayer;
	int		iKillNums;
};

struct BePlayerInfo
{
	BePlayerInfo()
	{
		Clear();
	}

	void Clear()
	{
		iDeathBeginTime = 0;
		eType = BPT_NULL;
		iID = 0;
		memset(acName, 0, sizeof(acName));
		iTypeID = 0;
		iCamp = 0;
		iGold = 0;
		iAllGold = 0;
		iCalCulateGold = 0;
		iKillCount = 0;
		iBeKilledCount = 0;
		iAssistCount = 0;
		iReLiveTime = 0;
		iTownNum = 0;
		iTownAssistNum = 0;
		iImTownNum = 0;
		iCampNum = 0;
		iCampAssistNum = 0;
		iImCampNum = 0;
		iBuildingNum = 0;
		iImBuildingNum = 0;
		iKillSoildiers = 0;
		iImKillSoildiers = 0;
		iKillMonsterNum = 0;
		iComboKills = 0;
		iComboMaxKills = 0;
		iSpyNums = 0;
		iKillBase = 0;
		iGameOverFlag = 0;
		memset(acTitle, 0, sizeof(acTitle));
		iGameTime = 0;
		iTotalCommandNums = 0;

		iHeroUnitID = 0;
		iHeroID = 0;

		iLastKillHeroTime = 0;
		iContinueKill = 0;
		iGetFirstBlood = 0;
		iBuyLiveCount = 0;
		iMVPTitleFlag = 0;
		iBuyItemCount = 0;
		iComboDeaths = 0;
		iSoldiersGoldByNum = 0;

		memset(auiAttackedTime, 0, sizeof(auiAttackedTime));
		memset(auiAddBufferTime, 0, sizeof(auiAddBufferTime));
		memset(auiHelpTime, 0, sizeof(auiHelpTime));

		iAddAssistGold = 0;
		fAddFarmGoldPer = 0.0f;
		fAddExpPer = 0.0f;

		for (int i = 0; i < MAX_PLAYERS; ++i)
		{
			akRevengeInfo[i].Clear();
		}

		iMaxContinueKill = 0;
		eCharacter = BPC_NULL;

		iDeathBeginTime = 0;
		iTotalDamage = 0;
		iTotalHeroDamage = 0;
		iTotalBeDamaged = 0;
		iZhiLiao = 0;
		iTotalBuildingDamage = 0;
		iDeathMaxHp = 0;
		iUseEyeTimes = 0;
		iEndComboKills = 0;
		iEndComboMaxKills = 0;
		iNoPlayCheckTime = 0;
		iCheckCount = 0;
		iSurrenderTime = 0;
		iKillBigBossNum = 0;
		iKillSmallBossNum = 0;

		fEquipMaxHp = 0.0f;
		fEquipMaxMp = 0.0f;
		fEquipRegenHp = 0.0f;
		fEquipRegenMp = 0.0f;
		fEquipDamage = 0.0f;
		fEquipAttackSpeed = 0.0f;
		fEquipMagicDamage = 0.0f;
		fEquipMagicArmor = 0.0f;
		fEquipArmor = 0.0f;
		fEquipDecCD = 0.0f;
		fEquipCritical = 0.0f;
		iEquipSkill[0] = 0;
		iEquipSkill[1] = 0;
		iEquipSkill[2] = 0;
		fLevelAddArmor = 0.0f;
		fLevelAddMagicDamage = 0.0f;
		fLevelAddMagicArmor = 0.0f;
		fPenetrateMagicArmor = 0.0f;
		fPenetrateArmor = 0.0f;
		fPersentBaseMaxHp = 0.0f;
		fBaseMoveSpeed = 0.0f;

		iZItenCount = 0;
		memset(akZItemsInfo, 0, sizeof(akZItemsInfo));
		iFeatsValue = 0;
		iMoraleValue = 0;
		iTeamScore = 0;
		iKillEternalGuard = 0;
		iKillNeutralMercenaries = 0;
		fPersentLifeSteal = 0.0f;
		fPersentPenetrateMa = 0.0f;

		uiKillerEffectID = 0;
		uiEmojiEffectID = 0;
	}

	BePlayerType	eType;
	BePlayerCharacter eCharacter;
	int				iID;
	char			acName[64];
	int				iTypeID;

	int				iCamp;
	int				iGold;
	int				iAllGold;
	int				iCalCulateGold;
	int				iKillCount;
	int				iBeKilledCount;
	int				iAssistCount;
	int				iReLiveTime;
	int				iTownNum;
	int				iTownAssistNum;
	int				iImTownNum;
	int				iCampNum;
	int				iCampAssistNum;
	int				iImCampNum;
	int				iBuildingNum;
	int				iImBuildingNum;
	int				iKillSoildiers;
	int				iImKillSoildiers;
	int				iKillMonsterNum;
	int				iKillBigBossNum;
	int				iKillSmallBossNum;
	int				iComboKills;
	int				iEndComboKills;
	int				iComboMaxKills;
	int				iComboDeaths;
	int				iSoldiersGoldByNum;
	int				iEndComboMaxKills;
	int				iSpyNums;
	int				iKillBase;
	int				iGameOverFlag;
	char		    acTitle[32];
	int				iGameTime;
	unsigned int	iTotalCommandNums;
	int				iBuyItemCount;

	int				iHeroUnitID;
	int				iHeroID;
	unsigned int		uiHeroProficiency;
	int				iLastKillHeroTime;
	int				iContinueKill;
	int				iMaxContinueKill;
	int 			iGetFirstBlood;
	int				iBuyLiveCount;

	int				iTotalDamage;
	int				iTotalHeroDamage;
	int				iTotalBuildingDamage;
	int				iTotalBeDamaged;
	int				iZhiLiao;
	int				iDeathMaxHp;
	int				iUseEyeTimes;

	int				iMVPTitleFlag;
	int				iMVPValue;
	int				iNoPlayCheckTime;
	int				iCheckCount;

	int				iAddAssistGold;
	float			fAddFarmGoldPer;
	float			fAddExpPer;

	unsigned int	auiAttackedTime[MAX_PLAYERS];
	unsigned int	auiHelpTime[MAX_PLAYERS];
	BeRevengeInfo	akRevengeInfo[MAX_PLAYERS];
	unsigned int	auiAddBufferTime[MAX_PLAYERS];

	int             iDeathBeginTime;
	int				iSurrenderTime;

	int				iFeatsValue;
	int				iMoraleValue;
	int				iTeamScore;
	int				iKillEternalGuard;
	int				iKillNeutralMercenaries;

	unsigned int	uiKillerEffectID;
	unsigned int	uiEmojiEffectID;

	float				fEquipMaxHp;
	float				fEquipMaxMp;
	float				fEquipRegenHp;
	float				fEquipRegenMp;
	float				fEquipDamage;
	float				fEquipAttackSpeed;
	float				fEquipMagicDamage;
	float				fEquipMagicArmor;
	float				fEquipArmor;
	float				fEquipDecCD;
	float				fEquipCritical;
	int					iEquipSkill[3];
	float				fLevelAddArmor;
	float				fLevelAddMagicDamage;
	float				fLevelAddMagicArmor;
	float				fPenetrateMagicArmor;
	float				fPenetrateArmor;
	float				fPersentBaseMaxHp;
	float				fBaseMoveSpeed;
	float				fPersentLifeSteal;
	float				fPersentPenetrateMa;

	int					iZItenCount;
	BeZItemInfo			akZItemsInfo[MAX_ZITEM_NUM];
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
	std::vector<BePos2> kVecPathPos[MAX_QUEST_KEY];
};

#define VALUE_A    0
#define VALUE_B    1
#define VALUE_C    2
#define VALUE_D    3
#define VALUE_E    4
#define VALUE_F    5
#define VALUE_G    6

#define VALIDFUNC(val) ((val >= 0 && (val < sizeof(afParseFunc) / sizeof(ParseFunc)) && afParseFunc[val])?true:false)

struct ShareUnitData
{
	ShareUnitData()
	{
		memset(aiSharePlayer, -1, sizeof(int) * MAX_ACTIVEPLAYERS);
		iPlayer = -1;
		iTypeID = 0;
	}

	int aiSharePlayer[MAX_ACTIVEPLAYERS];
	int iPlayer;
	int iTypeID;
};

enum MapActorType
{
	MAT_UNIT = 0,
	MAT_MAPITEM,

	MAT_NUM,
};
