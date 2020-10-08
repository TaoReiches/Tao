#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <map>
#include <unordered_map>
#include "TW_MemoryPool.h"

static	const	int UNIT_MAX_BUFFER = 256;
static	const	int MAX_ACTIVE_SKILLS = 4;

enum BeUnitAction
{
	BUA_STAND = 0,
	BUA_WALK,
	BUA_ATTACK,
	BUA_SPELL,
	BUA_DEATH,
	BUA_DISSIPATE,
	BUA_DECAY,
	BUA_BIRTH,

	BUA_MORPH,
	BUA_PORTRAIT,

	BUA_UNKOWN,
	BUA_NULL,
};

enum BeUnitClientFlag
{
	BUCF_ISNOSELECT = (1 << 0),
	BUCF_HIDE_EFFECT = (1 << 1),
	BUCF_ISNO_SHADOWS = (1 << 2),
	BUCF_ONLY_SELF_FULL_ALPHA = (1 << 3),
	BUCF_ALLYGREEN_ENEMYRED = (1 << 4),
	BUCF_ONLY_SELF_CAN_SEE = (1 << 5),
	BUCF_PLAYER_HERO = (1 << 6),
	BUCF_HIDE = (1 << 7),
	BUCF_SPEICAL_DIVIDMANCOLOR = (1 << 8),
};

enum BeUnitImmunityFlag
{
	BUIF_DAMAGE_DEC_IM = (1 << 0),
	BUIF_ATTACK_SPEED_SLOW_IM = (1 << 1),
	BUIF_SPEED_SLOW_IM = (1 << 2),
	BUIF_ARMOR_DEC_IM = (1 << 3),
	BUIF_BEDEMAGED_PER_IM = (1 << 4),
	BUIF_DIZZY_IM = (1 << 5),


	BUIF_ALL_IM = BUIF_SPEED_SLOW_IM | BUIF_ARMOR_DEC_IM,
	BUIF_BATI = BUIF_DIZZY_IM | BUIF_SPEED_SLOW_IM | BUIF_ATTACK_SPEED_SLOW_IM,
};

class BeLearnSkillData
{
	DECLARE_POOL_ARRAY(BeLearnSkillData);
public:
	int			iTypeID;
	int			iLearnLevel;
	int			iPos;

	BeLearnSkillData()
	{
		iTypeID = 0;
		iLearnLevel = 0;
		iPos = 0;
	}
};

struct BeDeathRecordData
{
	int iUnitType;
	int iUnitID;
	int iSkillTypeID;
	float fDamage;
	int iHitTime;


	BeDeathRecordData()
	{
		iUnitType = 0;
		iUnitID = 0;
		iSkillTypeID = 0;
		fDamage = 0.0f;
		iHitTime = 0;
	}
};

struct BeFinalDeathRecordData
{
	int iUnitID;
	std::vector<int> aiItemTypeID;
	std::map<int, float> kMapDamage;
	BeFinalDeathRecordData()
	{
		iUnitID = 0;
	}
};

enum BeUnitMorphType
{
	BUMT_CHANGE_UNIT,
	BUMT_CHANGE_HERO,
	BUMT_SKILL_MORPH,
	BUMT_SKILL_WOODOO,
	BUMT_CHANGE_MODEL,
	BUMT_SKILL_UNIT,
	BUMT_CHANGE_MODELSKILL,
	BUMT_CHANGESKILL,
};

class BeSkill;
struct BeUnitData
{
	DECLARE_POOL(BeUnitData);

	int					iPlayer;
	int					iControl;
	int					iCurExp;
	int					iLevel;

	float				fOrgArmor;
	float				fOrgMagicArmor;
	float				fOrgMaxHP;
	float				fOrgRegenHP;
	float				fOrgMaxMP;
	float				fOrgRegenMP;
	float				fOrgDamage;
	float				fOrgMoveSpeed;
	int					iOrgAttackCD;
	int					iMissileModel;
	float				fMissileSpeed;

	float				fHP;
	float				fMP;
	float				fMaxHP;
	float				fRegenHP;
	float				fMaxMP;
	float				fRegenMP;
	float				fDamage;
	float				fArmor;
	float				fMagicArmor;

	BeAttackType		eAttackType;
	int					eMoveType;
	float   			fAttackRange;
	float				fMoveSpeed;
	int					iAttackCD;
	int					iAttackDamagePt;
	int					iAttackBackPt;
	int					iAttackSpeedAdd;

