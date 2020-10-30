#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <string>
#include <memory>
#include "Skill_table.hpp"

enum MoMapID
{
	MAP_WANGTAO_TEST = 1295003698,
};

static	const	int		iMaxHeroSkillNum = 10;
static	const	int		iMaxHeroItem = 6;					
static	const	int		iMaxSkillLevel = 5;					
static	const	int		iLogicFrameTime = 40;				
static	const	unsigned	int		uiAllFilg = 0xFFFFFFFF;	
static	const	int		iMaxSkillContentData = 7;			
static	const	int		iPureCampNum = 2;					

enum BeAttachPos
{
	BAP_Null,				
	BAP_OriginRef,          
	BAP_ChestRef,           
	BAP_RootEffectNode,		
	BAP_OverHeadRef,		
	BAP_HandLeftRef,		
	BAP_HandRightRef,		
	BAP_SpecialRef,			
	BAP_Weapon,				
	BAP_WeaponL,			
	BAP_WeaponR,			
	BAP_Wing,				
	BAP_Ride,				

	BAP_AttachPosNum,
};

static const std::string& GetAttachName(BeAttachPos eAttachPos)
{
	static std::string s_akStr[BAP_AttachPosNum] = {
		"-",
		"Origin_Ref",
		"Chest_Ref",
		"Root_EffectNode",
		"OverHead_Ref",
		"Hand_Left_Ref",
		"Hand_Right_Ref",
		"Special_Ref",
		"Weapon",
		"Weapon_L",
		"Weapon_R",
		"Wing",
		"Ride",
	};

	if (eAttachPos < BAP_AttachPosNum)
	{
		return s_akStr[eAttachPos];
	}

	return s_akStr[0];
}

enum BeActionName
{
	BAN_Null,
	BAN_stand,
	BAN_stand1,
	BAN_walk,
	BAN_attack,
	BAN_death,
	BAN_spell,
	BAN_spell1,
	BAN_spell2,
	BAN_spell3,
	BAN_spell4,
	BAN_spell41,
	BAN_demorph,
	BAN_spell11,
	BAN_spell12,
	BAN_spell13,
	BAN_spell21,
	BAN_spell22,
	BAN_spell23,
	BAN_spell24,
	BAN_spell31,
	BAN_spell32,
	BAN_spell33,
	BAN_spell34,

	BAN_birth,
	BAN_hit,
	BAN_run,

	BAN_SPE_DEATH,	
	BAN_Crit_Attack,
	BAN_Back_Home,		
	BAN_ActionNum,
};

static const std::string& GetActionName(BeActionName eAction)
{
	static std::string s_akStr[BAN_ActionNum] = {
		"-",
		"stand",
		"stand-1",
		"walk",
		"attack",
		"death",
		"spell",
		"spell-1",
		"spell-2",
		"spell-3",
		"spell-4",
		"spell-4_1",
		"demorph",
		"spell-1_1",
		"spell-1_2",
		"spell-1_3",
		"spell-2_1",
		"spell-2_2",
		"spell-2_3",
		"spell-2_4",
		"spell-3_1",
		"spell-3_2",
		"spell-3_3",
		"spell-3_4",

		"birth",
		"hit",
		"run",
	};

	if (eAction < BAN_ActionNum)
	{
		return s_akStr[eAction];
	}

	return s_akStr[0];
}

enum BeShareUnitDataChangeFlag
{
	BSUDCF_TYPEID = 1 << 0,
	BSUDCF_POSX = 1 << 1,
	BSUDCF_FACE = 1 << 2,
	BSUDCF_CURHP = 1 << 3,
	BSUDCF_MAXHP = 1 << 4,
	BSUDCF_CURMP = 1 << 5,
	BSUDCF_MAXMP = 1 << 6,
	BSUDCF_ATTACKCD = 1 << 7,
	BSUDCF_SCALE = 1 << 8,
	BSUDCF_UNITLEVEL = 1 << 9,
	BSUDCF_ACTIONNAME = 1 << 10,
	BSUDCF_MOVESPEED = 1 << 12,
	BSUDCF_GUANKAFLAG = 1 << 13,
	BSUDCF_FLAG = 1 << 14,
	BSUDCF_OTHERFLAG = 1 << 15,
	BSUDCF_CARRYFLAG = 1 << 16,
	BSUDCF_PATHFINDER = 1 << 17,
	BSUDCF_PATHFINDERSUCESS = 1 << 18,
	BSUDCF_STOPPATHFIND = 1 << 19,	
	BSUDCF_REGENHP = 1 << 20,
	BSUDCF_REGENMP = 1 << 21,
	BSUDCF_CAMP = 1 << 22,
	BSUDCF_BEDAMAGED = 1 << 23,
	BSUDCF_HEROLVLUP = 1 << 24,
	BSUDCF_RESET_POX = 1 << 25,
	BSUDCF_EXP = 1 << 27,	
	BSUDCF_SHELD = 1 << 28,	
	BSUDCF_GRASS = 1 << 29,	
	BSUDCF_PLAYER = 1 << 30,	

