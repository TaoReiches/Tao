#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include "TW_FlagObject.h"
#include "TW_Pos2.h"
#include "TW_BitmaskEnum.h"

enum BeAttackingFlag
{
	BAF_AVOID			=	(1 << 0),
	BAF_REBOUND			=	(1 << 1),
	BAF_FIRST			=	(1 << 2),
	BAF_SHARE			=	(1 << 3),
	BAF_NORMAL			=	(1 << 4),
	BAF_CANTAVOID		=	(1 << 5),
	BAF_REATTACK		=	(1 << 6),
	BAF_DIVE			=	(1 << 7),
	BAF_MULTIPLE		=	(1 << 8),
	BAF_MHBEIJI			=	(1 << 9),
	BAF_NODAMAGE		=	(1 << 10),
	BAF_LEECH			=	(1 << 11),
	BAF_SKILLBREAK		=	(1 << 12),
	BAF_SKILL			=	(1 << 13),
	BAF_BAOJI			=	(1 << 14),
	BAF_NOT_BAOJI		=	(1 << 15),
	BAF_HAS_BAOJI		=	(1 << 16),
	BAF_EFFECT			=	(1 << 17),
};
ENABLE_BITMASK_OPERATORS(BeAttackingFlag)

enum BeAttackType
{
	BAT_NONE,
	BAT_NORMAL,
	BAT_SKILL,
	BAT_ABS,
};

class TwUnit;

class BeAttackingAttr : public BeFlagObj
{
public:
	BeAttackingAttr()
	{
		eAttackType = BeAttackType::BAT_NONE;
		fDamage = 0;
		iPlayer = 0;
		fDamageDecPer = 0.0f;
		fBumpRadius = 0.0f;
		iSkillProperty = 0;
		bNeedCulculateArmor = false;
		iBumpCount = 0;
		bFirstAttack = true;
		iSkillTypeID = 0;
		iSkillLevel = 0;
		bIgnorePhysicResist = false;
		fLeech = 0.0f;
		iItemID = 0;
		iMissileModel = 0;
		iBeDamagedEffect = 0;
	}

	void InitFromAttr(BeAttackingAttr* kAttr)
	{
		kAttacker = kAttr->kAttacker;
		eAttackType = kAttr->eAttackType;
		fDamage = kAttr->fDamage;
		iPlayer = kAttr->iPlayer;
		fDamageDecPer = kAttr->fDamageDecPer;
		fBumpRadius = kAttr->fBumpRadius;
		iSkillProperty = kAttr->iSkillProperty;
		bNeedCulculateArmor = kAttr->bNeedCulculateArmor;
		iBumpCount = kAttr->iBumpCount;
		bFirstAttack = kAttr->bFirstAttack;
		iSkillTypeID = kAttr->iSkillTypeID;
		iSkillLevel = kAttr->iSkillLevel;
		bIgnorePhysicResist = kAttr->bIgnorePhysicResist;
		fLeech = kAttr->fLeech;
		iItemID = kAttr->iItemID;
		iMissileModel = kAttr->iMissileModel;
		iBeDamagedEffect = kAttr->iBeDamagedEffect;
		m_iFlag = kAttr->m_iFlag;
	}

	inline void AddLeech(float fValue)
	{
		fLeech += fValue;
	}

	BeAttackType				eAttackType;
	float						fDamage;
	std::shared_ptr<TwUnit>		kAttacker;
	int							iPlayer;
	int							iBumpCount;
	float						fDamageDecPer;
	float						fBumpRadius;
	int							iSkillProperty;
	bool						bNeedCulculateArmor;
	bool						bFirstAttack;
	int							iSkillTypeID;
	int							iSkillLevel;
	bool						bIgnorePhysicResist;
	float						fLeech;
	int							iItemID;
	TwPos2						attackPos;
	int							iMissileModel;
	int							iBeDamagedEffect;
};
