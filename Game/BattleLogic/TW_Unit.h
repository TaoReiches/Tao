#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include "TW_Entity.h"
#include "TW_MemoryPool.h"
#include <Unit_table.hpp>
#include "TW_UnitData.h"
#include "TW_UnitDefine.h"
#include "TW_Commander.h"
#include "TW_CarryDefine.h"
#include "TW_ShareUnitData.h"
#include "TW_Functions.h"
#include "TW_UserDataDefine.h"
#include <TW_PointerType.h>

class BeItem;
class BeCarry;
struct UnitTable;
class BeBuffer;
class BeSkill;
class BeMapItem;

class BeUnit : public BeSingleLinkEntity, public BeEntity
{
protected:
	BeUnit(int iID);

public:
	~BeUnit(void);

public:
	inline bool IsHero(void) const
	{
		//if (!m_pkBackData || !m_pkBackData->pkRes)
		//{
		//	return false;
		//}

		//if (m_pkBackData->pkRes->uiClassType == UNIT_CLASSTYPE_HERO)
		//{
		//	return true;
		//}
		return false;
	}

	inline int GetCurrentTypeID() const
	{
		return m_pkCurData->iTypeID;
	}

	void SetCurrentTypeID(int iTypeID);

	inline void SetBackTypeID(int iTypeID)
	{
		m_pkBackData->iTypeID = iTypeID;
	}

	inline int GetBackTypeID() const
	{
		return m_pkBackData->iTypeID;
	}

	inline bool HasProperty(int iProperty) const
	{
		return ((m_pkBackData->iUnitProperty & iProperty) == iProperty) ? true : false;
	}

	bool	HasTableProperty(int iValue)
	{
		if (!m_pkBackData || !m_pkBackData->pkRes)
		{
			return false;
		}

		if ((m_pkBackData->pkRes->uiProperty & iValue) != 0)
		{
			return true;
		}
		return false;
	}

	inline BeUnitData* GetCurData(void) const
	{
		return m_pkCurData;
	}

	inline void AddProperty(int iProperty)
	{
		m_pkBackData->iUnitProperty |= iProperty;
	}

	inline int GetProperty(void) const
	{
		return m_pkBackData->iUnitProperty;
	}

	inline void ClrProperty(int iFlag)
	{
		m_pkBackData->iUnitProperty &= ~iFlag;
	}

	inline float GetOrgAttackRange(void) const
	{
		return m_pkCurData->pkRes->fAttackRange;
	}

	inline float GetBackAttackRange(void) const
	{
		return m_pkBackData->pkRes->fAttackRange;
	}

	float GetMissileArc(void)
	{
		float fSrcArc = m_pkCurData->pkRes->fMissileArc;
		float fValue = fSrcArc * 3.1415926f / 180.0f;
		return fValue;
	}

	inline float GetCollision(void) const
	{
		return m_pkBackData->pkRes->fTouchRadius;
	}

	inline int GetOBSize(void) const
	{
		return m_pkBackData->pkRes->fCollision;
	}

	inline float GetTurnSpeed(void) const
	{
		return m_pkCurData->pkRes->fTurnSpeed;
	}

	inline	const	UnitTable* GetRes()
	{
		return m_pkCurData->pkRes;
	}

	inline int GetAbilIDCount(void) const
	{
		return 4;
	}

	inline int GetAbilSkillID(int iPos) const
	{
		if (iPos < 0 || iPos > 3)
		{
			return 0;
		}

		int iType = GetCurrentTypeID();

		if (iType > 0) {

			const	UnitTable* pkUnitRes = UnitTableMgr::Get()->GetUnitTable(iType);

			if (pkUnitRes) {

				return pkUnitRes->iSkillList[iPos];
			}
		}

		return 0;

	}

	inline std::vector<BeSkill*>& GetNormalSkillVec()
	{
		return m_apkNormalSkill;
	}

	int GetDeathMoney(void) const
	{
		return m_pkBackData->pkRes->iDeathMoney;
	}

	int GetDeathExp(void) const
	{
		return m_pkBackData->pkRes->iDeathExp;
	}

	inline float GetModelScale(void) const
	{
		return m_pkCurData->pkRes->fModelScale;
	}

	int		GetDamageModle()
	{
		return m_pkCurData->pkRes->iTargetDamage;
	}

	inline int GetPlayer(void) const
	{
		return m_pkBackData->iPlayer;
	}

	inline int GetOrgPlayer(void) const
	{
		return m_pkBackData->iOrgPlayer == -1 ? GetPlayer() : m_pkBackData->iOrgPlayer;
	}

	inline void SetOrgPlayer(int iPlayerIdx)
	{
		m_pkBackData->iOrgPlayer = iPlayerIdx;
	}

	inline bool GetControl(int iPlayerIdx) const
	{
		if (m_pkBackData->iControl & (1 << iPlayerIdx))
		{
			return true;
		}
		return false;
	}

	inline int GetControl()
	{
		return m_pkBackData->iControl;
	}

	void SetControlPure(int iControl)
	{
		m_pkBackData->iControl = iControl;
	}

	inline void SetControl(int iPlayerIdx, bool bAddPlayer = true, bool bNeedRecordChange = true)
	{
		if (bNeedRecordChange)
		{
		}
		if (!bAddPlayer)
		{
			m_pkBackData->iControl = 0;
		}
		m_pkBackData->iControl |= (1 << iPlayerIdx);
	}

	inline void ClrControl(int iPlayerIdx, bool bNeedRecordChange = true)
	{
		if (bNeedRecordChange)
		{
		}
		m_pkBackData->iControl &= ~(1 << iPlayerIdx);
	}

	inline int GetCurExp(void) const
	{
		return m_pkCurData->iCurExp;
	}

	inline int GetLevel(void) const
	{
		return m_pkBackData->iLevel;
	}

	inline const int GetHPLock(void) const
	{
		return m_iHPLock;
	}

	inline void SetHPLock(int iValue)
	{
		m_iHPLock = iValue;
	}

	inline float GetHP(void) const
	{
		return m_pkCurData->fHP;
	}

	inline float GetMP(void) const
	{
		return m_pkCurData->fMP;
	}

	void SetMaxHP(float fMaxHP);

	inline float GetMaxHP(void) const
	{
		if (0 == m_pkCurData->fMaxHP)
		{
			return 1.0f;
		}

		return m_pkCurData->fMaxHP;
	}

	inline float GetHPPercent(void)
	{
		if (m_pkCurData->fMaxHP <= 0.0f)
		{
			SetMaxHP(1.0f);
		}
		return m_pkCurData->fHP / m_pkCurData->fMaxHP;
	}

	inline void SetRegenHP(float fRegenHP)
	{
		if (m_pkCurData->fRegenHP != fRegenHP)
		{
			SetShareUnitChangeFlag(BSUDCF_REGENHP);
		}
		m_pkCurData->fRegenHP = fRegenHP;
	}

	inline float GetRegenHP(void) const
	{
		if (HasUnitCarryFlag(BUCF_NO_REVERTHPMP))
		{
			return 0.0f;
		}
		return m_pkCurData->fRegenHP;
	}

	inline float GetMaxMP(void) const
	{
		return m_pkCurData->fMaxMP;
	}

	void SetMaxMP(float fMaxMP);

	inline void SetRegenMP(float fRegenMP)
	{
		if (m_pkCurData->fRegenMP != fRegenMP)
		{
			SetShareUnitChangeFlag(BSUDCF_REGENMP);
		}
		m_pkCurData->fRegenMP = fRegenMP;
	}

	inline float GetRegenMP(void) const
	{
		if (HasUnitCarryFlag(BUCF_NO_REVERTHPMP))
		{
			return 0.0f;
		}
		return m_pkCurData->fRegenMP;
	}

	inline float GetArmor(void) const
	{
		return m_pkCurData->fArmor;
	}

	inline float GetAddArmor(void) const
	{
		return m_pkCurData->fAddArmor;
	}


	inline float GetMoveSpeed(void) const
	{
		return m_pkCurData->fMoveSpeed;
	}

	inline float GetBaseMoveSpeed(void) const
	{
		return m_pkCurData->fBaseMoveSpeed;
	}

	inline bool GetSpeedLimit(void) const
	{
		return m_bSpeedLimit;
	}

	inline	void SetSpeedLimit(bool bLimit)
	{
		m_bSpeedLimit = bLimit;
	}

	int GetAttackCD(void) const;

	inline int GetAttackDamagePt(void) const
	{
		return m_pkCurData->iAttackDamagePt;
	}

	inline int GetAttackBackPt(void) const
	{
		return m_pkCurData->iAttackBackPt;
	}

	void SetAttackRange(float fRange)
	{
		if (fRange != m_pkCurData->fAttackRange)
		{
		}
		m_pkCurData->fAttackRange = fRange;
	}

	float GetAttackRange(const BeUnit* pkTarget) const
	{
		return m_pkCurData->fAttackRange + pkTarget->GetCollision();
	}

	inline int GetUnitCarryFlag(void) const
	{
		return m_iCarryFlag;
	}