	BSUDCF_NEW_UNIT = BSUDCF_TYPEID | BSUDCF_POSX | BSUDCF_FACE | BSUDCF_CURHP | BSUDCF_MAXHP | BSUDCF_ATTACKCD | BSUDCF_SCALE
	| BSUDCF_UNITLEVEL | BSUDCF_ACTIONNAME | BSUDCF_MOVESPEED | BSUDCF_GUANKAFLAG | BSUDCF_FLAG | BSUDCF_OTHERFLAG | BSUDCF_CARRYFLAG
	| BSUDCF_PATHFINDER | BSUDCF_REGENHP | BSUDCF_REGENMP | BSUDCF_CAMP | BSUDCF_EXP | BSUDCF_SHELD | BSUDCF_GRASS | BSUDCF_PLAYER
	| BSUDCF_MAXMP | BSUDCF_CURMP,
};

struct BeShareUnitData
{
	BeShareUnitData()
	{
		iChangeFlag = 0;
		iUnitTypeID = 0;
		iUnitLogicID = 0;
		fPosX = 0.0f;
		fPosY = 0.0f;
		fPosZ = 0.0f;
		fFace = 0.0f;
		fCurHp = 0.0f;
		fMaxHp = 0.0f;
		fRegenHP = 0.0f;
		iUpdateTime = 0;
		iAttackCD = 0;
		iUnitLevel = 1;
		fCurMp = 0.f;
		fMaxMp = 0.f;
		fRegenMp = 0.0f;
		eActionName = BAN_Null;
		iActionTime = 0;
		fMoveSpeed = 0.0f;
		iGuankaFlag = 0;
		iFlag = 0;
		iOtherFlag = 0;
		iUnitCarryFlag = 0;
		fTargetX = 0.f;
		fTargetY = 0.f;
		iPathSucessTime = 0;
		//		fTarFace = 0.0f;
		iExp = 0;
		fShild = 0.0f;
		fScale = 1.0f;
		byPlayer = -1;
	}
	int	iChangeFlag;
	unsigned int iUnitTypeID;	
	int iUnitLogicID;			
	float	fPosX;
	float	fPosY;
	float	fPosZ;
	float	fFace;
	float	fCurHp;
	float	fMaxHp;
	float	fRegenHP;
	int		iUpdateTime;
	int		iAttackCD;
	float	fPreAttack;			
	int		iUnitLevel;			
	float	fCurMp;				
	float	fMaxMp;
	float	fRegenMp;
	int		iCamp;				
	float	fMoveSpeed;
	int		iGuankaFlag;
	int		iFlag;
	int		iOtherFlag;
	int		iUnitCarryFlag;
	BeActionName eActionName;
	int iActionStartTime;
	int		iActionTime;
	float	fTargetX;
	float	fTargetY;
	int		iPathSucessTime;
	int		iExp;				
	float	fShild;				
	float	fScale;				
	int		iGrassIndex;		
	char	byPlayer;			
};

enum BeShareLockUnitChangeFlag
{
	BLUF_ATTACK = 1 << 0,		
	BLUF_MAGIC = 1 << 1,		
	BLUF_ARMOR = 1 << 2,		
	BLUF_MAGIC_ARMOR = 1 << 3,	
	BLUF_ATTACKSPEED = 1 << 4,	
	BLUF_CD_DEC = 1 << 5,		
	BLUF_CRITICAL = 1 << 6,		
};

struct BeShareLockUnitData
{
	BeShareLockUnitData()
	{
		iChangeFlag = 0;
	}
	int			iChangeFlag;
	float		fAttack;
	float		fMagic;
	float		fArmor;
	float		fMagicArmor;
	float		fAttackSpeed;
	float		fCdDec;
	float		fCritical;
};

enum BeEffectRace
{
	BER_ORIGIN = 0,
	BER_ATTACH = 1,
	BER_LINE_FOLLOWEARTH = 2,
	BER_LINE_TRACE = 3,
	BER_LINE_TRACE_COLLISION = 4,
	BER_CAST_TRACE = 5,
	BER_CAST_TRACE_COLLISION = 6,
	BER_CURVE_TRACE_MID = 7,
	BET_CURVE_TRACE_COLLISION = 8,
	BER_ATTACH_TRCE = 9,
	BET_CURVE_TRACE_FRANK = 10,	
	BET_DIRECT_UNIT = 11,	
};

enum BeEffectProperty
{
	BEP_LOGIC_EFFECT = (1 << 0),
	BEP_REMOVE_DELAY = (1 << 1),
	BEP_SCOPE_DAMAGE = (1 << 2),
	BEP_PERSIST_SHOW = (1 << 3),
	BEP_IGNOREHEIGHT = (1 << 4),
	BEP_IGNOREDIRECTION = (1 << 5),
	BEP_ONCYCLEROUND = (1 << 6),
};

enum BeEffectChangeFlag
{
	BECF_POS = 1 << 0,
	BECF_UPDATETIME = 1 << 1,
	BECF_FACE = 1 << 2,
	BECF_OWNERID = 1 << 3,
	BECF_TARUNITID = 1 << 4,
	BECF_MOVESPEED = 1 << 5,
	BECF_SCALE = 1 << 6,
	BECF_MISSILEARC = 1 << 7,
	BECF_MODEL = 1 << 8,
	BECF_ATTACHNODE = 1 << 9,
	BECF_PROPERTY = 1 << 10,
	BECF_CYCLEANGLE = 1 << 11,
	BECF_RACE = 1 << 12,
	BECF_CASTMISSILE = 1 << 13,	
	BECF_CASTNOTARGET = 1 << 14,
	BECF_VISION = 1 << 15,		
	BECF_SKILLTYPEID = 1 << 16,	
};

