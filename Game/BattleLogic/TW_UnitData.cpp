/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_UnitDefine.h"
#include "TW_Unit.h"

BeUnitData::BeUnitData()
{
	iPlayer = -1;
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
	memset(apkUISkill, 0, sizeof(apkUISkill));
	akLearnSkill = 0;
	iSkillPointSum = 0;
	iUsedSkillPoint = 0;
	eMorphType = BUMT_CHANGE_UNIT;
	iTypeID = 0;
	iUnitProperty = 0;
	iVisionRaius = 0;

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

void BeUnitData::SaveData(BeUnitMorphType eType, BeUnitData& other) const
{
	switch (eType)
	{
	case BUMT_CHANGE_UNIT:
	{
		break;
	}
	case BUMT_SKILL_MORPH:
	case BUMT_CHANGESKILL:
	{
		other.akLearnSkill = akLearnSkill;
		break;
	}
	case BUMT_CHANGE_MODEL:
	case BUMT_CHANGE_MODELSKILL:
	case BUMT_SKILL_WOODOO:
	case BUMT_SKILL_UNIT:
	{
		other.fMaxHP = fMaxHP;
		other.fRegenHP = fRegenHP;
		other.fMaxMP = fMaxMP;
		other.fRegenMP = fRegenMP;
		other.fDamage = fDamage;

		other.fArmor = fArmor;

		if (eType != BUMT_CHANGE_MODELSKILL)
		{
			other.eAttackType = eAttackType;
			other.fAttackRange = fAttackRange;
			other.eMoveType = eMoveType;
		}

		other.iAttackCD = iAttackCD;
		other.iAttackDamagePt = iAttackDamagePt;
		other.iAttackBackPt = iAttackBackPt;
		other.iAttackSpeedAdd = iAttackSpeedAdd;

		other.fOrgArmor = fOrgArmor;
		other.fOrgMagicArmor = fOrgMagicArmor;
		other.fOrgMaxHP = fOrgMaxHP;
		other.fOrgRegenHP = fOrgRegenHP;
		other.fOrgMaxMP = fOrgMaxMP;
		other.fOrgRegenMP = fOrgRegenMP;
		other.fOrgDamage = fOrgDamage;

		other.iOrgAttackCD = iOrgAttackCD;

		other.fBaseArmor = fBaseArmor;
		other.fBaseMaxHP = fBaseMaxHP;
		other.fBaseRegenHP = fBaseRegenHP;
		other.fBaseMaxMP = fBaseMaxMP;
		other.fBaseRegenMP = fBaseRegenMP;
		other.fBaseDamage = fBaseDamage;

		other.fAddDamage = fAddDamage;
		other.fAddArmor = fAddArmor;
		other.fAddArmorPer = fAddArmorPer;
		other.fAddMagicArmor = fAddMagicArmor;
		other.fAddMagicArmorPer = fAddMagicArmorPer;

		other.fBeDamagePerOrg = fBeDamagePerOrg;
		other.fDamagePerOrg = fDamagePerOrg;

		other.fBeDamagePer = fBeDamagePer;
		other.fDamagePer = fDamagePer;

		break;
	}
	break;
	default:break;
	}

	other.fHP = other.fMaxHP * fHP / fMaxHP;
	other.fMP = (fMaxMP > 0.0f) ? other.fMaxMP * fMP / fMaxMP : 0.0f;

	other.eActionType = eActionType;
	other.iActionCurTime = iActionCurTime;
	other.iAttackElapseTime = iAttackElapseTime;

	other.fPosX = fPosX;
	other.fPosY = fPosY;
	other.fPosZ = fPosZ;
	other.fFace = fFace;
	other.fPitch = fPitch;

	other.fOrgSkillDamagePer = fOrgSkillDamagePer;
	other.fSkillDamagePer = fSkillDamagePer;
	other.fSkillBlastPer = fSkillBlastPer;
}
void BeUnitData::LoadData(BeUnitMorphType eType, const BeUnitData& other)
{
	other.SaveData(eType, *this);
}
#define ORG_REGEN_HP (gMain.IsNight() ? (m_pkCurData->pkRes)->fOrgNightRegenHP : (m_pkCurData->pkRes)->fOrgDayRegenHP)

void BeUnit::OnInitAttribute(bool bCurrent, bool bNeedRecordChange)
{
	BeUnitData* pkOrgData = m_pkBackData;
	BeUnitData* pkData = 0;
	if (bCurrent)
	{
		pkData = m_pkCurData;
	}
	else
	{
		pkData = m_pkBackData;
	}

	pkData->iTypeID = (pkData->pkRes)->uiUnitTypeID;
	pkData->fOrgMaxHP = (pkData->pkRes)->fOrgMaxHP;
	pkData->fOrgRegenHP = (pkData->pkRes)->fOrgDayRegenHP;
	pkData->fOrgMaxMP = (pkData->pkRes)->fOrgMaxMP;
	pkData->fOrgRegenMP = (pkData->pkRes)->fOrgRegenMP;
	pkData->fOrgDamage = (pkData->pkRes)->fOrgDamage;
	pkData->fOrgMagicDamage = pkData->pkRes->fOrgMagic;
	pkData->fOrgArmor = (pkData->pkRes)->fOrgArmor;
	pkData->fOrgMagicArmor = (pkData->pkRes)->fOrgAntiMagic;
	pkData->fOrgMoveSpeed = (pkData->pkRes)->fOrgMoveSpeed;
	pkData->iOrgAttackCD = (pkData->pkRes)->fAttackBackPt;
	pkData->fBaseMaxHP = (pkData->pkRes)->fOrgMaxHP;
	pkData->fBaseRegenHP = (pkData->pkRes)->fOrgDayRegenHP;
	pkData->fBaseMaxMP = (pkData->pkRes)->fOrgMaxMP;
	pkData->fBaseRegenMP = (pkData->pkRes)->fOrgRegenMP;
	pkData->fBaseDamage = (pkData->pkRes)->fOrgDamage;
	pkData->fMagicDamage = pkData->pkRes->fOrgMagic;
	pkData->fBaseArmor = (pkData->pkRes)->fOrgArmor;
	pkData->fBaseMoveSpeed = (pkData->pkRes)->fOrgMoveSpeed;
	pkData->iMissileModel = (pkData->pkRes)->iMissileModel;
	pkData->eMoveType = 1;
	pkData->fMissileSpeed = (pkData->pkRes)->fMissileSpeed;
	pkData->fMaxHP = pkData->fBaseMaxHP;
	pkData->fRegenHP = pkData->fBaseRegenHP;
	pkData->fMaxMP = pkData->fBaseMaxMP;
	pkData->fRegenMP = pkData->fBaseRegenMP;
	pkData->fDamage = pkData->fBaseDamage;
	pkData->fArmor = pkData->fBaseArmor;
	pkData->fAttackRange = (pkData->pkRes)->fAttackRange;
	pkData->fMoveSpeed = pkData->fBaseMoveSpeed;
	pkData->iAttackCD = pkData->iOrgAttackCD;
	pkData->iVisionRaius = pkData->pkRes->iFogRadius >> 7;
	pkData->fHP = pkData->fMaxHP;
	pkData->fMP = pkData->fMaxMP;
	pkData->fScale = 1.0f;
	pkData->iActionAllTime = 1000;
	pkData->fBeDamagePerOrg = 1.0f;
	pkData->fDamagePerOrg = 1.0f;
	pkData->fDamagePer = 1.0f;
	pkData->fBeDamagePer = 1.0f;

	pkData->fMagicArmor = 0.0f;
	pkData->fDecAntiMagic = 0.0f;
	pkData->fLeech = 0.0f;
	pkData->fDecArmor = 0.0f;
	pkData->fDecMPCost = 0.0f;
	pkData->fDecCDTime = 0.0f;
	pkData->fPerCDTime = 0.0f;

	pkData->iAttackElapseTime = pkData->iAttackCD;

	SetBackTypeID(pkData->iTypeID);
}

void BeUnit::OnPropertyUpdate(int iLevel)
{
	BeUnitData* pkOrgData = m_pkBackData;
	BeUnitData* pkCurData = m_pkCurData;
	if (iLevel == 1 && pkCurData->fHP <= 1.0f)
	{
		pkCurData->fHP = 1.0f;
	}

	float fCalLevel = (float)(iLevel);

	float	fAddMaxHp = (pkOrgData->pkRes)->fHpLevelAdd * fCalLevel;
	float	fAddMaxMp = (pkOrgData->pkRes)->fMpLevelAdd * fCalLevel;
	float	fAddDamage = (pkOrgData->pkRes)->fOrgAddUpDamage * fCalLevel;
	float	fAddMagic = (pkOrgData->pkRes)->fMagicAddUp * fCalLevel;
	float	fAddAmor = (pkOrgData->pkRes)->fAmorAddUp * fCalLevel;
	float	fAddMagicArmor = (pkOrgData->pkRes)->fMagicAmorAddUp * fCalLevel;
	float	fAddRegenHp = (pkOrgData->pkRes)->fHpRegenAddUp * fCalLevel;
	float	fAddRegenMp = (pkOrgData->pkRes)->fMpRegenAddUp * fCalLevel;

	if (gMain.IsPveMap() && GetPlayer() >= 0 && GetPlayer() < MAX_PLAYERS)
	{
		int MoraleValue = gMain.GetPlayerMoraleValue(GetPlayer());

		fAddMaxHp += (pkOrgData->pkRes)->fHpLevelAdd * (MoraleValue / 2);
		fAddMaxMp += (pkOrgData->pkRes)->fMpLevelAdd * (MoraleValue / 2);
		fAddDamage += (pkOrgData->pkRes)->fOrgAddUpDamage * (MoraleValue / 2);
		if ((pkOrgData->pkRes)->fZMapMagicAddUp > 0)
		{
			fAddMagic += (pkOrgData->pkRes)->fZMapMagicAddUp * (MoraleValue / 2);
		}
		fAddAmor += (pkOrgData->pkRes)->fAmorAddUp * (MoraleValue / 2);
		fAddMagicArmor += (pkOrgData->pkRes)->fMagicAmorAddUp * (MoraleValue / 2);
		fAddRegenHp += (pkOrgData->pkRes)->fHpRegenAddUp * (MoraleValue / 2);
		fAddRegenMp += (pkOrgData->pkRes)->fMpRegenAddUp * (MoraleValue / 2);
	}

	int		iPlayer = GetPlayer();
	if (IsHero() && iPlayer >= 0 && iPlayer < MAX_ACTIVEPLAYERS)
	{
		float	fEquipAddArmor = gMain.GetEquipAddArmor(iPlayer) * fCalLevel;
		float	fEquipMagicDamage = gMain.GetEquipAddMagicDamage(iPlayer) * fCalLevel;
		float	fEquipMagicArmor = gMain.GetEquipAddMagicArmor(iPlayer) * fCalLevel;

		fAddAmor += fEquipAddArmor;
		fAddMagic += fEquipMagicDamage;
		fAddMagicArmor += fEquipMagicArmor;
	}

	pkCurData->fBaseMaxHP = pkCurData->fOrgMaxHP + fAddMaxHp;
	pkCurData->fBaseDamage = pkCurData->fOrgDamage + fAddDamage;
	pkCurData->fBaseArmor = pkCurData->fOrgArmor + fAddAmor;
	pkCurData->fOrgMagicArmor = (pkOrgData->pkRes)->fOrgAntiMagic + fAddMagicArmor;
	pkCurData->fOrgMagicDamage = pkCurData->pkRes->fOrgMagic + fAddMagic;
	pkCurData->fBaseRegenHP = (pkOrgData->pkRes)->fOrgDayRegenHP + fAddRegenHp;

	if (HasProperty(UNIT_PROPERTY_HUDUNBAR) || HasProperty(UNIT_PROPERTY_NUQIBAR) || HasProperty(UNIT_PROPERTY_POWERBAR) || HasProperty(UNIT_PROPERTY_BULLETBAR) || HasProperty(UNIT_PROPERTY_NOBAR))
	{
	}
	else
	{
		pkCurData->fBaseRegenMP = (pkOrgData->pkRes)->fOrgRegenMP + fAddRegenMp;
		pkCurData->fBaseMaxMP = pkCurData->fOrgMaxMP + fAddMaxMp;
	}

	UpdateAttribute(true);
}

void BeUnit::SetCurExp(int iCurExp)
{
	SetShareUnitChangeFlag(BSUDCF_EXP);
	int iUnitLevel = m_pkCurData->iLevel;
	for (int iIndex = iUnitLevel; iIndex < iHeroMaxLevel; iIndex++)
	{
		int	iLevelUpNeedExp = BeFormula::GetHeroLevelUpNeedExp(iUnitLevel + 1);
		if (iLevelUpNeedExp < iCurExp)
		{
			AddLevel(1);
			iCurExp -= iLevelUpNeedExp;
			iUnitLevel++;
		}
		else
		{
			break;
		}
	}
	m_pkCurData->iCurExp = iCurExp;
}

void BeUnit::SetLevel(int iLevel, bool bNeedRecordChange)
{
	if (iLevel <= 0)
	{
		return;
	}
	if (iLevel > iHeroMaxLevel)
	{
		iLevel = iHeroMaxLevel;
	}
	int iPreLevel = m_pkCurData->iLevel;
	m_pkCurData->iLevel = iLevel;

	OnPropertyUpdate(iLevel);

	SetHP(m_pkCurData->fMaxHP);
	SetMP(m_pkCurData->fMaxMP);

	if (HasProperty(UNIT_PROPERTY_NUQIBAR))
	{
		SetMP(0.0f, true);
	}

	if (iPreLevel != m_pkCurData->iLevel)
	{
		SetShareUnitChangeFlag(BSUDCF_UNITLEVEL);
		SetTabInfoFlag(BTCF_LEVEL);
	}
}

void BeUnit::UpdateAttribute(bool bUpdateNormal)
{
	BeUnitData* pkOrgData = m_pkBackData;
	BeUnitData* pkCurData = m_pkCurData;
	float afChange[NAT_MAX_NUM];
	float afChangeTemp[BCT_NUM][NAT_MAX_NUM][2];

	const int iCommon1Size = 10;
	float akCommonInfo[iCommon1Size] = { 0.f };
	akCommonInfo[0] = pkCurData->fHP;
	akCommonInfo[1] = pkCurData->fMP;
	akCommonInfo[2] = pkCurData->fMaxHP;
	akCommonInfo[3] = pkCurData->fMaxMP;
	akCommonInfo[4] = pkCurData->fMoveSpeed;
	akCommonInfo[5] = pkCurData->fRegenHP;
	akCommonInfo[6] = pkCurData->fRegenMP;
	akCommonInfo[7] = pkCurData->iAttackCD;

	const int iSelfSize = 1;
	float akSelfData[iSelfSize] = { 0.f };
	akSelfData[0] = pkCurData->fDecCDTime;
	float temp1, temp2 = 0.f;

	int iOldCarryFlag = m_iCarryFlag;
	int iOldImmunityFlag = m_iImmunityFlag;
	int iOldInvisByGroup = m_iNotInvisByGroup;

	m_apkCarry.clear();

	iOldCarryFlag = m_iCarryFlag;
	m_iCarryFlag = 0;
	iOldImmunityFlag = m_iImmunityFlag;
	m_iImmunityFlag = 0;
	iOldInvisByGroup = m_iNotInvisByGroup;
	m_iNotInvisByGroup = 0;

	UpdateValidItem();
	UpdateValidBuffer();
	UpdateValidSkill();

	int iMaskCarryFlag = m_iCarryFlag ^ iOldCarryFlag;

	if (!bUpdateNormal)
	{
		if (m_iCarryFlag != iOldCarryFlag)
		{
			SetShareUnitChangeFlag(BSUDCF_CARRYFLAG);
		}
		return;
	}

	memset(afChange, 0, sizeof(afChange));
	float afAntiMagic[BCT_NUM];
	memset(afAntiMagic, 0, sizeof(afAntiMagic));

	float afDecAntiMagic[BCT_NUM];
	memset(afDecAntiMagic, 0, sizeof(afDecAntiMagic));

	memset(afChangeTemp, 0, sizeof(afChangeTemp));
	for (std::vector<BeCarry*>::iterator itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
	{
		BeCarry* pkCarry = *itr;
		if (IsGhost() || IsDividMan())
		{
			pkCarry->ApplyNormalAttr(afChangeTemp, true, m_iImmunityFlag);
		}
		else
		{
			pkCarry->ApplyNormalAttr(afChangeTemp, false, m_iImmunityFlag);
			float fAntiRate = pkCarry->GetAttackedAntiMagic();
			if (fAntiRate > 0.0f && afAntiMagic[pkCarry->GetType()] <= fAntiRate)
			{
				afAntiMagic[pkCarry->GetType()] = fAntiRate;
			}
			else if (fAntiRate < 0.0f)
			{
				afDecAntiMagic[pkCarry->GetType()] += fAntiRate;
			}
		}
	}
	for (int i = 0; i < NAT_MAX_NUM; ++i)
	{
		switch (i)
		{
		case NAT_PER_SKILLBLAST:
		{
			for (int j = 0; j < BCT_NUM; ++j)
			{
				afChange[i] = 1.0f - (1.0f - afChange[i]) * (1.0f - (afChangeTemp[j][i][0] + afChangeTemp[j][i][1]));
			}
			break;
		}
		case NAT_PER_BAOJI:
		{
			afChange[i] = 1;

			for (int j = 0; j < BCT_NUM; ++j)
			{
				if (afChangeTemp[j][i][0] > 0)
				{
					afChange[i] *= afChangeTemp[j][i][0];
				}
			}
			break;
		}
		case NAT_PER_MOVE_SPEED:
		{
			afChange[i] = 1;
			temp1 = 0;
			temp2 = 1;

			for (int j = 0; j < BCT_NUM; ++j)
			{
				if (afChangeTemp[j][i][0] > 0) {

					temp1 += afChangeTemp[j][i][0];

				}

				if (afChangeTemp[j][i][1] > 0) {

					temp2 *= afChangeTemp[j][i][1];
				}
			}

			afChange[i] = temp1 - (1 - temp2);
			break;
		}
		case NAT_TOUGHNESS:
		{
			afChange[i] = 1;

			for (int j = 0; j < BCT_NUM; ++j)
			{
				if (afChangeTemp[j][i][0] > 0)
				{
					afChange[i] *= (1 - afChangeTemp[j][i][0]);
				}
			}

			afChange[i] = 1 - afChange[i];
			break;
		}
		case NAT_DEC_CDTIME:
		case NAT_DEC_MPCOST:
		case NAT_PER_CDTIME:
		default:
		{
			for (int j = 0; j < BCT_NUM; ++j)
			{
				afChange[i] += afChangeTemp[j][i][0] + afChangeTemp[j][i][1];
			}
			break;
		}
		}
	}
	pkCurData->fMagicArmor = (pkCurData->fOrgMagicArmor + afChange[NAT_MAGIC_ARMOR]) * (1.0f + afChange[NAT_PER_MAGIC_ARMOR]);
	pkCurData->fDecAntiMagic = afChange[NAT_DEC_MAGIC_RESISTANCE];

	pkCurData->fAddArmorPer = afChange[NAT_PER_ARMOR];
	pkCurData->fAddArmor = afChange[NAT_ABS_ARMOR];
	pkCurData->fAddDamage = afChange[NAT_ABS_DAMAGE] + afChange[NAT_PER_DAMAGE_BASE] * (pkCurData->fBaseDamage);
	if (pkCurData->fAddDamage < -pkCurData->fBaseDamage)
	{
		pkCurData->fAddDamage = -pkCurData->fBaseDamage;
	}
	pkCurData->fDamagePer = (1.0f + afChange[NAT_PER_DEMAGE]) * pkCurData->fDamagePerOrg;
	pkCurData->fBeDamagePer = (1.0f + afChange[NAT_PER_BEDEMAGED]) * pkCurData->fBeDamagePerOrg;
	pkCurData->fMagicDamage = (pkOrgData->fOrgMagicDamage + afChange[NAT_ABS_MAGIC]) * (1.0f + afChange[NAT_PER_MAGIC]);
	pkCurData->fSkillBlastPer = (afChange[NAT_PER_SKILLBLAST]);
	pkCurData->fSkillDamagePer = 1.0f + afChange[NAT_PER_DAMAGE_SKILL];

	pkCurData->fArmor = (pkCurData->fBaseArmor + pkCurData->fAddArmor) * (1 + pkCurData->fAddArmorPer);

	pkCurData->fAttackRange = m_pkBackData->pkRes->fAttackRange + afChange[NAT_ABS_ATTACKRANGE];
	pkCurData->fMaxHP = (pkCurData->fBaseMaxHP * (1.0f + afChange[NAT_PER_BASE_MAXHP]) + afChange[NAT_ABS_MAX_HP]);

	pkCurData->fDecDamage = afChange[NAT_ABS_DECDAMAGE];
	pkCurData->fRegenHP = pkCurData->fBaseRegenHP + afChange[NAT_ABS_REGEN_HP] + pkCurData->fBaseRegenHP * afChange[NAT_PER_REGEN_HP];
	if (HasProperty(UNIT_PROPERTY_HUDUNBAR) || HasProperty(UNIT_PROPERTY_POWERBAR) || HasProperty(UNIT_PROPERTY_NUQIBAR) || HasProperty(UNIT_PROPERTY_BULLETBAR) || HasProperty(UNIT_PROPERTY_NOBAR))
	{
		pkCurData->fRegenMP = 0.0f;
	}
	else
	{
		pkCurData->fMaxMP = pkCurData->fBaseMaxMP + afChange[NAT_ABS_MAX_MP];
		pkCurData->fRegenMP = pkCurData->fBaseRegenMP + afChange[NAT_ABS_REGEN_MP] + pkCurData->fBaseRegenMP * afChange[NAT_PER_REGEN_MP];
	}

	if (m_fSpeedFixup >= 0.0f)
	{
		pkCurData->fMoveSpeed = m_fSpeedFixup;
	}
	else
	{
		pkCurData->fMoveSpeed = (pkCurData->fBaseMoveSpeed + afChange[NAT_BASE_MOVE_SPEED]) * (1.0f + afChange[NAT_PER_MOVE_SPEED]) + afChange[NAT_ABS_MOVE_SPEED];
	}
	float	fTempMoveSpeed = pkCurData->fMoveSpeed;
	if (fTempMoveSpeed > 490.0f)
	{
		pkCurData->fMoveSpeed = (fTempMoveSpeed * 0.5f) + 230.0f;
	}
	else if (fTempMoveSpeed > 415.0f && fTempMoveSpeed <= 490.0f)
	{
		pkCurData->fMoveSpeed = (fTempMoveSpeed * 0.8f) + 83;
	}
	else if (fTempMoveSpeed < 220.0f)
	{
		pkCurData->fMoveSpeed = (fTempMoveSpeed * 0.5f) + 110.0f;
	}
	if (pkCurData->fMoveSpeed > 900.0f)
	{
		pkCurData->fMoveSpeed = 900.0f;
	}
	if (pkCurData->fMoveSpeed < 140.0f)
	{
		pkCurData->fMoveSpeed = 140.0f;
	}
	pkCurData->fLeech = afChange[NAT_LEECH];
	pkCurData->fMagicLeech = afChange[NAT_MAGIC_LEECH];
	pkCurData->fDecArmor = afChange[NAT_DEC_ARMOR];
	pkCurData->fPerDecArmor = min(afChange[NAT_PER_DEC_ARMOR], 1.0f);
	pkCurData->fPerDecMagicArmor = afChange[NAT_PER_FASHUCHUANTOU];
	pkCurData->fToughness = afChange[NAT_TOUGHNESS];
	pkCurData->fDecMPCost = afChange[NAT_DEC_MPCOST];
	pkCurData->fBaoJi = afChange[NAT_BAOJI];
	if (HasProperty(UNIT_PROPERTY_NOTADDBAOJI)) {
		pkCurData->fBaoJi = 0.0f;
	}
	if (pkCurData->fBaoJi > 1.0f)
	{
		pkCurData->fBaoJi = 1.0f;
	}
	pkCurData->fBaoJiDamagePer = afChange[NAT_PER_BAOJI];
	pkCurData->fBaoJiDecDamage = afChange[NAT_PER_DEC_BAOJI];

	if (gMain.IsZhanChangMap())
	{
		pkCurData->fDecCDTime = min(afChange[NAT_DEC_CDTIME], 0.6f + afChange[NAT_DEC_CDTIME_MAX]);
	}
	else
	{
		pkCurData->fDecCDTime = min(afChange[NAT_DEC_CDTIME], 0.4f + afChange[NAT_DEC_CDTIME_MAX]);
	}

	pkCurData->fPerCDTime = min(afChange[NAT_PER_CDTIME], 0.9f);
	pkCurData->fEnmityPoint = afChange[NAT_ABS_ENMITY_POINT];
	pkCurData->fPerDamageReduce = afChange[NAT_PER_DAMAGE_REDUCE];
	pkCurData->fEvadeRate = afChange[NAT_PER_AVOIDPHYSIC];
	pkCurData->fTreatment = afChange[NAT_PER_ADDHP];

	float fPrimaryAddDamage = 0.0f;
	pkCurData->fRegenHP += (pkCurData->fMaxHP * afChange[NAT_PER_REGEN_MAXHP]);

	pkCurData->fAddDamage += afChange[NAT_PER_DAMAGE_ADD] * (pkCurData->fAddDamage + pkCurData->fBaseDamage);
	pkCurData->fDamage = pkCurData->fBaseDamage + pkCurData->fAddDamage;

	if (m_fPreMaxHP > 0.0f && (pkCurData->fMaxHP != m_fPreMaxHP))
	{
		if (pkCurData->fMaxHP > m_fPreMaxHP) {

			float fHPTemp = ((pkCurData->fMaxHP - m_fPreMaxHP) * (1.0f - (m_fPreMaxHP - pkCurData->fHP) / m_fPreMaxHP));
			pkCurData->fHP += fHPTemp;
		}
		else {

			if (pkCurData->fHP > pkCurData->fMaxHP) {
				pkCurData->fHP = pkCurData->fMaxHP;
			}
		}

		if (pkCurData->fHP <= 0.0f)
		{
			pkCurData->fHP = 1.0f;
		}
	}
	m_fPreMaxHP = pkCurData->fMaxHP;

	if (m_fPreMaxMP > 0.0f && (pkCurData->fMaxMP != m_fPreMaxMP))
	{
		float fMPTemp = ((pkCurData->fMaxMP - m_fPreMaxMP) * (1.0f - (m_fPreMaxMP - pkCurData->fMP) / m_fPreMaxMP));
		pkCurData->fMP += fMPTemp;
		if (pkCurData->fMP <= 0.0f)
		{
			pkCurData->fMP = 1.0f;
		}
	}
	m_fPreMaxMP = pkCurData->fMaxMP;

	float fDivAttackSpeed = afChange[NAT_PER_ATTACK_SPEED];
	if (HasProperty(UNIT_PROPERTY_NOTADDATTACKCD)) {
		fDivAttackSpeed = 0.0f;
	}
	pkCurData->iAttackCD = m_pkCurData->iOrgAttackCD;

	float	fBaseNum = 1000.0f / m_pkCurData->iOrgAttackCD;
	float	fAddNum = 1000.0f / pkCurData->pkRes->fAttackBackPt * fDivAttackSpeed;
	float	fLevelNum = 1000.0f / pkCurData->pkRes->fAttackBackPt * 0.01f * pkCurData->pkRes->fAttackSpeedAddUp * (GetLevel() - 1);
	pkCurData->iAttackCD = 1000.0f / (fBaseNum + fAddNum + fLevelNum);

	if (pkCurData->iAttackCD < 400)
	{
		pkCurData->iAttackCD = 400;
	}

	pkCurData->iAttackDamagePt = (int)((float)(pkCurData->pkRes)->fAttackDamagePt * (pkCurData->iAttackCD / pkCurData->pkRes->fAttackBackPt));
	if (pkCurData->iAttackDamagePt > pkCurData->iAttackCD)
	{
		pkCurData->iAttackDamagePt = pkCurData->iAttackCD;
	}
	pkCurData->iAttackBackPt = pkCurData->iAttackCD - pkCurData->iAttackDamagePt;

	if (akCommonInfo[0] != pkCurData->fHP)
	{
		SetShareUnitChangeFlag(BSUDCF_CURHP);
	}
	if (akCommonInfo[1] != pkCurData->fMP)
	{
		SetShareUnitChangeFlag(BSUDCF_CURMP);
	}
	if (akCommonInfo[2] != pkCurData->fMaxHP)
	{
		SetShareUnitChangeFlag(BSUDCF_MAXHP);
	}
	if (akCommonInfo[3] != pkCurData->fMaxMP)
	{
		SetShareUnitChangeFlag(BSUDCF_MAXMP);
	}
	if (akCommonInfo[4] != pkCurData->fMoveSpeed)
	{
		SetShareUnitChangeFlag(BSUDCF_MOVESPEED);
	}
	if (akCommonInfo[5] != pkCurData->fRegenHP)
	{
		SetShareUnitChangeFlag(BSUDCF_REGENHP);
	}
	if (akCommonInfo[6] != pkCurData->fRegenMP)
	{
		SetShareUnitChangeFlag(BSUDCF_REGENMP);
	}
	if ((int)akCommonInfo[7] != pkCurData->iAttackCD)
	{
		SetShareUnitChangeFlag(BSUDCF_ATTACKCD);
	}

	if (m_iCarryFlag != iOldCarryFlag)
	{
		SetShareUnitChangeFlag(BSUDCF_CARRYFLAG);
	}
}

BeLearnSkillData* BeUnit::GetLearnSkillData(int iPos)
{
	if (!m_pkBackData || !m_pkBackData->akLearnSkill)
	{
		return NULL;
	}

	if (iPos < 0 || iPos >= MAX_LEARN_SKILLS)
	{
		return NULL;
	}

	return &(m_pkBackData->akLearnSkill)[iPos];
}

BeLearnSkillData* BeUnit::GetAllLearnSkillData()
{
	return m_pkBackData->akLearnSkill;
}

int	BeUnit::GetCanLearnLevel(int iTypeID, int iLevel)
{
	const	SkillTable* pkData = GetResSkill(iTypeID);
	if (!pkData || iLevel > pkData->iSkillMaxLevel || iLevel > 5 || iLevel < 1)
	{
		return 0;
	}

	return pkData->iLearnLevel[iLevel - 1];
}

int	BeUnit::GetSkillPointSum(void)
{
	return (m_pkBackData->iSkillPointSum);
}

void BeUnit::SetSkillPointSum(int iSkillPoint)
{
	(m_pkBackData->iSkillPointSum) = iSkillPoint;
}

bool BeUnit::LearnSkill(int iSkillPos)
{
	if (iSkillPos < 0 || iSkillPos >= MAX_LEARN_SKILLS || !m_pkCurData || !(m_pkCurData->akLearnSkill))
	{
		return false;
	}

	BeLearnSkillData& rkLearnData = (m_pkCurData->akLearnSkill)[iSkillPos];
	int		iSkillTypeID = rkLearnData.iTypeID;
	BeSkill* pkHasLearnSkill = GetSkillByPos(iSkillPos);
	if (pkHasLearnSkill)
	{
		iSkillTypeID = pkHasLearnSkill->GetTypeID();
	}
	const SkillTable* pkRes = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID);

	if (!pkRes || !m_pkBackData || GetLevel() - (m_pkBackData->iUsedSkillPoint) <= 0)
	{
		return false;
	}

	BeSkill* pkSkill = GetSkill(iSkillTypeID);
	if (!pkSkill)
	{
		int iCanLevel = GetCanLearnLevel(iSkillTypeID, 1);
		if (GetLevel() < iCanLevel || iCanLevel == 0)
		{
			return false;
		}
		BeSkill* pkNewSkill = AddSkill(iSkillTypeID, 1);
		if (!pkNewSkill)
		{
			return false;
		}
		++rkLearnData.iLearnLevel;
		SetUsedSkillPoint(m_pkBackData->iUsedSkillPoint + 1);
	}
	else
	{
		int iLevel = GetSkillLevel(iSkillTypeID);
		int iCanLevel = GetCanLearnLevel(iSkillTypeID, GetSkillLevel(iSkillTypeID) + 1);
		if (iCanLevel == 0 || GetLevel() < iCanLevel || rkLearnData.iLearnLevel > pkSkill->GetMaxLevel())
		{
			return false;
		}

		pkSkill->SetLevel(iLevel + 1);
		UpdateAttribute(true);
		++rkLearnData.iLearnLevel;
		SetUsedSkillPoint(m_pkBackData->iUsedSkillPoint + 1);
	}

	return true;
}

void BeUnit::ParseHaloSkill(const BeSkill* pkSkill, int iItemID)
{
	if (!pkSkill)
	{
		return;
	}

	const SkillTable* pkSkillRes = pkSkill->GetResPtr();
	SeCalSkillLvlData	rkData;
	if (!GetSkillLvlData(rkData, pkSkill->GetTypeID()))
	{
		return;
	}

	UnitGroup kGroup;
	float fRadius = (float)rkData.GetSpellScope();
	if (fRadius < 99999.0f)
	{
		{
			gUnitMgr.GetAreaGroup(kGroup, GetPosX(), GetPosY(), fRadius, this, BCT_SPELL, pkSkill->GetTypeID());
		}
	}
	else
	{
		gUnitMgr.GetAllMapGroup(kGroup, this, BCT_SPELL, pkSkill->GetTypeID());
	}

	if (!kGroup.IsEmpty())
	{
		for (int i = 0; i < (int)kGroup.Size(); i++)
		{
			BeUnit* pkUnit = kGroup[i];
			if (!pkUnit || pkUnit->HasFlag(BUF_HIDE) || pkUnit->HasUnitCarryFlag(BUCF_NO_HALO_EFFECT))
			{
				continue;
			}

			bool bForceUpdate = false;
			BeBuffer* pkBuffer = pkUnit->GetBuffer(pkSkill->GetBufferID(), GetID());
			if (!pkBuffer || !pkBuffer->HasProperty(BUFFER_PROPERTY_CDWANCHENG))
			{
				bForceUpdate = true;
				pkBuffer = pkUnit->AddBufferBegin(pkSkill->GetBufferID(), pkSkill->GetLevel(), GetID());
				if (!pkBuffer)
				{
					continue;
				}

				pkBuffer->SetSkillRes(pkSkillRes);
				pkBuffer->SetSkillLvlData(&rkData);
				pkBuffer->SetSkillTypeID(pkSkill->GetTypeID());
				pkBuffer->SetUnitID(GetID());
				pkBuffer->SetHaloBuffer(true);
				pkBuffer->SetLevel(pkSkill->GetLevel());
				pkBuffer->SetCDTime(1000, bForceUpdate, true);
				pkBuffer->SetRemoveTime(gTime + 3000);

				int iSkillLevel = pkSkill->GetLevel();
				for (int j = 0; j < MAX_CONTENT_DATA; ++j)
				{
					for (int j = 0; j < MAX_CONTENT_DATA; j++)
					{
						int eType = 0;
						float fValue = 0.0f;
						switch (j)
						{
						case 0:
							eType = pkSkillRes->uiDataContentA;
							fValue = rkData.GetDataValue(VALUE_A);
							break;
						case 1:
							eType = pkSkillRes->uiDataContentB;
							fValue = rkData.GetDataValue(VALUE_B);
							break;
						case 2:
							eType = pkSkillRes->uiDataContentC;
							fValue = rkData.GetDataValue(VALUE_C);
							break;
						case 3:
							eType = pkSkillRes->uiDataContentD;
							fValue = rkData.GetDataValue(VALUE_D);
							break;
						case 4:
							eType = pkSkillRes->uiDataContentE;
							fValue = rkData.GetDataValue(VALUE_E);
							break;
						case 5:
							eType = pkSkillRes->uiDataContentF;
							fValue = rkData.GetDataValue(VALUE_F);
							break;
						case 6:
							break;
						default:
							break;
						}

						if (eType == 0)
						{
							continue;
						}

						if (eType >= NAT_ABS_ALL && eType < NAT_MAX_NUM)
						{
							pkBuffer->SetNormalAttrByData(eType, fValue);
						}
					}
				}

				pkUnit->AddBufferEnd(pkBuffer);
			}
		}
	}
}

void BeUnit::ClrCastMove(void)
{
	m_iMoveAllTime = 0;
	DelBuffer('BA19');

	ClrFlag(BUF_IGNOREUNITOBS);
	ClrOtherFlag(BUOF_WEIYI);
	SetShareUnitChangeFlag(BSUDCF_POSX);
	SetOtherFlag(BUOF_Clr_WeiYi);

	gMap.SetUnitPosition(this, GetPosX(), GetPosY(), 0.0f, 1000.0f, false, BGF_FIXED_OBS | BGF_UNIT, 0, true);
}

void BeUnit::SetCastMove(int iMaxTime, int iUnitID, float fTarPosX, float fTarPosY, int iSkillTypeID, BeUnitFlyType kType, int iAttackUnitID)
{
	if (iMaxTime <= 0)
	{
		return;
	}
	if (GetClass() == UNIT_CLASSTYPE_BOSS)
	{
		return;
	}
	if (HasUnitCarryFlag(BUCF_NO_WEIYI))
	{
		return;
	}

	if (gMain.IsZhanChangMap() && this->GetSecClass() == UNIT_SECCLASSTYPE_GUARD)
	{
		return;
	}

	BeUnit* pkAttacker = gUnitMgr.GetUnitByID(iAttackUnitID);

	BeBuffer* pkLiLiSiBuffer = GetLilithBuffer();

	if ((!pkAttacker || pkAttacker->GetCamp() != GetCamp())) {

		if (pkLiLiSiBuffer) {
			RemoveLilithBuffer(pkLiLiSiBuffer->GetID());
			return;
		}

		if (HasUnitCarryFlag(BUCF_IMMUNITY_CONTROL)) {
			return;
		}
	}

	m_iMoveToUnitID = iUnitID;
	m_fMoveTarPosX = fTarPosX;
	m_fMoveTarPosY = fTarPosY;
	m_iMoveAllTime = max(iMaxTime, 2 * iLogicFrameTime);

	gMap.DelUnitObstacle(this);

	BeBuffer* pkBuffer = AddBufferBegin('BA19', -1);
	if (pkBuffer)
	{
		pkBuffer->SetRemoveTime(gTime + m_iMoveAllTime);
		pkBuffer->SetCarryFlag(BUCF_CANNOTCONTROL | BUCF_DIZZY);
		pkBuffer->SetCarryFlag(BUCF_ISMOVING);

		AddBufferEnd(pkBuffer);
	}

	SetFlag(BUF_IGNOREUNITOBS);
	SetOtherFlag(BUOF_WEIYI);
	ClrOtherFlag(BUOF_Clr_WeiYi);

	BeShareUnitFlyData	kData;
	kData.iUnitID = GetID();
	kData.byFlyType = kType;
	kData.iFlyTime = iMaxTime;
	kData.sStartX = GetPosX();
	kData.sStartY = GetPosY();
	kData.sTarX = fTarPosX;
	kData.sTarY = fTarPosY;
	kData.iTargetID = iUnitID;
	kData.iSkillTypeID = iSkillTypeID;

	gMain.AddUnitFlyData(kData);
}

float	BeUnit::GetSkillAddValue(int iEnum, float fValue)
{
	float	fResult = 0.0f;
	switch (iEnum)
	{
	case SKILL_AADJ_MAXHP:
	{
		fResult = GetMaxHP() * fValue;
		break;
	}
	case SKILL_AADJ_CURHP:
	{
		fResult = GetHP() * fValue;
		break;
	}
	case SKILL_AADJ_MAXMP:
	{
		fResult = GetMaxMP() * fValue;
		break;
	}
	case SKILL_AADJ_CURMP:
	{
		fResult = GetMP() * fValue;
		break;
	}
	case SKILL_AADJ_ATTACK:
	{
		fResult = GetDamageNum() * fValue;
		break;
	}
	case SKILL_AADJ_FASHANG:
	{
		fResult = GetMagicDamage() * fValue;
		break;
	}
	case SKILL_AADJ_HEROLEVEL:
	{
		fResult = GetLevel() * fValue;
		break;
	}
	case SKILL_AADJ_MOVESPEED:
	{
		fResult = GetMoveSpeed() * fValue;
		break;
	}
	case SKILL_AADJ_ADDATTACK:
	{
		fResult = (GetDamageNum() - GetBaseDamage()) * fValue;
		break;
	}
	case SKILL_AADJ_ADDARMOR:
	{
		fResult = (GetArmor() - GetBaseArmor()) * fValue;
		break;
	}
	case SKILL_AADJ_ADDHP:
	{
		fResult = (GetMaxHP() - GetBaseMaxHP()) * fValue;
		break;
	}
	case SKILL_AADJ_ADDMP:
	{
		fResult = (GetMaxMP() - GetBaseMaxMP()) * fValue;
		break;
	}
	case SKILL_AADJ_ADDMOVESPEED:
	{
		fResult = (GetMoveSpeed() - GetBaseMoveSpeed()) * fValue;
		break;
	}
	default:
		break;
	}

	return fResult;
}

bool BeUnit::GetSkillLvlData(SeCalSkillLvlData& rkData, int iSkillTypeID, int iSkillLevel)
{
	bool bRet = false;
	const SkillTable* pkSkillRes = NULL;
	BeSkill* pkSkill = GetSkill(iSkillTypeID);
	if (pkSkill)
	{
		SeSkillLvlData kData;
		pkSkill->GetLvlData(kData, iSkillLevel);
		pkSkillRes = pkSkill->GetResPtr();
		if (pkSkillRes)
		{
			rkData = kData;
			bRet = true;
		}
	}

	if (!bRet)
	{
		if (iSkillLevel == -1)
		{
			iSkillLevel = 0;
		}
		else
		{
			iSkillLevel -= 1;
		}
		pkSkillRes = GetResSkill(iSkillTypeID);
		if (pkSkillRes)
		{
			rkData = SeSkillLvlData(pkSkillRes, iSkillLevel);
			bRet = true;
		}
	}
	if (bRet)
	{
		rkData.SetSkillManaSpend((int)(rkData.GetSkillManaSpend() * (1.0f - GetDecMPCost())));

		{
			float fAllSkillDamagePer = GetMagicDamage();
			if (pkSkillRes->uiDataContentA != SKILL_DATACONTENTA_FASHANG)
			{
				float	fAddValue = GetSkillAddValue(pkSkillRes->uiAAdj, pkSkillRes->fAAdj);
				float fValue = rkData.GetDataValue(VALUE_A) + fAddValue;
				rkData.SetDataValue(VALUE_A, fValue, 1);
			}

			if (pkSkillRes->uiDataContentB != SKILL_DATACONTENTA_FASHANG)
			{
				float	fAddValue = GetSkillAddValue(pkSkillRes->uiBAdj, pkSkillRes->fBAdj);
				float fValue = rkData.GetDataValue(VALUE_B) + fAddValue;
				rkData.SetDataValue(VALUE_B, fValue, 1);
			}

			if (pkSkillRes->uiDataContentC != SKILL_DATACONTENTA_FASHANG)
			{
				float	fAddValue = GetSkillAddValue(pkSkillRes->uiCAdj, pkSkillRes->fCAdj);
				float fValue = rkData.GetDataValue(VALUE_C) + fAddValue;
				rkData.SetDataValue(VALUE_C, fValue, 1);
			}

			if (pkSkillRes->uiDataContentD != SKILL_DATACONTENTA_FASHANG)
			{
				float	fAddValue = GetSkillAddValue(pkSkillRes->uiDAdj, pkSkillRes->fDAdj);
				float fValue = rkData.GetDataValue(VALUE_D) + fAddValue;
				rkData.SetDataValue(VALUE_D, fValue, 1);
			}

			if (pkSkillRes->uiDataContentE != SKILL_DATACONTENTA_FASHANG)
			{
				float	fAddValue = GetSkillAddValue(pkSkillRes->uiEAdj, pkSkillRes->fEAdj);
				float fValue = rkData.GetDataValue(VALUE_E) + fAddValue;
				rkData.SetDataValue(VALUE_E, fValue, 1);
			}

			if (pkSkillRes->uiDataContentF != SKILL_DATACONTENTA_FASHANG)
			{
				float	fAddValue = GetSkillAddValue(pkSkillRes->uiFAdj, pkSkillRes->fFAdj);
				float fValue = rkData.GetDataValue(VALUE_F) + fAddValue;
				rkData.SetDataValue(VALUE_F, fValue, 1);
			}
		}

		if ((pkSkillRes->uiSkillProperty & SKILL_SKILLPROPERTY_NOTDECCD) == 0)
		{
			int iNowCD = rkData.GetSkillCoolDown();
			if (iSkillTypeID != 'S105' && iSkillTypeID != 'SN05' && iSkillTypeID != 'SN06' && iSkillTypeID != 'SN07' && iSkillTypeID != 'S107')
			{
				iNowCD = (int)(iNowCD * (1.0f - GetDecCDTime()));
			}

			if (iSkillTypeID == 'S105' || iSkillTypeID == 'SN05' || iSkillTypeID == 'SN06' || iSkillTypeID == 'SN07')
			{
				if (GetUD_Int(UDK_S105))
				{
					iNowCD = GetUD_Int(UDK_S105);
				}
			}
			if (iSkillTypeID == 'SQ09')
			{
				if (GetBuffer('S600'))
				{
					iNowCD *= 0.5f;
				}
			}

			if (pkSkillRes->iUIShowPos > 0 && pkSkillRes->iUIShowPos < 9)
			{
				float	fST15ForSkill = GetUD_Float(UDK_ST15_PlayerTime);
				if (fST15ForSkill > 0)
				{
					iNowCD = iNowCD * (1.0f - fST15ForSkill);
				}
			}
			else
			{
				float	fST15ForItem = GetUD_Float(UDK_ST15_ItemTime);
				if (fST15ForItem > 0)
				{
					iNowCD = iNowCD * (1.0f - fST15ForItem);
				}
			}

			rkData.SetCoolDown(iNowCD);
		}

		if (iSkillTypeID == 'SA03')
		{
			int iNowCD = rkData.GetSkillCoolDown();
			int     iSJ83 = GetUD_Int(UDK_SJ83);
			iNowCD -= iSJ83;
			rkData.SetCoolDown(iNowCD);
		}

		if (iSkillTypeID == 'SA02')
		{
			if (GetBuffer('BF13'))
			{
				rkData.SetEffectTime(4000);
			}
		}

		if (iSkillTypeID == 'SA06')
		{
			if (GetUD_Int(UDK_SA06_NoTime) == 888)
			{
				rkData.SetCoolDown(10 * 1000);
			}
		}

		rkData.SetBlastData(GetSkillBlastPer(), GetSkillBlastDmgPer());
	}
	return bRet;
}

float BeUnit::GetDecMagicResistance(void)
{
	for (std::vector<BeCarry*>::iterator itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
	{
		BeCarry* pkCarry = *itr;
		std::vector<BeNormalAtt>& kNormalAtt = pkCarry->GetNormalAttr();
		for (std::vector<BeNormalAtt>::iterator itrr = kNormalAtt.begin(); itrr != kNormalAtt.end(); ++itrr)
		{
			BeNormalAtt& rkAttr = *itrr;
			if (rkAttr.eType == NAT_DEC_MAGIC_RESISTANCE)
			{
				return rkAttr.fValue;
			}
		}
	}
	return 0.0f;
}