	inline void SetUnitCarryFlag(int iFlag)
	{
		m_iCarryFlag |= iFlag;
	}

	inline bool HasUnitCarryFlag(int iFlag) const
	{
		return (m_iCarryFlag & iFlag) != 0;
	}

	void ClrUnitCarryFlag(int iFlag)
	{
		m_iCarryFlag &= ~iFlag;
	}

	inline void SetUnitImmunityFlag(int iFlag)
	{
		m_iImmunityFlag |= iFlag;
	}

	inline bool HasUnitImmunityFlag(int iFlag) const
	{
		return (m_iImmunityFlag & iFlag) == iFlag;
	}

	inline int GetUnitImmunityFlag(void) const
	{
		return m_iImmunityFlag;
	}

	void ClrUnitImmunityFlag(int iFlag)
	{
		m_iImmunityFlag &= ~iFlag;
	}

	inline int GetUnitNotInvisFlag(void) const
	{
		return m_iNotInvisByGroup;
	}

	inline bool HasUnitNotInvisFlag(int iFlag) const
	{
		return (m_iNotInvisByGroup & iFlag) == iFlag;
	}

	void ClrUnitNotInvisFlag(int iFlag)
	{
		m_iNotInvisByGroup &= ~iFlag;
	}

	inline void SetUnitNotInvisFlag(int iFlag)
	{
		m_iNotInvisByGroup |= iFlag;
	}

	inline bool IsUnitNotInvisToCamp(int eCamp) const
	{
		if (eCamp == GetCamp())
		{
			return true;
		}

		return HasUnitNotInvisFlag(1 << (int)eCamp) || HasUnitNotInvisFlag(1 << 20);
	}

	inline bool IsUnitInvisToAny() const
	{
		return !m_iNotInvisByGroup;
	}

	inline void DecUnitCurTime(int iDecTime)
	{
		m_pkCurData->iUnitCurLiveTime -= iDecTime;
		if (m_pkCurData->iUnitCurLiveTime == 0)
		{
			m_pkCurData->iUnitCurLiveTime = -1;
		}
	}

	inline void SetUnitCurLiveTime(int iCurLiveTime)
	{
		m_pkCurData->iUnitCurLiveTime = iCurLiveTime;
	}

	inline int GetUnitCurLiveTime(void) const
	{
		return m_pkCurData->iUnitCurLiveTime;
	}

	inline unsigned int GetUnitReliveTime(void) const
	{
		return m_pkBackData->uiUnitReliveTime;
	}

	inline int GetUnitAllLiveTime(void) const
	{
		return m_pkCurData->iUnitAllLiveTime;
	}

	void SetUnitAllLiveTime(int iAllTime)
	{
		m_pkCurData->iUnitAllLiveTime = iAllTime;
	}

	inline void SetUnitCreateTime(unsigned int dwCreateTime)
	{
		m_pkCurData->dwUnitCreateTime = dwCreateTime;
	}

	inline unsigned int GetUnitCreateTime() const
	{
		return m_pkCurData->dwUnitCreateTime;
	}

	inline float GetOrgMaxHP(void) const
	{
		return m_pkCurData->fOrgMaxHP;
	}

	inline void SetOrgMaxHP(float fOrgMaxHP)
	{
		m_pkCurData->fOrgMaxHP = fOrgMaxHP;
	}

	inline float GetOrgRegenHp(void) const
	{
		return m_pkCurData->fOrgRegenHP;
	}

	inline void SetOrgRegenHP(float fOrgDayRegenHP)
	{
		m_pkCurData->fOrgRegenHP = fOrgDayRegenHP;
	}

	inline float GetOrgMaxMP(void) const
	{
		return m_pkCurData->fOrgMaxMP;
	}

	inline void SetOrgMaxMP(float fOrgMaxMP)
	{
		m_pkCurData->fOrgMaxMP = fOrgMaxMP;
	}

	inline float GetOrgRegenMp(void) const
	{
		return m_pkCurData->fOrgRegenMP;
	}

	inline void SetOrgRegenMP(float fOrgRegenMP)
	{
		m_pkCurData->fOrgRegenMP = fOrgRegenMP;
	}

	inline float GetOrgMinDamage(void) const
	{
		return m_pkCurData->fOrgDamage;
	}

	inline void SetOrgMinDamage(float fOrgDamage)
	{
		m_pkCurData->fOrgDamage = fOrgDamage;
	}

	inline float GetOrgMaxDamage(void) const
	{
		return m_pkCurData->fOrgDamage;
	}

	inline void SetOrgMaxDamage(float fOrgMaxDamage)
	{
		m_pkCurData->fOrgDamage = fOrgMaxDamage;
	}

	inline float GetOrgArmor(void) const
	{
		return m_pkCurData->fOrgArmor;
	}

	inline void SetOrgArmor(float fOrgArmor)
	{
		m_pkCurData->fOrgArmor = fOrgArmor;
	}

	inline float GetOrgAntiMagic(void) const
	{
		return m_pkCurData->fOrgMagicArmor;
	}

	inline void SetOrgAntiMagic(float fOrgAntiMagic)
	{
		m_pkCurData->fOrgMagicArmor = fOrgAntiMagic;
	}

	inline float GetOrgMoveSpeed(void) const
	{
		return m_pkCurData->fOrgMoveSpeed;
	}

	inline void SetOrgMoveSpeed(float fOrgMoveSpeed)
	{
		m_pkCurData->fOrgMoveSpeed = fOrgMoveSpeed;
	}

	inline int GetOrgAttackCD(void) const
	{
		return m_pkCurData->iOrgAttackCD;
	}

	inline void SetOrgAttackCD(int iOrgAttackCD)
	{
		if (iOrgAttackCD != m_pkCurData->iOrgAttackCD)
		{
		}
		m_pkCurData->iOrgAttackCD = iOrgAttackCD;
	}

	inline void SetAttackType(BeAttackType eType)
	{
		m_pkCurData->eAttackType = eType;
	}

	inline BeAttackType GetAttackType(void) const
	{
		return m_pkCurData->eAttackType;
	}

	int		GetResUnitAttackType()
	{
		return m_pkCurData->pkRes->uiAttackType;
	}

	inline int GetWeaponType(void) const
	{
		if (m_pkCurData->fAttackRange < 300.0f)
		{
			return 0;
		}
		return 1;
	}

	inline void SetMissleModel(int iMissileModel)
	{
		m_pkCurData->iMissileModel = iMissileModel;
	}

	inline int GetMissleModel(void) const
	{
		return m_pkCurData->iMissileModel;
	}

	inline int GetResMissleModel(void) const
	{
		return m_pkCurData->pkRes->iMissileModel;
	}

	inline void SetMissileSpeed(float fSpeed)
	{
		m_pkCurData->fMissileSpeed = fSpeed;
	}

	inline float GetMissileSpeed(void) const
	{
		return m_pkCurData->fMissileSpeed;
	}

	void SetActionName(int iActionName, int iTime)
	{
		if (iActionName < BAN_ActionNum)
		{
			if (m_eActionName != iActionName || iActionName == BAN_attack || iActionName == BAN_Crit_Attack)
			{
				SetShareUnitChangeFlag(BSUDCF_ACTIONNAME);
			}
			m_eActionName = (BeActionName)iActionName;
			m_iActionStartTime = iTime;
		}
	}

	BeActionName GetActionName(void) const
	{
		return m_eActionName;
	}

	int GetActionSartTime(void) const
	{
		return m_iActionStartTime;
	}

	inline float GetBaseDamage(void) const
	{
		return m_pkCurData->fBaseDamage;
	}

	inline void SetBaseDamage(float fMaxDamage, bool bUpdateAttribute = true)
	{
		if (fMaxDamage != m_pkCurData->fBaseDamage)
		{
		}
		m_pkCurData->fBaseDamage = fMaxDamage;
		if (bUpdateAttribute)
		{
			UpdateAttribute(true);
		}
	}

	inline void SetAddDamage(float fAddDamage, bool bUpdateAttribute = true)
	{
		if (fAddDamage != m_pkCurData->fAddDamage)
		{
		}
		m_pkCurData->fAddDamage = fAddDamage;
		if (bUpdateAttribute)
		{
			UpdateAttribute(true);
		}
	}

	inline float GetAddDamage(void) const
	{
		return m_pkCurData->fAddDamage;
	}

	inline float GetBaseArmor(void) const
	{
		return m_pkCurData->fBaseArmor;
	}

	inline void SetBaseArmor(float fBaseArmor, bool bUpdateAttribute = true)
	{
		if (m_pkCurData->fBaseArmor != fBaseArmor)
		{
		}

		m_pkCurData->fBaseArmor = fBaseArmor;
		if (bUpdateAttribute)
		{
			UpdateAttribute(true);
		}
	}

	inline void SetAddArmor(float fAddArmor, bool bUpdateAttribute = true)
	{
		if (fAddArmor != m_pkCurData->fAddArmor)
		{
		}

		m_pkCurData->fAddArmor = fAddArmor;
		if (bUpdateAttribute)
		{
			UpdateAttribute(true);
		}
	}