struct BeShareEffectData
{
	BeShareEffectData()
	{
		iChangeFlag = 0;
		iEffectLogicID = 0;
		fPosX = 0.0f;
		fPosY = 0.0f;
		fPosZ = 0.0f;
		iUpdateTime = 0;
		fFace = 0.0f;
		iOwnUnitID = 0;
		iTarUnitID = 0;
		fMoveSpeed = 0.0f;
		fScale = 1.0f;
		fMissileArc = 0.0f;
		fTarX = 0.0f;
		fTarY = 0.0f;

		iModleName = 0;
		iProperty = 0;
		fCycleAngel = 0.f;
		eRace = BER_ORIGIN;
		iSkillTypeID = 0;
	}
	int		iChangeFlag;
	int		iEffectLogicID;
	float	fPosX;
	float	fPosY;
	float	fPosZ;
	int		iUpdateTime;
	float	fFace;
	int		iOwnUnitID;	
	int		iTarUnitID;	
	float	fMoveSpeed;	
	float	fScale;		
	float	fMissileArc;
	float	fTarX;
	float	fTarY;
	float	fVisionRadius;

	int			iModleName;
	int			iProperty;
	float		fCycleAngel;
	BeEffectRace	eRace;
	int			iSkillTypeID;	
};

struct BeShareAttachData
{
	BeShareAttachData()
	{
		iUnitLogicID = 0;
		iRemoveTime = 0;
		iModleName = 0;
		bChangeFace = true;
		bRemove = false;
		iAttackerID = 0;
	}

	int		iUnitLogicID;
	int		iRemoveTime;
	bool	bChangeFace;
	bool	bRemove;	

	int		iModleName;
	int		iAttackerID;
};

struct BeShareMapItemData
{
	BeShareMapItemData()
	{
		iItemTypeID = 0;
		iLogicID = 0;
		fPosX = 0.0f;
		fPosY = 0.0f;
		bRemove = false;
	}
	int		iLogicID;
	int		iItemTypeID;
	float	fPosX;
	float	fPosY;
	bool	bRemove;
};

struct BeShareBufferData
{
	BeShareBufferData()
	{
		iBufferLogicID = 0;
		iBufferTypeID = 0;
		iRemoveTime = -1;
		bRemove = false;
		iUnitID = 0;
		byNum = 1;
		iAttackUnitID = 0;
	}
	int		iBufferLogicID;
	int		iBufferTypeID;
	int		iRemoveTime;
	bool	bRemove;
	int		iUnitID;	
	char	byNum;		
	int		iAttackUnitID;
};

enum BeSelfDataChangeFlag
{
	BSDCF_MONEY = 1 << 0,
	BSDCF_ENTERCDTIME = 1 << 1,
	BSDCF_ALLCDTIME = 1 << 2,
	BSDCF_SKILLTYPEID = 1 << 3,
	BSDCF_SKILLLEVEL = 1 << 4,
	BSDCF_SKILLCOUNTER = 1 << 5,
	BSDCF_ITEMINFO = 1 << 6,	
	BSDCF_ITEMENTERCDTIME = 1 << 7,
	BSDCF_ITEMALLCDTIME = 1 << 8,
	BSDCF_ITEMPIPLENUM = 1 << 9,

	BSDCF_ATTACKNUM = 1 << 11,
	BSDCF_MAGICNUM = 1 << 12,
	BSDCF_ARMOR = 1 << 14,
	BSDCF_MAGICARMOR = 1 << 15,
	BSDCF_BAOJI = 1 << 16,
	BSDCF_DECCD = 1 << 17,

	BSDCF_SELFID = 1 << 18,	
	BSDCF_CAMP = 1 << 20,	

	BSDCF_PassArmor = 1 << 21,
	BSDCF_PassMagicArmor = 1 << 22,
	BSDCF_Leech = 1 << 23,
	BSDCF_MagicLeech = 1 << 24,
	BSDCF_AttackRange = 1 << 25,
	BSDCF_Toughness = 1 << 26,
	BSDCF_PerPassArmor = 1 << 27,
	BSDCF_PerPassMagicArmor = 1 << 28,
};