	float				fBaseArmor;
	float				fBaseMaxHP;
	float				fBaseRegenHP;
	float				fBaseMaxMP;
	float				fBaseRegenMP;
	float				fBaseDamage;
	float				fBaseMoveSpeed;
	float				fAddDamage;
	float				fAddArmor;
	float				fAddArmorPer;
	float				fAddMagicArmor;
	float				fAddMagicArmorPer;

	BeUnitAction		eActionType;
	int					iActionCurTime;
	int					iActionAllTime;
	int					iAttackElapseTime;

	float				fPosX;
	float				fPosY;
	float				fPosZ;
	float				fFace;
	float				fPitch;

	float				fScale;

	float				fBeDamagePerOrg;
	float				fDamagePerOrg;

	float				fBeDamagePer;
	float				fDamagePer;
	int    				iAttackingUnitID;

	int					iUnitCurLiveTime;
	int					iUnitAllLiveTime;
	unsigned int		dwUnitCreateTime;
	unsigned int				uiUnitReliveTime;

	const UnitTable* pkRes;
	BeSkill* apkUISkill[iMaxHeroSkillNum];

	BeLearnSkillData* akLearnSkill;
	int					iSkillPointSum;
	int					iUsedSkillPoint;

	BeUnitMorphType		eMorphType;
	int					iTypeID;

	int					iUnitProperty;
	int					iVisionRaius;

	float				fOrgSkillDamagePer;
	float				fSkillDamagePer;
	float				fSkillBlastPer;

	float				fDecAntiMagic;
	float				fLeech;
	float				fMagicLeech;
	float				fDecArmor;
	float				fPerDecArmor;
	float				fPerDecMagicArmor;
	float				fDecMPCost;
	float				fDecCDTime;
	float				fPerCDTime;
	float				fEnmityPoint;
	float				fPerDamageReduce;
	float				fArmorPass;
	float				fMagicDamage;
	float				fOrgMagicDamage;
	float				fEvadeRate;
	float				fDecDamage;
	float				fBaoJi;
	float				fBaoJiDamagePer;
	float				fBaoJiDecDamage;
	float				fToughness;
	float				fTreatment;

	int					iOrgPlayer;

	void SaveData(BeUnitMorphType eType, BeUnitData& other) const;
	void LoadData(BeUnitMorphType eType, const BeUnitData& other);

protected:
	BeUnitData();
	~BeUnitData();
private:
	BeUnitData(const BeUnitData& other);
	BeUnitData& operator = (const BeUnitData& other);
};

struct BufChangeScaleData
{
	BufChangeScaleData()
	{
		fScale = 0;
		iAllTime = 0;
		iCurTime = 0;
	}
	float	fScale;
	int		iAllTime;
	int		iCurTime;
};

typedef Es::HashMap<int, BufChangeScaleData>	MapBufChangeScale;

enum BeUnitActorFlag
{
	BUAF_REMOVE = (1 << 0),
	BUAF_STOP_ACTION = (1 << 1),
	BUAF_PERSIST_ACTION = (1 << 2),
	BUAF_PERSIST_CUR = (1 << 3),
	BUAF_IS_SLEEP = (1 << 4),
	BUAF_DEFAULT_ACTION_TIME = (1 << 5),
};

enum UnitAutoSpellType
{
	UAST_DEFAULT = (1 << 0),
	UAST_ATTACKED = (1 << 1),
	UAST_DAMAGED = (1 << 2),
	UAST_BAONU = (1 << 3),

	UAST_ALL = ~0,
};

#define DEATH_RECORD_TIME 10 * 1000
#define LABEL_VEC_CAP 4

static	const	int		iMaxUnitItemSkillCount = 2;

enum UnitUseSkillResultType
{
	UUSRT_OK = 0,

	UUSRT_EER_COMMON,
	UUSRT_EER_MANA_NOT_ENOUGH,
	UUSRT_EER_CD,
	UUSRT_EER_FORBID_SKILL,
	UUSRT_EER_TYPENOTMATCHING,
	UUSRT_EER_INVINCIBLE,
	UUSRT_EER_MAGICIMMUNITY,
	UUSRT_ERR_TO_MYSELF,
	UUSRT_ERR_TO_ANCIENT,
	UUSRT_ERR_TO_GUARD,
	UUSRT_EER_TO_BUILDING,
	UUSRT_EER_TO_NORMAL,
	UUSRT_EER_TO_SAME_GROUP,
	UUSRT_EER_TO_MACHINE,
	UUSRT_EER_TO_SperHero,

	UUSRT_EER_ADDSKILL,
};