	inline void SetAddArmorPer(float fAddArmorPer, bool bUpdateAttribute = true)
	{
		m_pkCurData->fAddArmorPer = fAddArmorPer;
		if (bUpdateAttribute)
		{
			UpdateAttribute(true);
		}
	}

	inline float GetAddAromorPer(void) const
	{
		return m_pkCurData->fAddArmorPer;
	}

	inline void SetAddAntiMagic(float fAddMagicArmor, bool bUpdateAttribute = true)
	{
		m_pkCurData->fAddMagicArmor = fAddMagicArmor;
		if (bUpdateAttribute)
		{
			UpdateAttribute(true);
		}
	}

	inline int GetAddAntiMagic(void)
	{
		return m_pkCurData->fAddMagicArmor;
	}

	inline void SetAddAntiMagicPer(float fAddMagicArmorPer, bool bUpdateAttribute = true)
	{
		m_pkCurData->fAddMagicArmorPer = fAddMagicArmorPer;
		if (bUpdateAttribute)
		{
			UpdateAttribute(true);
		}
	}

	inline int GetAddAntiMagicPer(void)
	{
		return m_pkCurData->fAddMagicArmorPer;
	}

	inline float GetBaseMaxHP(void)
	{
		return m_pkCurData->fBaseMaxHP;
	}

	inline void SetBaseMaxHP(float fBaseMaxHP)
	{
		m_pkCurData->fBaseMaxHP = fBaseMaxHP;
		UpdateAttribute(true);
	}

	inline float GetBaseMaxMP(void)
	{
		return m_pkCurData->fBaseMaxMP;
	}

	inline void SetBaseMaxMP(float fBaseMaxMP)
	{
		m_pkCurData->fBaseMaxMP = fBaseMaxMP;
		UpdateAttribute(true);
	}

	inline void SetBaseRegenHP(float fBaseRegenHP)
	{
		m_pkCurData->fBaseRegenHP = fBaseRegenHP;
		UpdateAttribute(true);
	}

	inline float GetBaseRegenMP(void)
	{
		return m_pkCurData->fBaseRegenMP;
	}


	inline void SetBaseRegenMP(float fBaseRegenMP)
	{
		m_pkCurData->fBaseRegenMP = fBaseRegenMP;
		UpdateAttribute(true);
	}

	inline BeUnitAction GetActionType(void) const
	{
		return m_pkCurData->eActionType;
	}

	inline bool IsActionNow(BeUnitAction eAction) const
	{
		return (m_pkCurData->eActionType == eAction);
	}

	inline bool IsActionCurTimeOut(int iDeltaTime) const
	{
		return ((m_pkCurData->iActionCurTime + iDeltaTime) >= m_pkCurData->iActionAllTime);
	}

	int		GetRealDamagePt()
	{
		float iRealPtTime = m_pkCurData->iAttackDamagePt;

		return iRealPtTime;
	}

	bool IsDamageTime(int iDeltaTime);

	inline int GetActionCurTimeNeed(void) const
	{
		return (m_pkCurData->iActionAllTime - m_pkCurData->iActionCurTime);
	}

	inline void IncActionCurTime(int iDeltaTime)
	{
		m_pkCurData->iActionCurTime += iDeltaTime;
		m_pkCurData->iAttackElapseTime += iDeltaTime;
	}

	inline int GetActionCurTime(void) const
	{
		return m_pkCurData->iActionCurTime;
	}

	inline void SetActionCurTime(int iActionCurTime)
	{
		m_pkCurData->iActionCurTime = iActionCurTime;
	}

	inline int GetActionAllTime(void) const
	{
		return m_pkCurData->iActionAllTime;
	}

	inline void SetActionAllTime(int iTime)
	{
		if (m_pkCurData->iActionAllTime != iTime)
		{
		}
		m_pkCurData->iActionAllTime = iTime;
	}

	inline int GetAttackElapseTime(void) const
	{
		return m_pkCurData->iAttackElapseTime;
	}

	inline void ResetAttackElapseTime(void)
	{
		m_pkCurData->iAttackElapseTime = 0;
	}

	inline float GetPosX(void) const
	{
		return m_pkCurData->fPosX;
	}

	inline float GetPosY(void) const
	{
		return m_pkCurData->fPosY;
	}

	inline float GetPosZ(void) const
	{
		return m_pkCurData->fPosZ;
	}

	inline float GetFace(void) const
	{
		return m_pkCurData->fFace;
	}

	inline void SetPitch(float fPitch)
	{
		m_pkCurData->fPitch = fPitch;
	}

	inline float GetPitch(void)
	{
		return m_pkCurData->fPitch;
	}

	inline void SetAbsFace(float fFace)
	{
		AdjustRadian(fFace);

		m_pkCurData->fFace = fFace;
	}

	inline float GetTarFace(void) const
	{
		return m_pkCurData->fFace;
	}

	void SetTarFace(float fTarFace)
	{
		AdjustRadian(fTarFace);
		if (fabs(m_pkCurData->fFace - fTarFace) > 0.01f)
		{
			SetShareUnitChangeFlag(BSUDCF_FACE);
		}
		m_pkCurData->fFace = fTarFace;
	}

	float GetScale(void) const
	{
		return m_pkCurData->fScale;
	}

	void SetScale(float fScale)
	{
		m_pkCurData->fScale = fScale;

		SetShareUnitChangeFlag(BSUDCF_SCALE);
	}

	bool HasSkill(void) const
	{
		return GetAbilIDCount() > 0 ? true : false;
	}

	bool HasSkill(int iID) const
	{
		return GetSkill(iID) == NULL ? false : true;
	}

	inline bool IsDead(void) const
	{
		return HasFlag(BUF_DEAD);
	}

	inline bool	InDanTiao() const
	{
		return (GetBuffer('BD59') ? true : false);
	}

	inline int GetAttackingUnitID(void) const
	{
		return m_pkBackData->iAttackingUnitID;
	}

	inline void SetAttackingUnitID(int iID, bool bIsOrb = false, bool bAttackPos = false)
	{
		int iOldID = m_pkBackData->iAttackingUnitID;
		m_pkBackData->iAttackingUnitID = iID;
	}

	inline bool IsGhost(void) const
	{
		return HasFlag(BUF_ISGHOST);
	}

	inline bool IsDividMan(void) const
	{
		return HasFlag(BUF_ISDIVIDMAN);
	}

	inline bool IsSummonTemp(void) const
	{
		return HasFlag(BUF_ISSUMMONTEMP);
	}

	inline bool IsSummomPer(void) const
	{
		return HasFlag(BUF_ISSUMMONPER);
	}

	inline bool IsForbidItem(void) const
	{
		return HasUnitCarryFlag(BUCF_ISFORBIDITEM);
	}

	inline bool CanMove(void) const
	{
		return (!HasUnitCarryFlag(BUCF_CANNOTMOVE)
			&& !HasUnitCarryFlag(BUCF_DIZZY)
			&& !HasUnitCarryFlag(BUCF_ISJUMPING)
			&& !HasUnitCarryFlag(BUCF_ISMOVING));
	}

	inline bool CanSpell(void) const
	{
		return (!HasUnitCarryFlag(BUCF_DIZZY)
			&& !HasUnitCarryFlag(BUCF_ISJUMPING)
			&& !HasUnitCarryFlag(BUCF_ISMOVING));
	}

	inline bool CanAttack(void) const
	{
		return !(HasUnitCarryFlag(BUCF_CANNOTATTACK)
			|| HasUnitCarryFlag(BUCF_ISJUMPING)
			|| HasUnitCarryFlag(BUCF_ISMOVING)
			);
	}

	inline bool IsInvincible(void) const
	{
		return (HasUnitCarryFlag(BUCF_ISINVINCIBLE) || HasFlag(BUF_ISINVINCIBLE));
	}

	inline void SetUsedSkillPoint(int iUsedPoint)
	{
		m_pkBackData->iUsedSkillPoint = iUsedPoint;
	}

	inline int GetUsedSkillPoint(void) const
	{
		return m_pkBackData->iUsedSkillPoint;
	}

	int		GetSkillPoint()
	{
		return m_pkBackData->iLevel - m_pkBackData->iUsedSkillPoint;
	}

	inline std::list<BeCommand>& GetUnitCommands(void)
	{
		return m_kCommander.GetCommands();
	}

	inline void CopyUnitCommands(BeUnit* pkUnit)
	{
		m_kCommander.SetCommands(pkUnit->GetUnitCommands());
	}

	inline void ClearAllCommands()
	{
		m_kCommander.ClearAllCommands();
	}

	inline BeCommand& GetUnitCurCommand(void)
	{
		return m_kCommander.GetCurCommand();
	}

	inline const BeExeCommand* GetUnitCurCmd(void)
	{
		return m_kCommander.GetCurCmd();
	}

	inline const BeCommandType GetUnitCurCmdType(void)
	{
		BeExeCommand* pkECmd = m_kCommander.GetCurCmd();
		if (pkECmd)
		{
			return pkECmd->GetType();
		}

		return BCT_NUMS;
	}