struct BeShareSelfData
{
	void	ClearVec()
	{
		for (int i = 0; i < iMaxHeroSkillNum; i++)
		{
			bEnterCdTimePos[i] = false;
			bAllCdTimePos[i] = false;
			bSkillTypeIDPos[i] = false;
			bSkillLevelPos[i] = false;
			bSkillCounterPos[i] = false;
		}
		for (int i = 0; i < iMaxHeroItem; i++)
		{
			bItemInfoPos[i] = false;
			bItemEnterCDPos[i] = false;
			bItemAllCDTimePos[i] = false;
			bItemPiplePos[i] = false;
		}
	}
	BeShareSelfData()
	{
		iChangeFlag = 0;
		iUnitID = 0;
		iSelfMoney = 0;
		iSeat = -1;
		for (int i = 0; i < iMaxHeroSkillNum; i++)
		{
			iSkillEnterCdTime[i] = 0;
			iSkillAllCdTime[i] = 0;
			iSkillTypeID[i] = 0;
			bySkillLevel[i] = 0;
			bySkillCounter[i] = 0;
		}
		for (int i = 0; i < iMaxHeroItem; i++)
		{
			iItemInfo[i] = 0;
			iItemEnterCDTime[i] = 0;
			iItemAllCDTime[i] = 0;
			byItemPipleNum[i] = 1;
		}
		ClearVec();

		sAttackNum = 0;
		sMagicNum = 0;
		sArmor = 0;
		sMagicArmor = 0;
		s100BaoJi = 0;
		by100DecCD = 0;

		sPassArmor = 0;
		sPassMagicArmor = 0;
		sLeech = 0;
		sMagicLeech = 0;
		sAttackRange = 0;
		sToughness = 0;
		by100PerPassArmor = 0;
		by100PerPassMagicArmor = 0;
	}
	int		iChangeFlag;
	int		iUnitID;							
	int		iSelfMoney;							
	int		iSeat;								

	bool	bEnterCdTimePos[iMaxHeroSkillNum];
	int		iSkillEnterCdTime[iMaxHeroSkillNum];
	bool	bAllCdTimePos[iMaxHeroSkillNum];
	int		iSkillAllCdTime[iMaxHeroSkillNum];	
	bool	bSkillTypeIDPos[iMaxHeroSkillNum];
	int		iSkillTypeID[iMaxHeroSkillNum];		
	bool	bSkillLevelPos[iMaxHeroSkillNum];
	char	bySkillLevel[iMaxHeroSkillNum];		
	bool	bSkillCounterPos[iMaxHeroSkillNum];
	char	bySkillCounter[iMaxHeroSkillNum];	
	bool	bItemInfoPos[iMaxHeroItem];
	int		iItemInfo[iMaxHeroItem];			
	bool	bItemEnterCDPos[iMaxHeroItem];
	int		iItemEnterCDTime[iMaxHeroItem];		
	bool	bItemAllCDTimePos[iMaxHeroItem];
	int		iItemAllCDTime[iMaxHeroItem];		
	bool	bItemPiplePos[iMaxHeroItem];
	char	byItemPipleNum[iMaxHeroItem];		

	short	sAttackNum;			
	short	sMagicNum;			
	short	sArmor;				
	short	sMagicArmor;		
	short	s100BaoJi;			
	char	by100DecCD;			
	short	sPassArmor;			
	short	sPassMagicArmor;	
	short	sLeech;				
	short	sMagicLeech;		
	short	sAttackRange;		
	short	sToughness;			
	char	by100PerPassArmor;	
	char	by100PerPassMagicArmor;
};

struct BeShareChainEffectData
{
	BeShareChainEffectData()
	{
		iSrcUnitID = 0;
		iTarUnitID = 0;
		iEffectID = 0;
		iRemoveTime = 0;
		iLogicID = 0;
	}
	int				iSrcUnitID;	
	int				iTarUnitID;	
	int				iEffectID;	
	int				iRemoveTime;
	int				iLogicID;	
};

enum  BeShareDamageType
{
	BSDT_DAMAGE = 0,
	BSDT_ADD_LIFE,
	BSDT_CRITILE,
	BSDT_GOLDEN,
	BSDT_EXP,
	BSDT_ZHANCHANG_FEATS,
	BSDT_ZHANCHANG_SCORE,
};

struct BeShareDamageData
{
	BeShareDamageData()
	{
		iAttackerID = 0;
		iUnitID = 0;
		iDamageType = 0;
		fValue = 0.f;
		eType = BSDT_DAMAGE;
		iKillUnitID = 0;
	}
	int			iAttackerID;
	int			iUnitID;	
	int			iDamageType;
	float		fValue;
	int			iKillUnitID;
	BeShareDamageType	eType;
};

enum BeKillInfo
{
	BKI_NONE = 1 << 0,		
	BKI_DaShaTeSha = 1 << 1,
	BKI_WuRenNengDang = 1 << 2,
	BKI_JieJinShen = 1 << 3,
	BKI_ChaoShen = 1 << 4,	
	BKI_ZhongJie = 1 << 5,	
	BKI_TuanMie = 1 << 6,	
};
struct BeShareKillHeroData
{
	BeShareKillHeroData()
	{
		uiAttackerTypeID = 0;
		byAttackerSeat = 0;
		byDeathPlayer = 0;
		uiDeathUnitTypeID = 0;
		eInfo = BKI_NONE;
		byZhuGongNum = 0;
		for (int i = 0; i < 20; i++)
		{
			byZhuGongPlayer[i] = 0;
		}
	}
	unsigned int uiAttackerTypeID;	
	char		byAttackerSeat;		
	char		byDeathPlayer;		
	unsigned int uiDeathUnitTypeID;	
	char		eType;				
	char		eInfo;				
	char		byZhuGongNum;		
	char		byZhuGongPlayer[20];
};

enum BeUiWindowType
{
	BWT_NONE,					
	BWT_SPELL_CAST,				
	BWT_SPELL_EFFECT,			
	BWT_SMALLMAP_MESSAGE,		
	BWT_DALUANDOU_QIANGKUANG,	
	BWT_DALUANDOU_JINBI,		
	BWT_RIGHT_LEFT,				