enum BeUnitStaticProperty
{
	BUSP_CLASS_NORMAL = (1 << 0),
	BUSP_CLASS_HERO = (1 << 1),
	BUSP_CLASS_BUILDING = (1 << 2),
	BUSP_CLASS_SOLIDER = (1 << 20),
	BUSP_CLASS_YEGUAI = (1 << 21),
	BUSP_CLASS_BOSS = (1 << 22),
	BUSP_CLASS_TOUSHI = (1 << 23),
	BUSP_CLASS_TOTEM = (1 << 24),
	BUSP_CLASS_BRAND = (1 << 25),
	BUSP_CLASS_MIJING = (1 << 26),

	BUSP_MOVETYPE_FLOOR = (1 << 3),
	BUSP_MOVETYPE_AIR = (1 << 4),

	BUSP_PROP_NONE = (1 << 5),
	BUSP_PROP_MACHINE = (1 << 6),
	BUSP_PROP_ORGANIC = (1 << 7),
	BUSP_PROP_GUARD = (1 << 8),

	BUSP_PROP_ANCIENT = (1 << 9),

	BUSP_WEAPON_MELEE = (1 << 10),
	BUSP_WEAPON_MISSILE = (1 << 11),

	BUSP_NO_ITEMPANEL = (1 << 12),

	BUSP_HERO = BUSP_CLASS_HERO
	| BUSP_MOVETYPE_FLOOR
	| BUSP_PROP_MACHINE | BUSP_PROP_ORGANIC
	| BUSP_PROP_ANCIENT
	| BUSP_WEAPON_MELEE | BUSP_WEAPON_MISSILE,
	BUSP_BUILDING = BUSP_CLASS_BUILDING
	| BUSP_MOVETYPE_FLOOR
	| BUSP_PROP_MACHINE | BUSP_PROP_ORGANIC
	| BUSP_PROP_ANCIENT
	| BUSP_WEAPON_MELEE | BUSP_WEAPON_MISSILE,
	BUSP_NORMAL = BUSP_HERO | BUSP_CLASS_NORMAL | BUSP_MOVETYPE_AIR | BUSP_NO_ITEMPANEL | BUSP_CLASS_MIJING,
	BUSP_BUYER = BUSP_HERO | BUSP_CLASS_NORMAL | BUSP_MOVETYPE_AIR,
};

enum BeUnitDynamicProperty
{
	BUDP_SUMMON = (1 << 0),
	BUDP_NOTSUMMON = (1 << 1),

	BUDP_CAMP_ENEMY = (1 << 2),
	BUDP_CAMP_NATURE = (1 << 3),
	BUDP_CAMP_SELF = (1 << 4),
	BUDP_CAMP_ALLY = (1 << 5),

	BUDP_INVISIBLE = (1 << 6),
	BUDP_VISION_OUTSIGHT = (1 << 7),

	BUDP_DEAD = (1 << 8),
	BUDP_ALIVE = (1 << 9),

	BUDP_MAGIC_IMMUNITY = (1 << 10),
	BUDP_PHYSIC_IMMUNITY = (1 << 11),

	BUDP_INVINCIBLE = (1 << 12),

	BUDP_CASTER = (1 << 13),
	BUDP_NOTCASTER = (1 << 14),

	BUDP_ATTACKABLE_ENEMY = BUDP_SUMMON | BUDP_NOTSUMMON
	| BUDP_CAMP_ENEMY
	| BUDP_VISION_OUTSIGHT | BUDP_INVISIBLE
	| BUDP_ALIVE
	| BUDP_MAGIC_IMMUNITY
	| BUDP_CASTER
	| BUDP_NOTCASTER,

	BUDP_ENEMY = BUDP_SUMMON | BUDP_NOTSUMMON
	| BUDP_CAMP_ENEMY
	| BUDP_VISION_OUTSIGHT | BUDP_INVISIBLE
	| BUDP_ALIVE
	| BUDP_MAGIC_IMMUNITY | BUDP_INVINCIBLE
	| BUDP_CASTER
	| BUDP_NOTCASTER | BUDP_PHYSIC_IMMUNITY,

	BUDP_ALLY = BUDP_SUMMON | BUDP_NOTSUMMON
	| BUDP_CAMP_ALLY | BUDP_CAMP_SELF
	| BUDP_VISION_OUTSIGHT | BUDP_INVISIBLE
	| BUDP_ALIVE
	| BUDP_MAGIC_IMMUNITY | BUDP_INVINCIBLE
	| BUDP_CASTER
	| BUDP_NOTCASTER | BUDP_PHYSIC_IMMUNITY,
};

enum UnitActorTransType
{
	UATT_NONE_TRANS = 0,
	UATT_WHOLE_TRANS,
};