	inline void CopyUnitCurCommand(BeUnit* pkUnit)
	{
		GiveCommand(pkUnit->GetUnitCurCommand());
	}

	//inline void SetConnectUnit(BeUnit* pkUnit)
	//{
	//	if (pkUnit)
	//	{
	//		m_akConnectUnits.push_back(pkUnit->GetSharePtr());
	//	}
	//}

	//inline std::vector<std::auto_ptr<BeUnit>>& GetConnectUnits(void)
	//{
	//	return m_akConnectUnits;
	//}

	inline void SetShareHPUnitID(int iUnitID)
	{
		m_VecShareHPIDs.push_back(iUnitID);
	}

	inline const std::vector<int>& GetShareHPUnitID(void) const
	{
		return m_VecShareHPIDs;
	}

	inline void DelConnectUnit(BeUnit* pkUnit)
	{
		//assert(pkUnit);
		//if (pkUnit)
		//{
		//	int iID = pkUnit->GetID();
		//	for (std::vector<SeSharedPtr<BeUnit>>::iterator itr = m_akConnectUnits.begin(); itr != m_akConnectUnits.end(); ++itr)
		//	{
		//		SeSharedPtr<BeUnit>& rkUnitPtr = (*itr);
		//		if (!rkUnitPtr.IsNull() && (rkUnitPtr->GetID() == iID))
		//		{
		//			m_akConnectUnits.erase(itr);
		//			break;
		//		}
		//	}
		//}
	}

	inline void SetOrgSkillLevel(int iSkillLevel)
	{
		m_iOrgSkillLevel = iSkillLevel;
	}

	inline void SetTransTime(int iTransTime, bool bForce = false)
	{
		m_iTransTime = iTransTime;

		if (bForce || m_iTransTime < m_iAddTransTime)
		{
			m_iAddTransTime = 0;
		}
	}

	inline int GetTransTime(void)
	{
		return m_iTransTime;
	}

	inline const void SetSkillDamagePer(float fPer) const
	{
		m_pkCurData->fSkillDamagePer = fPer;
	}

	inline const float GetSkillDamagePer(void) const
	{
		return m_pkCurData->fSkillDamagePer;
	}

	inline const float GetMagicDamage(void) const
	{
		return m_pkCurData->fMagicDamage;
	}

	inline	const	float	GetDamagePer()	const
	{
		return m_pkCurData->fDamagePer;
	}

	inline	const	float	GetBeDamagePer()	const
	{
		return m_pkCurData->fBeDamagePer;
	}

	inline const float GetSkillBlastPer(void) const
	{
		return m_pkCurData->fSkillBlastPer;
	}

	inline const float GetSkillBlastDmgPer(void) const
	{
		return 1.5f;
	}

	inline void SetAntiMagic(float fAntiMagic)
	{
		m_pkCurData->fMagicArmor = fAntiMagic;
	}

	inline const float GetMagicArmor(void) const
	{
		return m_pkCurData->fMagicArmor;
	}

	inline const float GetAddMagicArmor(void) const
	{
		return m_pkCurData->fAddMagicArmor;
	}

	inline const float GetDecMagicArmor(void) const
	{
		return m_pkCurData->fDecAntiMagic;
	}

	inline const float GetLeech(void) const
	{
		return m_pkCurData->fLeech;
	}

	inline const float GetMagicLeech(void) const
	{
		return m_pkCurData->fMagicLeech;
	}

	inline const float GetToughness(void) const
	{
		return m_pkCurData->fToughness;
	}

	const float GetDecArmor(void) const
	{
		return m_pkCurData->fDecArmor;
	}
	const float GetPerDecArmor(void) const
	{
		return m_pkCurData->fPerDecArmor;
	}
	const	float	GetPerDecMagicArmor()	const
	{
		return m_pkCurData->fPerDecMagicArmor;
	}

	inline const float GetSpeedFixup(void) const
	{
		return m_fSpeedFixup;
	}

	inline void SetSpeedFixup(float fValue)
	{
		m_fSpeedFixup = fValue;
	}

	inline void SetDecMPCost(float fDecMPCost)
	{
		if (fDecMPCost != m_pkCurData->fDecMPCost)
		{
		}
		m_pkCurData->fDecMPCost = fDecMPCost;
	}
	inline const float GetDecMPCost(void) const
	{
		return m_pkCurData->fDecMPCost;
	}
	inline const float GetDecCDTime(void) const
	{
		return m_pkCurData->fDecCDTime;
	}
	float	GetPerCDTime()
	{
		return m_pkCurData->fPerCDTime;
	}
	float	GetBaoJiRate()	const
	{
		return m_pkCurData->fBaoJi;
	}
	float	GetBaoJiDamagePer()	const
	{
		return m_pkCurData->fBaoJiDamagePer;
	}

	inline const float GetAddEnmityPoint(void) const
	{
		return m_pkCurData->fEnmityPoint;
	}

	inline const float GetPerDamageReduce(void) const
	{
		return m_pkCurData->fPerDamageReduce;
	}
	inline const float GetEvadeRate(void) const
	{
		return m_pkCurData->fEvadeRate;
	}

	const BeUnitData* GetBackData() const
	{
		return m_pkBackData;
	}

	void SetOtherFlag(int iFlag)
	{
		if (!HasOtherFlag(iFlag))
		{
			SetShareUnitChangeFlag(BSUDCF_OTHERFLAG);
		}
		m_iOtherFlag |= iFlag;
	}

	void ClrOtherFlag(int iFlag)
	{
		if (HasOtherFlag(iFlag))
		{
			SetShareUnitChangeFlag(BSUDCF_OTHERFLAG);
		}
		m_iOtherFlag &= ~iFlag;
	}

	bool HasOtherFlag(int iFalg) const
	{
		return (m_iOtherFlag & iFalg) == iFalg;
	}

	int GetOtherFlag() const
	{
		return m_iOtherFlag;
	}

	inline int GetDeathRecordLastTime(void) const
	{
		return m_iDeathRecordLastTime;
	}

	inline void SetDeathRecordLastTime(int iDeathRecordLastTime)
	{
		m_iDeathRecordLastTime = iDeathRecordLastTime;
	}

	inline float GetDeathRecordTotalDamage(void) const
	{
		return m_fDeathRecordTotalDamage;
	}

	inline void SetWalkLineIdx(int iIdx)
	{
		m_iWalkLineIdx = iIdx;
	}

	inline bool IsPureNeedUpdate()
	{
		return m_bPureNeedUpdae;
	}

	inline void SetPathFindSucessTime(int iSucessTime)
	{
		if (m_iPathFindSucessTime != iSucessTime)
		{
			SetShareUnitChangeFlag(BSUDCF_PATHFINDERSUCESS);
		}
		m_iPathFindSucessTime = iSucessTime;
	}

	inline void SetActionState(int iActionState)
	{
		if (m_iActionState != iActionState)
		{
			SetShareUnitChangeFlag(BSUDCF_PATHFINDER);
		}
		m_iActionState = iActionState;
	}

	inline int GetActionState()
	{
		return m_iActionState;
	}

	inline float GetTarPosX() const
	{
		return m_fWalkTargetX;
	}

	inline float GetTarPosY() const
	{
		return m_fWalkTargetY;
	}

	inline void SetTarPosX(float fTarPosX)
	{
		if (m_fWalkTargetX != fTarPosX)
		{
			SetShareUnitChangeFlag(BSUDCF_PATHFINDER);
		}
		m_fWalkTargetX = fTarPosX;
	}

	inline void SetTarPosY(float fTarPosY)
	{
		if (m_fWalkTargetY != fTarPosY)
		{
			SetShareUnitChangeFlag(BSUDCF_PATHFINDER);
		}
		m_fWalkTargetY = fTarPosY;
	}

	inline int GetWalkLineIdx(void)
	{
		return m_iWalkLineIdx;
	}

	void SetShareUnitChangeFlag(int iFlag)
	{
		m_iShareUnitDataChangeFlag |= iFlag;
	}

	bool HasShareUnitChangeFlag(int iFlag)
	{
		return (m_iShareUnitDataChangeFlag & iFlag) != 0;
	}

	void ClrShareUnitChangeFlag(int iFlag)
	{
		m_iShareUnitDataChangeFlag &= ~iFlag;
	}

	int GetShareUnitChangeFlag()
	{
		return m_iShareUnitDataChangeFlag;
	}

	std::map<int, int> GetCommonCD()
	{
		return m_akCommonCD;
	}

	void SetCommonCD(std::map<int, int> kMap)
	{
		m_akCommonCD = kMap;
	}

	std::map<int, float>& GetShield()
	{
		return m_akShield;
	}

	unsigned int GetTransAlphaDec()
	{
		return m_dwTransAlphaDec;
	}
	int GetAlphaTransTime()
	{
		return m_iAlphaTransTime;
	}

	int GetMoveAllTime()
	{
		return m_iMoveAllTime;
	}

	float GetMinMoveDistance()
	{
		return m_fMinMoveDistance;
	}

	bool	Initialize(int iTypeID);
	void	Update(int iDeltaTime);