	BWT_RELIVEBAR,                 
	BWT_TPBAR,                     

	BWT_TISHI,					
	BWT_TIXING,					
	BWT_DAOJISHI,				
	BWT_TIXINGDAOJISHI,			

	BWT_MEISHUZI,				
	BWT_XIAWANGSHANG,			
	BWT_BUDONG,					
	BWT_SHIJIANDAOSHU,			

	BWT_CAMARA_SET,                
	BWT_XIXUE,					
	BWT_RELOAD_AMMO,			

	BWT_RED_UP_APPEAR_GUARD,	
	BWT_RED_MID_APPEAR_GUARD,	
	BWT_RED_DOWN_APPEAR_GUARD,	
	BWT_BLUE_UP_APPEAR_GUARD,	
	BWT_BLUE_MID_APPEAR_GUARD,	
	BWT_BLUE_DOWN_APPEAR_GUARD,	
	BWT_GUARD_REBRITH_TIME,		
	RED_UP_AFTER_DALONG,		
	BLUE_UP_AFTER_DALONG,		
};

struct BeShareUIWindowData
{
	BeShareUIWindowData()
	{
		eWindowType = BWT_NONE;
		iSkillID = 0;
		iRemoveTime = 0;
		iResID = 0;
		iUnitID = -1;
		bRemove = false;
		iSkipTime = 0;
		fValue1 = 0.0f;
		fValue2 = 0.0f;
		fValue3 = 0.0f;
		fValue4 = 0.0f;
	}
	BeUiWindowType		eWindowType;
	int					iSkillID;
	int					iRemoveTime;
	int					iResID;		
	int					iUnitID;	
	bool				bRemove;	
	int					iSkipTime;	
	float				fValue1;	
	float				fValue2;
	float				fValue3;
	float				fValue4;
};

struct BeShareTabInfo
{
	BeShareTabInfo()
	{
		uiChangeFlag = 0;
		bySeat = 0;
		byKill = 0;
		byDead = 0;
		byHelp = 0;
		for (int i = 0; i < 6; i++)
		{
			uiItem[i] = 0;
		}
		byLevel = 0;
		uiReliveTime = 0;
		usKillSolider = 0;
		uiBigSkillTime = 0;
		uiSurrenderTime = 0;
		uiFeatsValue = 0;
		uiMoraleValue = 0;
		uiTeamScore = 0;
	}
	unsigned int	uiChangeFlag;
	char			bySeat;		
	char			byKill;		
	char			byDead;		
	char			byHelp;		
	unsigned int	uiItem[6];	
	char			byLevel;	
	short			sUnitID;	
	unsigned int	uiReliveTime;
	unsigned short	usKillSolider;
	unsigned int	uiBigSkillTime;
	unsigned int	uiSurrenderTime;
	unsigned int	uiFeatsValue;
	unsigned int	uiMoraleValue;
	unsigned int	uiTeamScore;
};

enum BeUnitFlyType
{
	BUFT_MOVE,	
	BUFT_PARABOLA,
};
struct BeShareUnitFlyData
{
	BeShareUnitFlyData()
	{
		iUnitID = -1;
		byFlyType = BUFT_MOVE;
		iFlyTime = 0;
		sStartX = 0;
		sStartY = 0;
		sTarX = 0;
		sTarY = 0;
		iTargetID = 0;
		iSkillTypeID = 0;
	}
	int			iUnitID;	
	char		byFlyType;	
	int			iFlyTime;	
	short		sStartX;	
	short		sStartY;
	short		sTarX;		
	short		sTarY;
	int			iTargetID;	
	int			iSkillTypeID;
};

struct BeShareSmallMapMessage
{
	BeShareSmallMapMessage()
	{
		byMessageType = 0;
		usPosX = 0;
		usPosY = 0;
	}
	unsigned	char	byMessageType;
	unsigned	short	usPosX;		
	unsigned	short	usPosY;		
};

enum BeGameNoticeType
{
	BNT_Null,		
	BNT_Relink,		
	BNT_Leave,		
	BNT_NoPlay_Little,
	BNT_NoPlay_Big,	
	BNT_New_Add,	
};
struct BeShareGameNotice
{
	BeShareGameNotice()
	{
		bySeat = -1;
		byType = BNT_Null;
	}

	char			bySeat;
	char			byType;
};

struct SeSkillLvlData
{
	SeSkillLvlData()
	{
		iCastTime = 0;
		iShakesTime = 0;
		iEffectTime = 0;
		iCoolDown = 0;
		iManaSpend = 0;
		iSpellScope = 0;
		iSpellAngle = 0;
		iSpellDistance = 0;
		iPilePointNums = 0;
		iMissileModle = 0;
		iSelfModle = 0;
		iTargetModle = 0;
		iDstPosModle = 0;
		iSkillEffectModle = 0;
	}

