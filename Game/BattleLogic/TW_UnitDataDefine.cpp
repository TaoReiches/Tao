/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_UnitDataDefine.h"
#include "TW_UnitLearnSkillData.h"

BeUnitData::BeUnitData()
{
	Player = -1;
	iControl = 0;
	iCurExp = 0;
	iLevel = 0;
	fHP = 0.0f;
	fMP = 0.0f;
	fMaxHP = 1.0f;
	fRegenHP = 0.0f;
	fMaxMP = 1.0f;
	fRegenMP = 0.0f;
	fDamage = 0.0f;

	fArmor = 0.0f;
	eAttackType = BAT_NONE;
	eMoveType = 0;
	fAttackRange = 0.0f;
	fMoveSpeed = 0.0f;
	iAttackCD = 0;
	iAttackDamagePt = 0;
	iAttackBackPt = 0;
	iAttackSpeedAdd = 0;
	fOrgArmor = 0.0f;
	fOrgMagicArmor = 0.0f;
	fOrgMaxHP = 1.0f;
	fOrgRegenHP = 0.0f;
	fOrgMaxMP = 1.0f;
	fOrgRegenMP = 0.0f;
	fOrgDamage = 0.0f;

	fOrgMoveSpeed = 0.0f;
	iOrgAttackCD = 0;
	fMissileSpeed = 0.0f;
	fBaseArmor = 0.0f;
	fBaseMaxHP = 1.0f;
	fBaseRegenHP = 0.0f;
	fBaseMaxMP = 1.0f;
	fBaseRegenMP = 0.0f;
	fBaseDamage = 0.0f;

	fBaseMoveSpeed = 0.0f;

	fAddDamage = 0.0f;
	fAddArmor = 0.0f;
	fAddArmorPer = 0.0f;
	fAddMagicArmor = 0.0f;
	eActionType = BUA_STAND;
	iActionCurTime = 0;
	iActionAllTime = 0;
	iAttackElapseTime = 0;

	fPosX = 0.0f;
	fPosY = 0.0f;
	fPosZ = 0.0f;
	fFace = 0.0f;
	fPitch = 0.0f;
	fScale = 1.0f;

	fBeDamagePerOrg = 1.0f;
	fDamagePerOrg = 1.0f;
	fBeDamagePer = 1.0f;
	fDamagePer = 1.0f;
	iAttackingUnitID = 0;
	iUnitCurLiveTime = 0;
	iUnitAllLiveTime = 0;
	uiUnitReliveTime = 0;
	dwUnitCreateTime = 0;

	pkRes = 0;
	akLearnSkill = 0;
	iSkillPointSum = 0;
	iUsedSkillPoint = 0;
	iTypeID = 0;
	iUnitProperty = 0;

	fOrgSkillDamagePer = 0.0f;
	fSkillDamagePer = 0.0f;
	fSkillBlastPer = 0.0f;

	fMagicArmor = 0.0f;
	fDecAntiMagic = 0.0f;
	fLeech = 0.0f;
	fDecArmor = 0.0f;
	fDecMPCost = 0.0f;
	fDecCDTime = 0.0f;
	fPerCDTime = 0.0f;

	fEnmityPoint = 0.0f;
	fPerDamageReduce = 0.0f;

	iOrgPlayer = -1;
	fArmorPass = 0.0f;
	fMagicDamage = 0.0f;
	fEvadeRate = 0.0f;

	fDecDamage = 0.0f;
	fBaoJi = 0.0f;
	fMagicLeech = 0.0f;
	fToughness = 0.0f;
	fTreatment = 0.0f;
	fBaoJiDamagePer = 0.0f;
	fBaoJiDecDamage = 0.0f;
}
BeUnitData::~BeUnitData()
{
}