	void	OnPlayerChanged(void);
	void	OnSuspend(void);
	void	OnResume(void);
	void	SetUnitvisible(bool bVisible);

	void	BuyItem(int iItemTypeID);
	void	SellItem(int iPos);

	void	SetChangeScale(int iBufferID, float fChangeScale, int iChangeTime = 0);
	void	SetColorWithBuf(int iBufferID, unsigned int dwColor);
	void	AddAttach(int pcMdlFile, int iRemoveTime, int iAttackerID = 0, bool bChangeFace = true);
	bool	DelAttachByName(int iModelID);

	void    AddControlSummonUnitID(int iUnitID);

	void			AppyUnitSkill(void);

	void			OnDelete(void);

	void			SetUnitReliveTime(unsigned int uiUnitReliveTime);
	void			SetPlayer(int iPlayerIdx);
	void			SetCastMove(int iMaxTime, int iUnitID, float fTarPosX, float fTarPosY, int iSkillTypeID, BeUnitFlyType kType = BUFT_MOVE, int iAttackUnitID = 0);
	void			ClrCastMove(void);

	bool			IsIdle(void) const;
	bool			GiveCommand(BeCommand& kCmd, BeGiveCmdType eType = BCT_IMMEDIATE, bool bPlayerControl = false, bool bNeedHangCurrent = true, bool bCheckChaoFeng = true);
	int				GetCommandCount() const;

	void			SetPosition(float fX, float fY, float fZ = 0.0f, bool bNoRecordChange = false);

	void			SetUnitAction(BeUnitAction eAction, int eActionName, int iAllTime = 2000, bool bSetShow = true, bool bForceRefresh = false);

	void			UpdateAttribute(bool bUpdateNormal = true);

	void			DamagedByAbsNum(BeAttackType eAttackType, float fDamage, float fRawDamage, const std::shared_ptr<BeUnit> kAttacker, int iPlayer, int iFlag, bool bCanDead = true, bool bFirstAttack = true, int iSkillTypeID = 0, int iSkillLevel = 0, float fLeech = 0.0f, float fMultiDamage = 0.f, int iBeDamagedEffect = 0);
	void			OperateUnitDead(BeAttackType eAttackType = BAT_NONE, float fDamage = 0.0f, BeUnit* pkAttacker = NULL, int iPlayer = -1, int iFlag = 0, bool bIsTimeOver = false, int iSkillTypeID = 0);
	void			OperateUnitRemove();
	float			GetDamagedByFormula(const BeUnit* pkAttacker, const BeAttackingAttr& rkAttackingAttr, float fDamage) const;
	void			OnBeDamaged(BeAttackingAttr& rkAttackingAttr, bool bCanDead = true, bool bIgnoreInvincible = false, float fMaxHeroAbsDamage = 0.0f);

	BeCarry* AddSuiting(int iTypeID, int iNumber);
	BeCarry* AddLevelStrengthenSuit(int iSkillTypeID, int iSkillLevel);

	void 			OnItemUpDate(int iID);
	BeItem* GetItemByID(int iID) const;
	BeItem* GetItemByPos(int iPos);
	BeItem* GetItemByTypeID(int iTypeID, int iOwnPlayer = -1);
	BeItem* GetComposeItemByTypeID(int iTypeID);
	void			GetItemGroupByTypeID(std::vector<BeItem*>& rkItems, int iTypeID, int iOwnerPlayer = -1);
	int				GetItemNumsByID(int iTypeID) const;
	bool			SwapItemPos(char bySrcPos, char byTarPos);
	void			DropItemByPos(int iPos, float fX, float fY, bool bForce = false);
	void			DelItemByPos(int iPos);
	void			DelAllItem(void);
	void			DelItemByTypeID(int iTypeID, bool bFirstOnly = false);

	bool			HasItem(int iTypeID, bool bOwner = false);
	int				HasItemNum(int iTypeID, bool bOwner = false);
	int				GetFreeItemPos();
	bool			HasItemSkill(int iTypeID);
	bool			IsPackageFull(void) const;
	bool			IsPackageEmpty(void) const;
	bool			PickAroundItem(void);
	unsigned	int	GetItemTypeIDByPos(int iPos);

	BeSkill* AddSkill(int iTypeID, int iLevel = 1, bool bCurrent = false, bool bGenus = false, bool bGenusSkill = false, int iForceID = 0);
	BeSkill* GetSkill(int iTypeID, bool bCurrent = true) const;
//	BeSkill*		GetEquipSkill(int iTypeID) const;
	BeSkill* GetSkillByPos(int iPos, bool bOrg = false);
	//	BeSkill*		GetGenusSkillByPos(int iPos) const;
	int				GetCanLearnSkillIDByPos(int iPos) const;
	BeSkill* GetSkillByUIPos(int iPos) const;
	int				GetCanLearnSkillByIdx(int iPos) const;
	void			DelSkill(int iTypeID);
	void			DelAllSkill(bool bHeroSkillOnly = false);
	void			SetSkillLevel(int iTypeID, int iLevel);
	int				GetSkillLevel(int iTypeID);
	const std::vector<BeSkill*>& GetNormalSkill(void);
	bool			ActiveSkill(const BeActiveSkill& kCmd);
	//	const SeSkillRes*	GetItemSkill(int iItemID) const;
	int				GetItemSkillTypeID(int iItemID);
	int				GetItemUseSkill(int iItemTypeID);

	void			SetMP(float fMP, bool bChange = true);
	bool			IsForbidSkill(BeSkill* pkSkill) const;

	BeLearnSkillData* GetLearnSkillData(int iPos);
	BeLearnSkillData* GetAllLearnSkillData();
	bool			LearnSkill(int iSkillPos);
	int				GetCanLearnLevel(int iTypeID, int iLevel);
	int				GetSkillPointSum(void);
	void			SetSkillPointSum(int iSkillPoint);

	BeBuffer* AddBufferBegin(int iTypeID, int iOrgUnitID, int iLevel = 1, int iUnitID = 0, bool bAddDead = false, int iForceID = 0);
	void			AddBufferEnd(BeBuffer* pkBuffer);
	BeBuffer* GetBuffer(int iTypeID, int iUnitID = 0);
	const BeBuffer* GetBuffer(int iTypeID, int iUnitID = 0) const;
	BeBuffer* GetLilithBuffer();
	void	RemoveLilithBuffer(int iUnitID);
	const std::vector<BeBuffer*>& GetBufferByMulty(int iTypeID, int iUnitID = 0);
	const int GetMultyBufferSize(int iTypeID, int iUnitID = 0) const;
	const std::vector<BeBuffer*>& GetBufferByNegative(void);
	std::vector<BeBuffer*> GetAllBuffer(void) const;
	bool			DelBuffer(int iTypeID, int iUnitID = 0);
	bool			DelBufferByID(int iBufferID);
	void			UpdateState(int iDeltaTime);
	bool			DelBufferByType(int iTypeID, int iUnitID = 0);
	void			DelBufferByClean(bool bNegative = true, bool bGood = true);
	void			DelBufferByAutoClean(void);
	bool			DelNegativeBuffer(void);
	bool			DelNegativeBufferExceptNoInvins(void);
	BeBuffer* GetBufferByInnerID(int iID);

	void			RemoveBufferAttr(BeNormalAttType eType);

	const SkillTable* GetResSkill(int iType) const;


	template<class T>
	void TrgOnPreDamage_T(T& kSkill, int iCount, int eAttackType, float& fDamage, BeUnit* pkTarget, int iPlayer, int iFlag, bool bCanDead, BeAttackingAttr& kAttr, int iItemPos = -1);
	template<class T>
	void TrgOnBeDamaged_T(T& kSkill, int iCount, int eAttackType, float& fDamage, float fRawDamage, BeUnit* pkAttacker, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID, int iItemPos = -1);
	template<class T>
	void TrgOnDamage_T(T& kSkill, int iCount, int eAttackType, float& fDamage, BeUnit* pkTarget, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID, int iItemPos = -1, int iItemID = 0);
	template<class T>
	void TrgOnAttack_T(T& kSkill, int iCount, BeUnit* pkTarget, BeAttackingAttr* pkAttackAttr, int iItemPos = -1);

	void			TrgOnPreBeDamaged(int eAttackType, float& fDamage, float fRawDamage, BeUnit* pkAttacker, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID, BeAttackingAttr& kAttr);
	void			TrgOnBeDamaged(int eAttackType, float& fDamage, float fRawDamage, BeUnit* pkAttacker, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID);
	void			TrgOnPreDamage(int eAttackType, float& fDamage, BeUnit* pkTarget, int iPlayer, int iFlag, bool bCanDead, BeAttackingAttr& kAttr);
	void			TrgOnDamage(int eAttackType, float& fDamage, BeUnit* pkTarget, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID);
	void			TrgOnDead(int eAttackType, float fDamage, BeUnit* pkAttacker, int iPlayer, int iFlag, int iSkillTypeID);