	SeSkillLvlData(std::shared_ptr<const SkillTable> pkRes, int iLevel)
	{
		if (iLevel < 0 || iLevel >= iMaxSkillLevel)
		{
			iLevel = 0;
		}
		//iCastTime = pkRes->iCastTime[iLevel];
		//iShakesTime = pkRes->iShakesTime[iLevel];
		//iEffectTime = pkRes->iEffectTime[iLevel];
		//iCoolDown = pkRes->iCoolDown[iLevel];
		//iManaSpend = (int)pkRes->fManaSpend[iLevel];
		//iSpellScope = pkRes->iEffectScope[iLevel];
		//iSpellAngle = pkRes->iSkillAngle[iLevel];
		//iSpellDistance = pkRes->iSkillDistance[iLevel];
		//iPilePointNums = pkRes->iPilePointNums[iLevel];
		//afValue[0] = pkRes->fValueA[iLevel];
		//afValue[1] = pkRes->fValueB[iLevel];
		//afValue[2] = pkRes->fValueC[iLevel];
		//afValue[3] = pkRes->fValueD[iLevel];
		//afValue[4] = pkRes->fValueE[iLevel];
		//afValue[5] = pkRes->fValueF[iLevel];
		//afValue[6] = pkRes->fValueG[iLevel];
		//fMissileSpeed = pkRes->fMissileSpeed;
		//iMissileModle = pkRes->iMissileModel;
		//iSelfModle = pkRes->iSrcEffect;
		//iTargetModle = pkRes->iDstUnitEffect;
		//iDstPosModle = pkRes->iDstPosEffect;
		//iSkillEffectModle = pkRes->iSkillEffect;
	}

	int		iNextLearnLevel;
	int		iCastTime;		
	int		iShakesTime;	
	int		iEffectTime;	
	int		iCoolDown;		
	int		iManaSpend;		
	int		iSpellScope;	
	int		iSpellDistance;	
	int		iSpellAngle;	
	int		iPilePointNums;	
	float	afValue[iMaxSkillContentData];
	int		iMissileModle;
	int		iSelfModle;
	int		iTargetModle;
	int		iDstPosModle;
	int		iSkillEffectModle;
	float	fMissileSpeed;

	float GetDataValue(int iIndex) const
	{
		if (iIndex < 0 || iIndex >= iMaxSkillContentData)
		{
			return 0.0f;
		}
		return afValue[iIndex];
	}
	int GetSpellScope(void) const
	{
		return iSpellScope;
	}
	int		GetSpellAngle(void)	const
	{
		return iSpellAngle;
	}
	void SetCoolDown(int iValue)
	{
		iCoolDown = iValue;
	}
	int GetCoolDown(void) const
	{
		return iCoolDown;
	}
	int GetSpellDistance(void) const
	{
		return iSpellDistance;
	}
	int GetEffectTime(void) const
	{
		return iEffectTime;
	}
	void SetEffectTime(int iTime)
	{
		iEffectTime = iTime;
	}
	int GetSkillManaSpend(void) const
	{
		return iManaSpend;
	}
	float	GetMissileSpeed()
	{
		return fMissileSpeed;
	}
	int GetSkillCoolDown(void) const
	{
		return iCoolDown;
	}
	int GetCastTime(void) const
	{
		return iCastTime;
	}
	void SetCastTime(int iTime)
	{
		iCastTime = iTime;
	}
	int GetMissileModle()
	{
		return iMissileModle;
	}
	int GetSelfModle()
	{
		return iSelfModle;
	}
	int GetTargetModle()
	{
		return iTargetModle;
	}
	int GetDstPosModle()
	{
		return iDstPosModle;
	}
	int	GetSkillEffectModle()
	{
		return iSkillEffectModle;
	}
};

struct SeCalSkillLvlData : public SeSkillLvlData
{
	float fBlastPer;
	float fBlastDmgPer;
	bool  bCalBlast;

	bool abCalBlast[iMaxSkillContentData];
	SeCalSkillLvlData()
	{
		fBlastPer = 0.0f;
		fBlastDmgPer = 1.75f;
		bCalBlast = false;
	}

	SeCalSkillLvlData(const SeSkillLvlData& other)
	{
		fBlastPer = 0.0f;
		fBlastDmgPer = 1.75f;
		bCalBlast = false;
		iCastTime = other.iCastTime;
		iShakesTime = other.iShakesTime;
		iEffectTime = other.iEffectTime;
		iCoolDown = other.iCoolDown;			
		iManaSpend = other.iManaSpend;			
		iSpellScope = other.iSpellScope;		
		iSpellAngle = other.iSpellAngle;
		iSpellDistance = other.iSpellDistance;	
		iPilePointNums = other.iPilePointNums;	
		iMissileModle = other.iMissileModle;
		iSelfModle = other.iSelfModle;
		fMissileSpeed = other.fMissileSpeed;
		iTargetModle = other.iTargetModle;
		iDstPosModle = other.iDstPosModle;
		iSkillEffectModle = other.iSkillEffectModle;
	}

