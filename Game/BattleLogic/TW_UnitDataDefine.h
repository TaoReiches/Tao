#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <vector>
#include "TW_MemoryPool.h"
#include "TW_AttackAttr.h"

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

class BeSkill;
struct UnitTable;
class BeLearnSkillData;

class BeUnitData
{
public:
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
	unsigned int		uiUnitReliveTime;

	std::shared_ptr<const UnitTable> pkRes;
	std::vector<std::shared_ptr<BeSkill>> apkUISkill;

	std::unique_ptr<BeLearnSkillData> akLearnSkill;
	int					iSkillPointSum;
	int					iUsedSkillPoint;

	int					iTypeID;

	int					iUnitProperty;
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

public:
	BeUnitData();
	~BeUnitData();

private:
	BeUnitData(const BeUnitData& other);
	BeUnitData& operator = (const BeUnitData& other);
};