	void			TrgOnPreAttack(BeUnit* pkTarget, BeAttackingAttr* pkAttackAttr);
	void			TrgOnAttack(int iTargetID, BeAttackingAttr* pkAttackAttr);
	void			TrgOnSpell(int iSkillTypeID, int iSkillLevel, int iItemID, int iTargetID, float fTargetPosX, float fTargetPosY);
	void			TrgOnUpdate(int iDeltaTime);
	void			TrgOnKill(int eAttackType, float fDamage, BeUnit* pkTarget, int iPlayer, int iFlag, int iSkillTypeID);

	void			GetAttackingAttr(BeUnit* pkTarget, BeAttackingAttr*& rkAttackingAttr);
	BeAttackingAttr& GetSelfAttackingAttr(void);
	bool			GetAttackingMiss(void) const;
	bool			GetAttackedAvoid(void) const;
	float			GetAttackedAntiLeech(void) const;
	void			GetAttackedPhysicAttr(float& fAntiPhysic, const BeUnit* pkAttacker, float fDecArmorValue, float fDecArmorPer) const;
	void			GetAttackedMagicAttr(float& fAntiMagic, const BeUnit* pkAttacker, float fDecEnemyAntiMagic, float fDecMagicArmorPer) const;
	float			GetAttackedReboundSkill(void) const;
	float			GetAttackedReboundSkillAll(void) const;
	float			GetAttackedReboundPhysicShorRange(int& iSkill) const;
	void			GetAttackedReboundPhysicBoth(float& fShortRange, float& fEject, int& iShortSkill, int& iEjectSkill);
	float			GetAttackedAttackCDAttr(void) const;
	float			GetAttackedBaoJiAttr(void) const;
	bool GetSkillLvlData(SeCalSkillLvlData& rkData, int iSkillTypeID, int iSkillLevel = -1);

	void			OnPropertyUpdate(int iLevel);
	void            SetCurExp(int iCurExp);
	void			SetLevel(int iLevel, bool bNeedRecordChange = true);
	void			AddLevel(int iAddLevel);
	void			ResetSkill(void);

	//	void			PlayLevelupEffect();

	void			SetHP(float fHP, bool bChange = true);

	void			GuanQiaAddExp(int iExp);

	bool UnitCanBeControl(int iPlayer) const;
	bool UnitCanMove(bool bTemp = false) const;
	bool UnitCanAttack(const BeUnit* pkTarget = 0, bool bTemp = false, bool bForceAttackSameGroup = false) const;

	UnitUseSkillResultType UnitCanUseSkill(int iSkillTypeID, const BeUnit* pkTarget = NULL, bool bTemp = false, bool bCheckPos = false, float fTargetPosX = 0.0f, float fTargetPosY = 0.0f, bool bArea = false, BeUnit** ppkTarget = NULL);

	bool UnitCanUseActiveSkill(int iAttackedID, int& iSkillTypeID, int& iSkillLevel);

	void TranslateSkillTargetType(int iSkillTargetFlag, int& iStaticProcFlag, int& iDynaPropFlag) const;
	bool IsSkillTargetType(const SkillTable* pkRes, const BeUnit* pkTarget = 0) const;
	bool IsTargetUnit(const BeUnit* pkUnit, int iStaticProcFlag, int iDynaPropFlag) const;

	bool PickMapItem(BeMapItem* pkMapItem);

	bool				InBossArea(const BeUnit* pkBoss) const;

	void				AddIgnoreUnit(int iUnitID, int iTime = 5000);
	bool				IsIgnoreUnit(int iUnitID) const;

	void				CopyAttribute(BeUnit* pkUnit);

	int					GetDeadCount()
	{
		return m_iDeadCount;
	}
	void				AddDeadCount()
	{
		m_iDeadCount++;
	}
	void				ResetDeadCount()
	{
		m_iDeadCount = 0;
	}

	void				ChangeSkill(int iPos, int iSkillTypeID)
	{
		//if (iPos >= 0 && iPos < iMaxHeroSkillNum)
		//{
		//	int		iLastLevel = 0;
		//	if (m_pkCurData->apkUISkill[iPos])
		//	{
		//		iLastLevel = m_pkCurData->apkUISkill[iPos]->GetLevel();

		//		m_pkCurData->apkUISkill[iPos] = nullptr;
		//		BeSkill* pkNewSkill = AddSkill(iSkillTypeID);
		//		if (pkNewSkill)
		//		{
		//			pkNewSkill->SetLevel(iLastLevel);
		//		}

		//		m_bChangeSkill = true;
		//	}
		//}
	}

	bool			IsChangeSkill()
	{
		return m_bChangeSkill;
	}
	void			ClearnChangeSkill()
	{
		m_bChangeSkill = false;
	}

public:
	void SetAutoSpellFlag(UnitAutoSpellType eType)
	{
		m_kAutoSpellFlag.SetFlag((int)eType);
	}
	void ClrAutoSpellFlag(UnitAutoSpellType eType)
	{
		m_kAutoSpellFlag.ClrFlag((int)eType);
	}

	bool HasAutoSpellFlag(UnitAutoSpellType eType)
	{
		return m_kAutoSpellFlag.HasFlag((int)eType);
	}

	void AutoSpell(UnitAutoSpellType eType, BeUnit* pkEnemy = NULL);

	void SetCommonCDLastUseTime(int iTypeID, int iTime);
	int GetCommonCDLastUseTime(int iTypeID) const;
	bool CommonCDComplete(int iTypeID, int iCDTime) const;
	void ClearAllCommonCD(void);
	std::map<int, int>& GetAllCD(void);

	void			OnDelBuffer(BeBuffer* pkBuffer, bool bUpdate = true, bool bDelEffect = true);
	void SetCheatAttr(void);

	float GetDecMagicResistance(void);
	void SetAllLiveTime(int iAllTime);

	void AddNewShield(int iBufferID, float fShield);
	float GetShieldByBuffer(int iBufferID);
	inline	float	GetTotalShield(void);

	float		GetExtraBossDamageTotal(int iBossID);

protected:
	void			SetDead(void);
	void			DelAllBuffer(bool bRelive = false);
	void			DelAllBufferFinal(void);
	void			OnInitAttribute(bool bCurrent = true, bool bNeedRecordChange = true);
	void			SafeDelBuf(BeBuffer* pkBuffer, bool bNeedRecordChange = true);

	void			UpdateValidItem(void);
	void			UpdateValidBuffer(void);
	void			UpdateValidSkill(bool bReset = false);
	void			OnAddBuffer(BeBuffer* pkBuffer);

	BeBuffer* SuperposeBuffer(int iTypeID, int iSingleCount = 1, int iMultiCount = 1, int iLevel = 1, int iUnitID = 0);

public:
	void			GetSingleSuperposeBuffer(int iTypeID, std::vector<BeBuffer*>& rakBuffer, int iUnitID);
	void			GetMultiSuperposeBuffer(int iTypeID, std::vector<BeBuffer*>& rakBuffer, int iUnitID);

	BeItem* AddItem(int iTypeID, int iPos = -1, int iForceID = 0, int iOrgData = 0);
public:
	void        SetUD_Int(UserDataKey eKey, int i);
	void        SetUD_Float(UserDataKey eKey, float f);
	int         GetUD_Int(UserDataKey eKey, int i = 0)const;
	float       GetUD_Float(UserDataKey eKey, float f = 0.f)const;
	int         PopUD_Int(UserDataKey eKey);
	float       PopUD_Float(UserDataKey eKey);
	void        ClearUserData(UserDataKey eKey);
	bool        HasUserData(UserDataKey eKey) const;

protected:
	int				ParseItemAndCompose(int& iI167UseCount, int& iSellProtectTime, int& iCanPos, std::vector<int>& akChildItemOrgUnitID, int iAcceptItemID = 0, int iAccetItemOwnPlayer = 0);

protected:
	BeUnitData* m_pkBackData;
	BeUnitData* m_pkCurData;

	BeCommander									m_kCommander;
	std::vector<BeUnitData*>					m_akUnitData;
	bool										m_bChangeSkill;

	std::vector<BeSkill*>						m_apkNormalSkill;
	int							m_iOtherFlag;
	int							m_iCarryFlag;
	int							m_iImmunityFlag;
	std::vector<BeCarry*>		m_apkCarry;
	BeItem* m_apkItem[6];

	int							m_iNotInvisByGroup;

	std::map<int, int>			m_akCommonCD;

	std::map<int, float>	m_akShield;

	float						m_fPreMaxHP;
	float						m_fPreMaxMP;

	int							m_iMoveToUnitID;
	float                       m_fMoveTarPosX;
	float                       m_fMoveTarPosY;
	int							m_iMoveAllTime;
	float						m_fMinMoveDistance;

	//std::vector<std::auto_ptr<BeUnit>>			m_akConnectUnits;
	std::vector<int>			m_VecShareHPIDs;

	int							m_iOrgSkillLevel;

	int					m_iTransTime;
	int					m_iAddTransTime;
	UnitActorTransType  m_eActorTransType;
	bool				m_bNeedTrans;

	unsigned int				m_dwTransAlphaDec;
	unsigned int				m_dwStartAlpha;
	int					m_iAlphaTransTime;	