	SeCalSkillLvlData& operator = (const SeSkillLvlData& other)
	{
		iCastTime = other.iCastTime;			
		iShakesTime = other.iShakesTime;		
		iEffectTime = other.iEffectTime;		
		iCoolDown = other.iCoolDown;			
		iManaSpend = other.iManaSpend;			
		iSpellScope = other.iSpellScope;		
		iSpellAngle = other.iSpellAngle;
		iSpellDistance = other.iSpellDistance;	
		iPilePointNums = other.iPilePointNums;	
		iMissileModle = other.iMissileModle;
		iSelfModle = other.iSelfModle;
		fMissileSpeed = other.fMissileSpeed;
		iTargetModle = other.iTargetModle;
		iDstPosModle = other.iDstPosModle;
		iSkillEffectModle = other.iSkillEffectModle;
		return *this;
	}
	SeCalSkillLvlData& operator = (const SeCalSkillLvlData& other)
	{
		fBlastPer = other.fBlastPer;
		fBlastDmgPer = other.fBlastDmgPer;
		bCalBlast = other.bCalBlast;
		iCastTime = other.iCastTime;			
		iShakesTime = other.iShakesTime;		
		iEffectTime = other.iEffectTime;		
		iCoolDown = other.iCoolDown;			
		iManaSpend = other.iManaSpend;			
		iSpellScope = other.iSpellScope;		
		iSpellAngle = other.iSpellAngle;
		iSpellDistance = other.iSpellDistance;	
		iPilePointNums = other.iPilePointNums;	
		iMissileModle = other.iMissileModle;
		iSelfModle = other.iSelfModle;
		fMissileSpeed = other.fMissileSpeed;
		iTargetModle = other.iTargetModle;
		iDstPosModle = other.iDstPosModle;
		iSkillEffectModle = other.iSkillEffectModle;
		return *this;
	}

	void SetBlastData(float fPer, float fDmgPer)
	{
		fBlastPer = fPer;
		fBlastDmgPer = fDmgPer;
	}

	void SetCalBlast(bool bCal)
	{
		bCalBlast = bCal;
	}

	float GetBlastPer(void) const
	{
		return fBlastPer;
	}

	float GetDataValue(int iIndex) const
	{
		if (iIndex < 0 || iIndex >= iMaxSkillContentData)
		{
			return 0.0f;
		}
		float fValue = afValue[iIndex];
		if (bCalBlast && abCalBlast[iIndex])
		{
			fValue *= fBlastDmgPer;
		}
		return fValue;
	}

	int GetSpellScope(void) const
	{
		return iSpellScope;
	}
	int		GetSpellAngle(void)	const
	{
		return iSpellAngle;
	}
	int GetSpellDistance(void) const
	{
		return iSpellDistance;
	}
	int GetEffectTime(void) const
	{
		return iEffectTime;
	}
	int GetSkillManaSpend(void) const
	{
		return iManaSpend;
	}
	int GetSkillCoolDown(void) const
	{
		return iCoolDown;
	}
	int GetCastTime(void) const
	{
		return iCastTime;
	}
	int GetShakesTime(void) const
	{
		return iShakesTime;
	}
	int GetPilePointNums(void) const
	{
		return iPilePointNums;
	}
	void	SetPilePointNums(int iValue)
	{
		iPilePointNums = iValue;
	}
	void SetSkillManaSpend(int iMana)
	{
		iManaSpend = iMana;
	}
	void SetDataValue(int iIndex, float fValue, bool bCalBlast)
	{
		if (iIndex < 0 || iIndex >= iMaxSkillContentData)
		{
			return;
		}
		afValue[iIndex] = fValue;
		abCalBlast[iIndex] = bCalBlast;
	}

	void SetCoolDown(int iCD)
	{
		iCoolDown = iCD;
	}
};

enum BeUnitFlag
{
	BUF_DEAD = (1 << 0),				
	BUF_REMOVE = (1 << 1),				
	BUF_MORPHUNIT = (1 << 2),			
	BUF_ISGHOST = (1 << 3),				
	BUF_ISDIVIDMAN = (1 << 4),			
	BUF_ISSUMMONTEMP = (1 << 5),		
	BUF_ISSUMMONPER = (1 << 6),			
	BUF_ISBLOCKED = (1 << 7),			
	BUF_ISPERSIST = (1 << 8),			
	BUF_ISINVINCIBLE = (1 << 9),		
	BUF_HASBUMPATTACK = (1 << 10),		
	BUF_NOOBSTACLE = (1 << 11),			
	BUF_IGNOREUNITOBS = (1 << 12),		
	BUF_IGNOREFIXEDOBS = (1 << 13),		
	BUF_MOVING = (1 << 14),				
	BUF_OBSTACLESET = (1 << 15),		
	BUF_RELIVE = (1 << 16),				
	BUF_BUYLIVE = (1 << 17),			
	BUF_SPELL_SHARK = (1 << 18),		
	BUF_HIDE = (1 << 19),				
	BUF_HASINVISIBLE = (1 << 20),		
	BUF_HASSHARED = (1 << 21),			
	BUF_HASHALOSKILL = (1 << 22),		
	BUF_IGNOREPHYSICRESIST = (1 << 23),	
	BUF_NEEDUPDATESKILL = (1 << 24),	
	BUF_NEEDUPDATEITEMSKILL = (1 << 25),
	BUF_ISPERSISTSKILL = (1 << 26),		
	BUF_HAS_BIRTHACTION = (1 << 27),	
	BUF_NEEDRECYCLEITEM = (1 << 28),	
	BUF_IGNOREDOODADOBS = (1 << 29),	
	BUF_ITEMINVALID = (1 << 30),		
	BUF_IGNORETERRAINOBS = (1 << 31),