	std::vector<BeBuffer*> m_apkMultyBuffer;

	BeActionName		m_eActionName;
	int					m_iActionStartTime;	

	int					m_iHPLock;			
	float				m_fSpeedFixup;		
	bool				m_bSpeedLimit;		
	bool				m_iSummonSkillOrItem;

	int					m_iOutBattleTime;
	bool				m_bOutOfBattle;

	int			m_iDeathRecordLastTime;
	float			m_fDeathRecordTotalDamage;
	int			m_iWalkLineIdx;

	unsigned int	m_iTurnTableFlag;
	unsigned int	m_iTurnTableInfo[6];
	unsigned int	m_uiNowTurnResult;
	unsigned int	m_uiTurnResult[6];
	unsigned int	m_uiReTurnNum;
	unsigned int	m_uiConfirmResult;

	unsigned int	m_uiHunQiSkillID;
	unsigned short	m_usHunStoneNum;
	unsigned short  m_usHunStoneTotalNum;
	unsigned int	m_uiTurnTableAttrInfo[6];
	std::vector<BeUnitBeDamageRecord> m_apkBeDamageRecord;

	BeCommand			m_kLastCommand;

protected:
	void	UpdateMP(int iDeltaTime);
	void	UpdateHP(int iDeltaTime);
	void	UpdateLiveTime(int iDeltaTime);
	void	UpdateBuffer(int iDeltaTime);

protected:
	bool m_bIsUpdate;
protected:
	std::vector<int>  m_kLabel;


public:
	int		m_iActionState;
	float	m_fWalkTargetX;
	float	m_fWalkTargetY;
	int		m_iPathFindSucessTime;
private:
	bool	m_bPureNeedUpdae;
	void	UpdateTransrate(int iDeltaTime);

private:
	void	UpdateSummonUnit();

public:
	void SetFlag(int iFlag, bool bNeedRecordChange = true);
	void ClrFlag(int iFlag, bool bNeedRecordChange = true);
	void	ClrAllPureData();
	void SetFace(float fFace, bool bChange = true);
	void SetPreTurnFace(float fFace);
	float	GetSkillAddValue(int iEnum, float fValue);
	void SetCurAttackCD(int iCurAttackCD);
	void UpdateBattleState(bool bBattle);
	void	UpdateItemPassiveSkill();
	bool	GetBattleState()
	{
		return m_bOutOfBattle;
	}
private:
	bool					m_bNewUnit[2];
	BeShareSelfData			m_kShareSelfDataCur;
	BeShareSelfData			m_kShareSelfDataLast;
	int						m_iEffectID;
	int						m_iActionStayTime;
	int						m_iGrassIndex;
	int						m_iGiveAttackCmdTime;
	std::unordered_map<int, TePointerType>		m_akUserData;
	BeFlagObj				m_kAutoSpellFlag;
	int						m_iDeadCount;
	bool					m_bNeedUpdateObstacle;
	bool					m_bSetObstacle;
	std::shared_ptr<BeUnit>		m_spSharePtr;
	bool					m_abInPlayerVision[20];
	bool					m_abEverInPlayerVision[20];
	int						m_eCamp;
	int						m_iShareUnitDataChangeFlag;
	std::vector<BeBuffer*>	m_apkBuffer;
	float					m_fTotalShield;
	bool					m_bVisionForCamp[iPureCampNum];
	bool					m_bVisionForCampLast[iPureCampNum];
	bool					m_bGrassVisionForCamp[iPureCampNum];
	bool					m_bGrassVisionForCampLast[iPureCampNum];
	unsigned	int			m_iTabInfoFlagForCampA;
	unsigned	int			m_iTabInfoFlagForCampB;
	std::vector<int>		m_kItemPassiveSkill;
	int						m_iLastAttackTime;
	int						m_iLastAttackHeroTime;
	int						m_iCastVisionTime;
	int						m_iOneFrameDamage;
	std::vector<int>		m_kVecSkillUnit;


public:
	inline		void					SetUnitVision(void* pkData);
	inline		void* GetUnitVision()	const;
	inline		unsigned int			GetClass(void) const;
	inline		unsigned int			GetSecClass(void) const;
	inline		int						GetEffectID();
	inline		void					SetEffectID(int iValue);
	inline		void					SetActionStayTime(int iValue);
	inline		void					ClearActionStayTime();
	inline		const int				GetGrassIndex()  const;
	inline		bool					IsNewUnit(int iCamp);
	inline		void					SetNewUnit(bool bNew);
	inline		BeShareSelfData& GetCurShareSelfData();
	inline		const BeShareSelfData& GetLastShareSelfData();
	inline		int						GetCamp(void) const;
	inline		void					SetCamp(int iCamp);
	inline		int						GetOrgAttackCD();
	inline		int						GetVisionRadius(void) const;
	inline		void					SetIsInPlayerVision(int iSeat);
	inline		bool					GetIsInPlayerVision(int iSeat) const;
	inline		bool					GetIsInPlayerVisionEver(int iSeat) const;
	inline		void					SetHaveSetObstacle(bool bSet);
	inline		bool					GetHaveSetObstacle() const;
	inline		bool					GetNeedUpdateObstacle() const;
	inline		const std::shared_ptr<BeUnit> GetSharePtr() const;
	inline		int						GetDamageNum();
	inline	const	bool				GetUnitVisionForCamp(int iDstCamp)	const;
	inline		void					SetTabInfoFlag(int iFlag);
	inline		unsigned	int			GetTabInfoFlag(int iCamp)	const;
	inline		void					ClrTabInfoFlag(int iCamp);
	inline		unsigned	int			GetZhuanShuSkillTypeID();
	inline		unsigned	int			GetSkillTypeIDByPos(int iPos);
	inline		void					AddItemPassiveSkill(int iSkillTypeID);
	inline		void					ClearItemPassiveSkill();
	inline		bool					CanAddItemPassiveSkill(int iSkillTypeID);
	inline		void					SetLastAttackTime(int iTime);
	inline		int						GetLastAttackTime();
	inline		int						GetLastGiveAttackCmdTime();
	inline		void					SetLastAttackHeroTime(int iTime);
	inline		int						GetLastAttackHeroTime();
	inline		void					SetCastVisionTime(int iTime);

	inline		void					SetTurnTableFlag(int iFlag);
	inline		unsigned int			GetTurnTableFlag();
	inline		void					ClearTurnTableFlag();
	inline		void					SetNextTurnTime(int iTime);
	inline		int						GetNextTurnTime();
	inline		void					SetTurnTableInfoByPos(int iIdx, unsigned int iValue);
	inline		unsigned int			GetTurnTableInfoByPos(int iIdx);
	inline		void					SetNowTurnTableResult(unsigned int iValue);
	inline		unsigned int			GetNowTurnTableResult();
	inline		unsigned int			GetReTurnNum();
	inline		void					AddReTurnNum();
	inline		unsigned int			GetTurnConfirmResult();
	inline		void					SetTurnResultArrayByPos(int iIdx, unsigned int iValue);
	inline		unsigned int			GetTurnResultArrayByPos(int iIdx);
	inline		void					AddOneFrameDamage(float fDamage);
	inline		float					GetOneFrameDamage();

	inline		void					AddBeDamageRecord(int iID, unsigned int uiDamage);
	inline		unsigned int 			GetBeDamageRecord(int iID);

	inline void							SetHunQiSkillID(unsigned int uiSkillID);
	inline unsigned int					GetHunQiSkillID();
	inline void							AddHunStoneNum(unsigned short usCount);
	inline void							ReduceHunStoneNum(unsigned short usCount);
	inline void							SetHunStoneNum(unsigned short usCount);
	inline unsigned short				GetHHunStoneNum();
	inline void							AddHunStoneTotalNum(unsigned short usCount);
	inline unsigned short				GetHunStoneTotalNum();
	inline unsigned int					GetTurnTableAttrInfoByPos(int iPos);
	inline void							SetTurnTableAttrInfoByPos(int iPos, unsigned int uiSkillID);
	inline void							AddTurnTableAttrInfo(unsigned int uiSkillID);
	inline	void						AddSkillUnit(int iUnitID);
	inline	std::vector<int>& GetSkillUnit();
};

inline	const		bool	BeUnit::GetUnitVisionForCamp(int iDstCamp)	const
{
	if (iDstCamp < 0 || iDstCamp >= iPureCampNum)
	{
		return false;
	}
	if (m_bVisionForCamp[iDstCamp] && m_bGrassVisionForCamp[iDstCamp])
	{
		return true;
	}
	return false;
}

inline unsigned int BeUnit::GetClass(void) const
{
	if (!m_pkBackData || !m_pkBackData->pkRes)
	{
		return 0;
	}

	return m_pkBackData->pkRes->uiClassType;
}
inline unsigned int BeUnit::GetSecClass(void) const
{
	if (!m_pkBackData || !m_pkBackData->pkRes)
	{
		return 0;
	}

	//return m_pkBackData->pkRes->uiSecClassType;
}

inline int			BeUnit::GetEffectID()
{
	return m_iEffectID;
}
inline void			BeUnit::SetEffectID(int iValue)
{
	m_iEffectID = iValue;
}
inline void			BeUnit::SetActionStayTime(int iValue)
{
	if (iValue > m_iActionStayTime)
	{
		m_iActionStayTime = iValue;
	}
}
inline		void	BeUnit::ClearActionStayTime()
{
	m_iActionStayTime = 0;
}
inline const int	BeUnit::GetGrassIndex()  const
{
	return m_iGrassIndex;
}
inline bool			BeUnit::IsNewUnit(int iCamp)
{
	if (iCamp < 0 || iCamp > 1)
	{
		return false;
	}
	return m_bNewUnit[iCamp];
}
inline void			BeUnit::SetNewUnit(bool bNew)
{
}
inline BeShareSelfData& BeUnit::GetCurShareSelfData()
{
	return m_kShareSelfDataCur;
}
inline const BeShareSelfData& BeUnit::GetLastShareSelfData()
{
	return m_kShareSelfDataLast;
}
inline int			BeUnit::GetCamp(void) const
{
	return m_eCamp;
}
inline void			BeUnit::SetCamp(int iCamp)
{
	SetShareUnitChangeFlag(BSUDCF_CAMP);
	m_eCamp = iCamp;
}
inline int			BeUnit::GetOrgAttackCD()
{
	return m_pkCurData->iOrgAttackCD;
}
inline int			BeUnit::GetVisionRadius(void) const
{
	return m_pkCurData->iVisionRaius;
}
inline void			BeUnit::SetIsInPlayerVision(int iSeat)
{
	if (iSeat < 0 || iSeat >= 20)
	{
		return;
	}
	m_abInPlayerVision[iSeat] = true;
}
inline bool			BeUnit::GetIsInPlayerVision(int iSeat) const
{
	return true;

	if (iSeat < 0 || iSeat >= 20)
	{
		return false;
	}
	return m_abInPlayerVision[iSeat];
}
inline bool			BeUnit::GetIsInPlayerVisionEver(int iSeat) const
{
	if (iSeat < 0 || iSeat >= 20)
	{
		return false;
	}
	return m_abEverInPlayerVision[iSeat];
}
inline void			BeUnit::SetHaveSetObstacle(bool bSet)
{
	m_bSetObstacle = bSet;
}
inline bool			BeUnit::GetHaveSetObstacle() const
{
	return m_bSetObstacle;
}
inline bool			BeUnit::GetNeedUpdateObstacle() const
{
	return m_bNeedUpdateObstacle;
}
inline const std::shared_ptr<BeUnit> BeUnit::GetSharePtr() const
{
	return m_spSharePtr;
}
inline int			BeUnit::GetDamageNum()
{
	return m_pkCurData->fBaseDamage + m_pkCurData->fAddDamage;
}
inline		void					BeUnit::SetTabInfoFlag(int iFlag)
{
	m_iTabInfoFlagForCampA |= iFlag;
	m_iTabInfoFlagForCampB |= iFlag;
}
inline		unsigned	int			BeUnit::GetTabInfoFlag(int iCamp)	const
{
	return 0;
}
inline		void					BeUnit::ClrTabInfoFlag(int iCamp)
{

}
inline		unsigned	int			BeUnit::GetZhuanShuSkillTypeID()
{
	unsigned	int		iSkillTypeID = 0;
	if (m_pkCurData->pkRes)
	{
		iSkillTypeID = m_pkCurData->pkRes->iSkillList[4];
		if (iSkillTypeID < 'SM01' || iSkillTypeID > 'SM99')
		{
			return 0;
		}
	}

	return iSkillTypeID;
}
inline		unsigned	int			BeUnit::GetSkillTypeIDByPos(int iPos)
{
	unsigned	int		iSkillTypeID = 0;
	if (m_pkCurData->pkRes && iPos <= 4 && iPos >= 0)
	{
		iSkillTypeID = m_pkCurData->pkRes->iSkillList[iPos];
	}

	return iSkillTypeID;
}

inline		void					BeUnit::AddItemPassiveSkill(int iSkillTypeID)
{
	m_kItemPassiveSkill.push_back(iSkillTypeID);
}
inline		void					BeUnit::ClearItemPassiveSkill()
{
	m_kItemPassiveSkill.clear();
}
inline		bool					BeUnit::CanAddItemPassiveSkill(int iSkillTypeID)
{
	if (!m_kItemPassiveSkill.empty())
	{
		for (int iIndex = 0; iIndex < m_kItemPassiveSkill.size(); ++iIndex)
		{
			if (m_kItemPassiveSkill[iIndex] == iSkillTypeID)
			{
				return false;
			}
		}
	}
	return true;
}
inline		void					BeUnit::SetLastAttackTime(int iTime)
{
	m_iLastAttackTime = iTime;
}
inline		int						BeUnit::GetLastAttackTime()
{
	return m_iLastAttackTime;
}

inline		int						BeUnit::GetLastGiveAttackCmdTime()
{
	return m_iGiveAttackCmdTime;
}

inline		void					BeUnit::SetLastAttackHeroTime(int iTime)
{
	m_iLastAttackHeroTime = iTime;
}
inline		int						BeUnit::GetLastAttackHeroTime()
{
	return m_iLastAttackHeroTime;
}
inline		float					BeUnit::GetTotalShield(void)
{
	return m_fTotalShield;
}
inline		void					BeUnit::SetCastVisionTime(int iTime)
{
	m_iCastVisionTime = iTime;
}

inline		void					BeUnit::SetTurnTableFlag(int iFlag)
{
	m_iTurnTableFlag |= iFlag;
}

inline		unsigned int			BeUnit::GetTurnTableFlag()
{
	return m_iTurnTableFlag;
}

inline		void					BeUnit::ClearTurnTableFlag()
{
	m_iTurnTableFlag = 0;
}

inline		void					BeUnit::SetTurnTableInfoByPos(int iIdx, unsigned int iValue)
{
	if (iIdx < 0 && iIdx >= 6)
	{
		return;
	}
	m_iTurnTableInfo[iIdx] = iValue;
}

inline		unsigned int			BeUnit::GetTurnTableInfoByPos(int iIdx)
{
	if (iIdx < 0 && iIdx >= 6)
	{
		return 0;
	}
	return m_iTurnTableInfo[iIdx];
}

inline		unsigned int			BeUnit::GetNowTurnTableResult()
{
	return this->m_uiNowTurnResult;
}

inline		void					BeUnit::SetNowTurnTableResult(unsigned int uiResult)
{
	this->m_uiNowTurnResult = uiResult;
}

inline		unsigned int			BeUnit::GetReTurnNum()
{
	return this->m_uiReTurnNum;
}

inline		void					BeUnit::AddReTurnNum()
{
	m_uiReTurnNum++;
}

inline		void					BeUnit::SetTurnResultArrayByPos(int iIdx, unsigned int iValue)
{
	if (iIdx < 0 && iIdx >= 6)
	{
		return;
	}
	this->m_uiTurnResult[iIdx] = iValue;
}

inline		unsigned int			BeUnit::GetTurnResultArrayByPos(int iIdx)
{
	if (iIdx < 0 && iIdx >= 6)
	{
		return 0;
	}
	return this->m_uiTurnResult[iIdx];
}

inline		unsigned int			BeUnit::GetTurnConfirmResult()
{
	return this->m_uiConfirmResult;
}
inline		void					BeUnit::AddOneFrameDamage(float fDamage)
{
	m_iOneFrameDamage += fDamage;
}
inline		float					BeUnit::GetOneFrameDamage()
{
	return m_iOneFrameDamage;
}

inline		void					BeUnit::AddBeDamageRecord(int iID, unsigned int uiDamage)
{
	for (int i = 0; i < m_apkBeDamageRecord.size(); i++)
	{
		if (m_apkBeDamageRecord[i].iID == iID)
		{
			m_apkBeDamageRecord[i].uiBeDamage += uiDamage;
			return;
		}
	}
	m_apkBeDamageRecord.push_back(BeUnitBeDamageRecord(iID, uiDamage));
}

inline		unsigned int 			BeUnit::GetBeDamageRecord(int iID)
{
	for (int i = 0; i < m_apkBeDamageRecord.size(); i++)
	{
		if (m_apkBeDamageRecord[i].iID == iID)
		{
			return m_apkBeDamageRecord[i].uiBeDamage;
		}
	}
	return 0;
}

inline unsigned int					BeUnit::GetHunQiSkillID()
{
	return m_uiHunQiSkillID;
}

inline void					BeUnit::SetHunQiSkillID(unsigned int uiSkillID)
{
	m_uiHunQiSkillID = uiSkillID;
}

inline unsigned int					BeUnit::GetTurnTableAttrInfoByPos(int iPos)
{
	return m_uiTurnTableAttrInfo[iPos];
}

inline	void						BeUnit::AddSkillUnit(int iUnitID)
{
	m_kVecSkillUnit.push_back(iUnitID);
}

inline	std::vector<int>& BeUnit::GetSkillUnit()
{
	return	m_kVecSkillUnit;
}