	BUF_HIDEALL = BUF_HIDE | BUF_NOOBSTACLE | BUF_ISINVINCIBLE,
	BUF_IGNOREALLOBS = BUF_IGNOREUNITOBS | BUF_IGNOREFIXEDOBS | BUF_IGNOREDOODADOBS,
	BUF_TEMPSUMMONNOTCONTROL = BUF_ISINVINCIBLE | BUF_ISSUMMONTEMP | BUF_IGNOREALLOBS,
	BUF_TEMPGHOSTNOTCONTROL = BUF_ISINVINCIBLE | BUF_ISGHOST | BUF_IGNOREALLOBS,	
};

enum BeUnitOtherFlag
{
	BUOF_72BIAN = (1 << 0),			
	BUOF_SPECIAL_RELIVE = (1 << 1),	
	BUOF_CANNT_TP = (1 << 2),
	BUOF_WEIYI = (1 << 3),			
	BUOF_AREA = (1 << 4),			
	BUOF_SHOUWEI = (1 << 5),		
	BUOF_ATK_NOCD = (1 << 6),		
	BUOF_IGNORE_DIVISIVE = (1 << 7),
	BUOF_NEEDREFRESHITEM = (1 << 8),
	BUOF_CANNOT_WEIYI = (1 << 9),	
	BUOF_NODISTANCE_SHOP = (1 << 10),
	BUOF_FELLOW = (1 << 11),
	BUOF_CANT_AUTOBETARGET = (1 << 12),
	BUOF_ONLYATTACKBUILD = (1 << 13),
	BUOF_CANT_DIZZY = (1 << 14),	
	BUOF_NEVEDEATH = (1 << 15),		
	BUOF_SPELL_DELAY = (1 << 16),	
	BUOF_IGNORE_SPLAHS = (1 << 17),	
	BUOF_BUILDING_TURN = (1 << 18),	
	BUOF_GUT_EATHOTHER = (1 << 19),	
	BUOF_AREA_SELF = (1 << 20),		
	BUOF_NO_DEATH_ACTION = (1 << 21),
	BUOF_Clr_WeiYi = (1 << 22),		
};

enum BeUnitCarryFlag
{
	BUCF_ISFORBIDITEM = (1 << 0),
	BUCF_ISFORBIDSKILL = (1 << 1),
	BUCF_CANNOTMOVE = (1 << 2),					
	BUCF_CANNOTATTACK = (1 << 3),				
	BUCF_DIZZY = (1 << 4),						
	BUCF_ISTETANIC = (1 << 5),					
	BUCF_ISINVINCIBLE = (1 << 6),				
	BUCF_ISINVISIBLE = (1 << 7),				
	BUCF_ISMAGICIMMUNITY = (1 << 8),			
	BUCF_ISPHYSICIMMUNITY = (1 << 9),			
	BUCF_NOTINVISIBLE = (1 << 10),				
	BUCF_NOTFINDTARGET = (1 << 11),				
	BUCF_HASCHANGESCALE = (1 << 12),			
	BUCF_ISJUMPING = (1 << 13),					
	BUCF_ISAIRCAST = (1 << 14),					
	BUCF_ISMOVING = (1 << 15),					
	BUCF_HASICED = (1 << 16),					
	BUCF_BREAKPERSIST = (1 << 17),				
	BUCF_CANNOTDEATH = (1 << 18),				
	BUCF_CANNOTCONTROL = (1 << 19),				
	BUCF_IMMUNITY_CONTROL = (1 << 20),			
	BUCF_NO_HALO_EFFECT = (1 << 21),			
	BUCF_ISPHYSICNODAMAGE = (1 << 22),			
	BUCF_NO_WEIYI = (1 << 23),					
	BUCF_NOHALO = (1 << 24),					
	BUCF_NO_DAMAGE_EFFECT = (1 << 25),			
	BUCF_NO_BUFFER_EFFECT = (1 << 26),			
	BUCF_NO_REVERTHPMP = (1 << 27),				
	BUCF_ONLYCANUSEITEM = (1 << 28),			
	BUCF_NEW_WALK = (1 << 29),					
	BUCF_IGNORE_PHYSICIMMUNITY = (1 << 30),		
	BUCF_DEATH_CMD = (1 << 31),					

	BUCF_ISALLFORBID = BUCF_CANNOTCONTROL | BUCF_DIZZY,
	BUCF_NOMOVE_NOATTACK = BUCF_CANNOTMOVE | BUCF_CANNOTATTACK,
	BUCF_FORBIDITEMANDSKILL = BUCF_ISFORBIDITEM | BUCF_ISFORBIDSKILL,
};

enum BeSecretAreaFlag
{
	BSAF_LAYER = 1 << 0,
	BSAF_END_TIME = 1 << 1,
	BSAF_FAIL = 1 << 2,

};

struct BeSecretAreaInfo
{
	BeSecretAreaInfo()
	{
		bySecretAreaFlag = 0;
		uiLayer = 1;
		uiEndTime = 0;
		byEnd = 0;
	}

	char			bySecretAreaFlag;
	unsigned short  uiLayer;
	unsigned int    uiEndTime;
	char			byEnd;
};

struct BeUnitBeDamageRecord
{
	BeUnitBeDamageRecord(int iParamID, unsigned int	uiParamBeDamage)
	{
		iID = iParamID;
		uiBeDamage = uiParamBeDamage;
	}

	BeUnitBeDamageRecord()
	{
		iID = 0;
		uiBeDamage = 0;
	}

	int				iID;
	unsigned int	uiBeDamage;
};
