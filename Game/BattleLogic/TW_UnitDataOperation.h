#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>

#include "TW_UnitDataDefine.h"
#include "TW_UnitBase.h"
#include "Unit_table.hpp"

class TwUnitDataOperation : public TwUnitBase
{
public:
    void OnInitAttribute(bool bCurrent, bool bNeedRecordChange);
    void OnPropertyUpdate(int iLevel);
    void SetCurExp(int iCurExp);
    void SetLevel(int iLevel, bool bNeedRecordChange = false);

	BeLearnSkillData* GetLearnSkillData(int iPos);
	bool LearnSkill(int iSkillPos);
	float	GetSkillAddValue(int iEnum, float fValue);
	bool GetSkillLvlData(SeCalSkillLvlData& rkData, int iSkillTypeID, int iSkillLevel = 1);

	int	GetSkillPointSum(void);
	void SetSkillPointSum(int iSkillPoint);
	int GetCurrentTypeID() const;
	void SetCurrentTypeID(int iTypeID);
	void SetBackTypeID(int iTypeID);
	int GetBackTypeID() const;
	bool HasProperty(int iProperty) const;
	bool HasTableProperty(int iValue);
	float GetMissileArc(void);
	float GetBackAttackRange(void) const;
	float GetOrgAttackRange(void) const;
	void ClrProperty(int iFlag);
	int GetProperty(void) const;
	void AddProperty(int iProperty);
	std::shared_ptr<TwUnitData> GetCurData(void) const;
	float GetCollision(void) const;
	int GetOBSize(void) const;
	float GetTurnSpeed(void) const;
	const	std::shared_ptr<const UnitTable> GetRes();
	int GetOrgPlayer(void) const;
    std::uint64_t GetPlayer() const;
	int		GetDamageModle();
	float GetModelScale(void) const;
	int GetDeathExp(void) const;
	int GetDeathMoney(void) const;
	int GetLevel(void) const;
	int GetCurExp(void) const;
	void ClrControl(int iPlayerIdx, bool bNeedRecordChange = true);
	void SetControl(int iPlayerIdx, bool bAddPlayer = true, bool bNeedRecordChange = true);
	void SetControlPure(int iControl);
	int GetControl() const;
	bool GetControl(int iPlayerIdx) const;
	void SetOrgPlayer(int iPlayerIdx);
	float GetHPPercent(void);
	float GetMaxHP(void) const;
	void SetMaxHP(float fMaxHP);
	float GetMP(void) const;
	float GetHP(void) const;
	void SetRegenMP(float fRegenMP);
	float GetMaxMP(void) const;
	float GetRegenHP(void) const;
	void SetRegenHP(float fRegenHP);
	float GetBaseMoveSpeed(void) const;
	float GetMoveSpeed(void) const;
	float GetAddArmor(void) const;
	float GetArmor(void) const;
	float GetRegenMP(void) const;
	float GetAttackRange(const std::shared_ptr<TwUnit> pkTarget) const;
	void SetAttackRange(float fRange);
	int GetAttackBackPt(void) const;
	int GetAttackDamagePt(void) const;
	bool CanAttack(void) const;
	bool CanSpell(void) const;
	bool CanMove(void) const;
	bool IsForbidItem(void) const;
	const std::shared_ptr<TwUnitData> GetBackData() const;
	void SetHP(float fHP, bool bChange = false);
	void AddLevel(int iAddLevel);
	void SetMP(float fMP, bool bChange = true);
	void DecUnitCurTime(int iDecTime);
	void SetUnitCurLiveTime(int iCurLiveTime);
	int GetUnitCurLiveTime(void) const;
	unsigned int GetUnitReliveTime(void) const;
	int GetUnitAllLiveTime(void) const;
	void SetUnitAllLiveTime(int iAllTime);
	void SetUnitCreateTime(unsigned int dwCreateTime);
	unsigned int GetUnitCreateTime() const;
	float GetOrgMaxHP(void) const;
	void SetOrgMaxHP(float fOrgMaxHP);
	float GetOrgRegenHp(void) const;
	void SetOrgRegenHP(float fOrgDayRegenHP);
	float GetOrgMaxMP(void) const;
	void SetOrgMaxMP(float fOrgMaxMP);
	float GetOrgRegenMp(void) const;
	void SetOrgRegenMP(float fOrgRegenMP);
	float GetOrgMinDamage(void) const;
	void SetOrgMinDamage(float fOrgDamage);
	float GetOrgMaxDamage(void) const;
	void SetOrgMaxDamage(float fOrgMaxDamage);
	float GetOrgArmor(void) const;
	void SetOrgArmor(float fOrgArmor);
	float GetOrgAntiMagic(void) const;
	void SetOrgAntiMagic(float fOrgAntiMagic);
	float GetOrgMoveSpeed(void) const;
	void SetOrgMoveSpeed(float fOrgMoveSpeed);
	int GetOrgAttackCD(void) const;
	void SetOrgAttackCD(int iOrgAttackCD);
	void SetAttackType(BeAttackType eType);
	BeAttackType GetAttackType(void) const;
	int		GetResUnitAttackType();
	int GetWeaponType(void) const;
	void SetMissleModel(int iMissileModel);
	int GetMissleModel(void) const;
	int GetResMissleModel(void) const;
	void SetMissileSpeed(float fSpeed);
	float GetMissileSpeed(void) const;
	float GetBaseDamage(void) const;
	float GetAddDamage(void) const;
	float GetBaseArmor(void) const;
	float GetAddAromorPer(void) const;
	float GetAddAntiMagic(void);
	float GetAddAntiMagicPer(void);
	float GetBaseMaxHP(void);
	float GetBaseMaxMP(void);
	float GetBaseRegenMP(void);
	BeUnitAction GetActionType(void) const;
	bool IsActionNow(BeUnitAction eAction) const;
	bool IsActionCurTimeOut(int iDeltaTime) const;
	int		GetRealDamagePt();
	int GetActionCurTimeNeed(void) const;
	void IncActionCurTime(int iDeltaTime);
	int GetActionCurTime(void) const;
	void SetActionCurTime(int iActionCurTime);
	int GetActionAllTime(void) const;
	void SetActionAllTime(int iTime);
	int GetAttackElapseTime(void) const;
	void ResetAttackElapseTime(void);
	float GetPosX(void) const;
	float GetPosY(void) const;
	float GetPosZ(void) const;
	float GetFace(void) const;
	void SetPitch(float fPitch);
	float GetPitch(void);
	float GetTarFace(void) const;
	float GetScale(void) const;
	int GetAttackingUnitID(void) const;
	void SetAttackingUnitID(int iID, bool bIsOrb = false, bool bAttackPos = false);
	void SetUsedSkillPoint(int iUsedPoint);
	int GetUsedSkillPoint(void) const;
	int		GetSkillPoint();
	const void SetSkillDamagePer(float fPer) const;
	const float GetSkillDamagePer(void) const;
	const float GetMagicDamage(void) const;
	const	float	GetDamagePer()	const;
	const	float	GetBeDamagePer()	const;
	const float GetSkillBlastPer(void) const;
	const float GetSkillBlastDmgPer(void) const;
	void SetAntiMagic(float fAntiMagic);
	const float GetMagicArmor(void) const;
	const float GetAddMagicArmor(void) const;
	const float GetDecMagicArmor(void) const;
	const float GetLeech(void) const;
	const float GetMagicLeech(void) const;
	const float GetToughness(void) const;
	const float GetDecArmor(void) const;
	const float GetPerDecArmor(void) const;
	const	float	GetPerDecMagicArmor()	const;
	const float GetDecMPCost(void) const;
	const float GetDecCDTime(void) const;
	float	GetPerCDTime();
	float	GetBaoJiRate()	const;
	float	GetBaoJiDamagePer()	const;
	const float GetAddEnmityPoint(void) const;
	const float GetPerDamageReduce(void) const;
	const float GetEvadeRate(void) const;

protected:
    TwUnitDataOperation();
    TwUnitDataOperation(int iID);
    ~TwUnitDataOperation();

protected:
    std::shared_ptr<TwUnitData>                         m_pkBackData;
	std::shared_ptr<TwUnitData>                         m_pkCurData;
    std::vector<std::shared_ptr<TwUnitData>>            m_akUnitData;
};

inline void TwUnitDataOperation::SetMP(float fMP, bool bChange)
{
	float fPreMP = m_pkCurData->fMP;

	if (fMP > m_pkCurData->fMaxMP)
	{
		m_pkCurData->fMP = m_pkCurData->fMaxMP;
	}
	else if (fMP <= 0.0f)
	{
		m_pkCurData->fMP = 0.0f;
	}
	else
	{
		m_pkCurData->fMP = fMP;
	}

	if (fPreMP != m_pkCurData->fMP && bChange)
	{
		SetOutputChangeFlag(BSUDCF_CURMP);
	}
}

inline void TwUnitDataOperation::SetMaxHP(float fMaxHP)
{
	if (fMaxHP != m_pkCurData->fMaxHP)
	{
		SetOutputChangeFlag(BSUDCF_MAXHP);
	}
	m_pkCurData->fMaxHP = fMaxHP;
}

inline void TwUnitDataOperation::SetCurrentTypeID(int iTypeID)
{
	if (m_pkCurData->iTypeID != iTypeID)
	{
		SetOutputChangeFlag(BSUDCF_TYPEID);
	}
	m_pkCurData->iTypeID = iTypeID;
}

inline const std::shared_ptr<TwUnitData> TwUnitDataOperation::GetBackData() const
{
	return m_pkBackData;
}

inline bool TwUnitDataOperation::IsForbidItem(void) const
{
	return HasUnitCarryFlag(BUCF_ISFORBIDITEM);
}

inline bool TwUnitDataOperation::CanMove(void) const
{
	return (!HasUnitCarryFlag(BUCF_CANNOTMOVE)
		&& !HasUnitCarryFlag(BUCF_DIZZY)
		&& !HasUnitCarryFlag(BUCF_ISJUMPING)
		&& !HasUnitCarryFlag(BUCF_ISMOVING));
}

inline bool TwUnitDataOperation::CanSpell(void) const
{
	return (!HasUnitCarryFlag(BUCF_DIZZY)
		&& !HasUnitCarryFlag(BUCF_ISJUMPING)
		&& !HasUnitCarryFlag(BUCF_ISMOVING));
}

inline bool TwUnitDataOperation::CanAttack(void) const
{
	return !(HasUnitCarryFlag(BUCF_CANNOTATTACK)
		|| HasUnitCarryFlag(BUCF_ISJUMPING)
		|| HasUnitCarryFlag(BUCF_ISMOVING)
		);
}

inline int TwUnitDataOperation::GetAttackDamagePt(void) const
{
	return m_pkCurData->iAttackDamagePt;
}

inline int TwUnitDataOperation::GetAttackBackPt(void) const
{
	return m_pkCurData->iAttackBackPt;
}

inline void TwUnitDataOperation::SetAttackRange(float fRange)
{
	if (fRange != m_pkCurData->fAttackRange)
	{
	}
	m_pkCurData->fAttackRange = fRange;
}

inline float TwUnitDataOperation::GetRegenMP(void) const
{
	if (HasUnitCarryFlag(BUCF_NO_REVERTHPMP))
	{
		return 0.0f;
	}
	return m_pkCurData->fRegenMP;
}

inline float TwUnitDataOperation::GetArmor(void) const
{
	return m_pkCurData->fArmor;
}

inline float TwUnitDataOperation::GetAddArmor(void) const
{
	return m_pkCurData->fAddArmor;
}

inline float TwUnitDataOperation::GetMoveSpeed(void) const
{
	return m_pkCurData->fMoveSpeed;
}

inline float TwUnitDataOperation::GetBaseMoveSpeed(void) const
{
	return m_pkCurData->fBaseMoveSpeed;
}

inline void TwUnitDataOperation::SetRegenHP(float fRegenHP)
{
	if (m_pkCurData->fRegenHP != fRegenHP)
	{
		SetOutputChangeFlag(BSUDCF_REGENHP);
	}
	m_pkCurData->fRegenHP = fRegenHP;
}

inline float TwUnitDataOperation::GetRegenHP(void) const
{
	if (HasUnitCarryFlag(BUCF_NO_REVERTHPMP))
	{
		return 0.0f;
	}
	return m_pkCurData->fRegenHP;
}

inline float TwUnitDataOperation::GetMaxMP(void) const
{
	return m_pkCurData->fMaxMP;
}

inline void TwUnitDataOperation::SetRegenMP(float fRegenMP)
{
	if (m_pkCurData->fRegenMP != fRegenMP)
	{
		SetOutputChangeFlag(BSUDCF_REGENMP);
	}
	m_pkCurData->fRegenMP = fRegenMP;
}

inline float TwUnitDataOperation::GetHP(void) const
{
	return m_pkCurData->fHP;
}

inline float TwUnitDataOperation::GetMP(void) const
{
	return m_pkCurData->fMP;
}

inline float TwUnitDataOperation::GetMaxHP(void) const
{
	if (0 == m_pkCurData->fMaxHP)
	{
		return 1.0f;
	}

	return m_pkCurData->fMaxHP;
}

inline float TwUnitDataOperation::GetHPPercent(void)
{
	if (m_pkCurData->fMaxHP <= 0.0f)
	{
		SetMaxHP(1.0f);
	}
	return (m_pkCurData->fHP) / (m_pkCurData->fMaxHP);
}

inline void TwUnitDataOperation::SetOrgPlayer(int iPlayerIdx)
{
	m_pkBackData->iOrgPlayer = iPlayerIdx;
}

inline bool TwUnitDataOperation::GetControl(int iPlayerIdx) const
{
	if (m_pkBackData->iControl & (1 << iPlayerIdx))
	{
		return true;
	}
	return false;
}

inline int TwUnitDataOperation::GetControl() const
{
	return m_pkBackData->iControl;
}

inline void TwUnitDataOperation::SetControlPure(int iControl)
{
	m_pkBackData->iControl = iControl;
}

inline void TwUnitDataOperation::SetControl(int iPlayerIdx, bool bAddPlayer, bool bNeedRecordChange)
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

inline void TwUnitDataOperation::ClrControl(int iPlayerIdx, bool bNeedRecordChange)
{
	if (bNeedRecordChange)
	{
	}
	m_pkBackData->iControl &= ~(1 << iPlayerIdx);
}

inline int TwUnitDataOperation::GetCurExp(void) const
{
	return m_pkCurData->iCurExp;
}

inline int TwUnitDataOperation::GetLevel(void) const
{
	return m_pkBackData->iLevel;
}

inline int TwUnitDataOperation::GetDeathMoney(void) const
{
	return 0;
}

inline int TwUnitDataOperation::GetDeathExp(void) const
{
	return 0;
}

inline float TwUnitDataOperation::GetModelScale(void) const
{
	return 0;
}

inline int		TwUnitDataOperation::GetDamageModle()
{
	return 0;
}

inline std::uint64_t TwUnitDataOperation::GetPlayer() const
{
	return m_pkBackData->Player;
}

inline float TwUnitDataOperation::GetCollision(void) const
{
	return m_pkBackData->pkRes->fTouchRadius;
}

inline int TwUnitDataOperation::GetOBSize(void) const
{
	return m_pkBackData->pkRes->uiCollision;
}

inline float TwUnitDataOperation::GetTurnSpeed(void) const
{
	return m_pkCurData->pkRes->fTurnSpeed;
}

inline	const	std::shared_ptr<const UnitTable> TwUnitDataOperation::GetRes()
{
	return m_pkCurData->pkRes;
}

inline std::shared_ptr<TwUnitData> TwUnitDataOperation::GetCurData(void) const
{
	return m_pkCurData;
}

inline void TwUnitDataOperation::AddProperty(int iProperty)
{
	m_pkBackData->iUnitProperty |= iProperty;
}

inline int TwUnitDataOperation::GetProperty(void) const
{
	return m_pkBackData->iUnitProperty;
}

inline void TwUnitDataOperation::ClrProperty(int iFlag)
{
	m_pkBackData->iUnitProperty &= ~iFlag;
}

inline float TwUnitDataOperation::GetOrgAttackRange(void) const
{
	return m_pkCurData->pkRes->fAttackRange;
}

inline float TwUnitDataOperation::GetBackAttackRange(void) const
{
	return m_pkBackData->pkRes->fAttackRange;
}

inline float TwUnitDataOperation::GetMissileArc(void)
{
	return 0;
}

inline int TwUnitDataOperation::GetCurrentTypeID() const
{
	return m_pkCurData->iTypeID;
}

inline void TwUnitDataOperation::SetBackTypeID(int iTypeID)
{
	m_pkBackData->iTypeID = iTypeID;
}

inline int TwUnitDataOperation::GetBackTypeID() const
{
	return m_pkBackData->iTypeID;
}

inline bool TwUnitDataOperation::HasProperty(int iProperty) const
{
	return ((m_pkBackData->iUnitProperty & iProperty) == iProperty) ? true : false;
}

inline bool	TwUnitDataOperation::HasTableProperty(int iValue)
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

inline int	TwUnitDataOperation::GetSkillPointSum(void)
{
	return (m_pkBackData->iSkillPointSum);
}

inline void TwUnitDataOperation::SetSkillPointSum(int iSkillPoint)
{
	(m_pkBackData->iSkillPointSum) = iSkillPoint;
}
inline void TwUnitDataOperation::DecUnitCurTime(int iDecTime)
{
	m_pkCurData->iUnitCurLiveTime -= iDecTime;
	if (m_pkCurData->iUnitCurLiveTime == 0)
	{
		m_pkCurData->iUnitCurLiveTime = -1;
	}
}

inline void TwUnitDataOperation::SetUnitCurLiveTime(int iCurLiveTime)
{
	m_pkCurData->iUnitCurLiveTime = iCurLiveTime;
}

inline int TwUnitDataOperation::GetUnitCurLiveTime(void) const
{
	return m_pkCurData->iUnitCurLiveTime;
}

inline unsigned int TwUnitDataOperation::GetUnitReliveTime(void) const
{
	return m_pkBackData->uiUnitReliveTime;
}

inline int TwUnitDataOperation::GetUnitAllLiveTime(void) const
{
	return m_pkCurData->iUnitAllLiveTime;
}

inline void TwUnitDataOperation::SetUnitAllLiveTime(int iAllTime)
{
	m_pkCurData->iUnitAllLiveTime = iAllTime;
}

inline void TwUnitDataOperation::SetUnitCreateTime(unsigned int dwCreateTime)
{
	m_pkCurData->dwUnitCreateTime = dwCreateTime;
}

inline unsigned int TwUnitDataOperation::GetUnitCreateTime() const
{
	return m_pkCurData->dwUnitCreateTime;
}

inline float TwUnitDataOperation::GetOrgMaxHP(void) const
{
	return m_pkCurData->fOrgMaxHP;
}

inline void TwUnitDataOperation::SetOrgMaxHP(float fOrgMaxHP)
{
	m_pkCurData->fOrgMaxHP = fOrgMaxHP;
}
inline float TwUnitDataOperation::GetOrgRegenHp(void) const
{
	return m_pkCurData->fOrgRegenHP;
}

inline void TwUnitDataOperation::SetOrgRegenHP(float fOrgDayRegenHP)
{
	m_pkCurData->fOrgRegenHP = fOrgDayRegenHP;
}

inline float TwUnitDataOperation::GetOrgMaxMP(void) const
{
	return m_pkCurData->fOrgMaxMP;
}

inline void TwUnitDataOperation::SetOrgMaxMP(float fOrgMaxMP)
{
	m_pkCurData->fOrgMaxMP = fOrgMaxMP;
}

inline float TwUnitDataOperation::GetOrgRegenMp(void) const
{
	return m_pkCurData->fOrgRegenMP;
}

inline void TwUnitDataOperation::SetOrgRegenMP(float fOrgRegenMP)
{
	m_pkCurData->fOrgRegenMP = fOrgRegenMP;
}

inline float TwUnitDataOperation::GetOrgMinDamage(void) const
{
	return m_pkCurData->fOrgDamage;
}

inline void TwUnitDataOperation::SetOrgMinDamage(float fOrgDamage)
{
	m_pkCurData->fOrgDamage = fOrgDamage;
}

inline float TwUnitDataOperation::GetOrgMaxDamage(void) const
{
	return m_pkCurData->fOrgDamage;
}

inline void TwUnitDataOperation::SetOrgMaxDamage(float fOrgMaxDamage)
{
	m_pkCurData->fOrgDamage = fOrgMaxDamage;
}

inline float TwUnitDataOperation::GetOrgArmor(void) const
{
	return m_pkCurData->fOrgArmor;
}

inline void TwUnitDataOperation::SetOrgArmor(float fOrgArmor)
{
	m_pkCurData->fOrgArmor = fOrgArmor;
}

inline float TwUnitDataOperation::GetOrgAntiMagic(void) const
{
	return m_pkCurData->fOrgMagicArmor;
}

inline void TwUnitDataOperation::SetOrgAntiMagic(float fOrgAntiMagic)
{
	m_pkCurData->fOrgMagicArmor = fOrgAntiMagic;
}

inline float TwUnitDataOperation::GetOrgMoveSpeed(void) const
{
	return m_pkCurData->fOrgMoveSpeed;
}

inline void TwUnitDataOperation::SetOrgMoveSpeed(float fOrgMoveSpeed)
{
	m_pkCurData->fOrgMoveSpeed = fOrgMoveSpeed;
}

inline int TwUnitDataOperation::GetOrgAttackCD(void) const
{
	return m_pkCurData->iOrgAttackCD;
}

inline void TwUnitDataOperation::SetOrgAttackCD(int iOrgAttackCD)
{
	if (iOrgAttackCD != m_pkCurData->iOrgAttackCD)
	{
	}
	m_pkCurData->iOrgAttackCD = iOrgAttackCD;
}

inline void TwUnitDataOperation::SetAttackType(BeAttackType eType)
{
	m_pkCurData->eAttackType = eType;
}

inline BeAttackType TwUnitDataOperation::GetAttackType(void) const
{
	return m_pkCurData->eAttackType;
}

inline int		TwUnitDataOperation::GetResUnitAttackType()
{
	return m_pkCurData->pkRes->uiAttackType;
}

inline int TwUnitDataOperation::GetWeaponType(void) const
{
	if (m_pkCurData->fAttackRange < 300.0f)
	{
		return 0;
	}
	return 1;
}

inline void TwUnitDataOperation::SetMissleModel(int iMissileModel)
{
	m_pkCurData->iMissileModel = iMissileModel;
}

inline int TwUnitDataOperation::GetMissleModel(void) const
{
	return m_pkCurData->iMissileModel;
}

inline int TwUnitDataOperation::GetResMissleModel(void) const
{
	return m_pkCurData->pkRes->uiMissileModel;
}

inline void TwUnitDataOperation::SetMissileSpeed(float fSpeed)
{
	m_pkCurData->fMissileSpeed = fSpeed;
}

inline float TwUnitDataOperation::GetMissileSpeed(void) const
{
	return m_pkCurData->fMissileSpeed;
}

inline float TwUnitDataOperation::GetBaseDamage(void) const
{
	return m_pkCurData->fBaseDamage;
}
inline float TwUnitDataOperation::GetAddDamage(void) const
{
	return m_pkCurData->fAddDamage;
}

inline float TwUnitDataOperation::GetBaseArmor(void) const
{
	return m_pkCurData->fBaseArmor;
}
inline float TwUnitDataOperation::GetAddAromorPer(void) const
{
	return m_pkCurData->fAddArmorPer;
}
inline float TwUnitDataOperation::GetAddAntiMagic(void)
{
	return m_pkCurData->fAddMagicArmor;
}
inline float TwUnitDataOperation::GetAddAntiMagicPer(void)
{
	return m_pkCurData->fAddMagicArmorPer;
}

inline float TwUnitDataOperation::GetBaseMaxHP(void)
{
	return m_pkCurData->fBaseMaxHP;
}
inline float TwUnitDataOperation::GetBaseMaxMP(void)
{
	return m_pkCurData->fBaseMaxMP;
}
inline float TwUnitDataOperation::GetBaseRegenMP(void)
{
	return m_pkCurData->fBaseRegenMP;
}
inline BeUnitAction TwUnitDataOperation::GetActionType(void) const
{
	return m_pkCurData->eActionType;
}

inline bool TwUnitDataOperation::IsActionNow(BeUnitAction eAction) const
{
	return (m_pkCurData->eActionType == eAction);
}

inline bool TwUnitDataOperation::IsActionCurTimeOut(int iDeltaTime) const
{
	return ((m_pkCurData->iActionCurTime + iDeltaTime) >= m_pkCurData->iActionAllTime);
}
inline int		TwUnitDataOperation::GetRealDamagePt()
{
	int iRealPtTime = m_pkCurData->iAttackDamagePt;

	return iRealPtTime;
}
inline int TwUnitDataOperation::GetActionCurTimeNeed(void) const
{
	return (m_pkCurData->iActionAllTime - m_pkCurData->iActionCurTime);
}

inline void TwUnitDataOperation::IncActionCurTime(int iDeltaTime)
{
	m_pkCurData->iActionCurTime += iDeltaTime;
	m_pkCurData->iAttackElapseTime += iDeltaTime;
}

inline int TwUnitDataOperation::GetActionCurTime(void) const
{
	return m_pkCurData->iActionCurTime;
}

inline void TwUnitDataOperation::SetActionCurTime(int iActionCurTime)
{
	m_pkCurData->iActionCurTime = iActionCurTime;
}

inline int TwUnitDataOperation::GetActionAllTime(void) const
{
	return m_pkCurData->iActionAllTime;
}

inline void TwUnitDataOperation::SetActionAllTime(int iTime)
{
	if (m_pkCurData->iActionAllTime != iTime)
	{
	}
	m_pkCurData->iActionAllTime = iTime;
}

inline int TwUnitDataOperation::GetAttackElapseTime(void) const
{
	return m_pkCurData->iAttackElapseTime;
}

inline void TwUnitDataOperation::ResetAttackElapseTime(void)
{
	m_pkCurData->iAttackElapseTime = 0;
}

inline float TwUnitDataOperation::GetPosX(void) const
{
	return m_pkCurData->fPosX;
}

inline float TwUnitDataOperation::GetPosY(void) const
{
	return m_pkCurData->fPosY;
}

inline float TwUnitDataOperation::GetPosZ(void) const
{
	return m_pkCurData->fPosZ;
}

inline float TwUnitDataOperation::GetFace(void) const
{
	return m_pkCurData->fFace;
}

inline void TwUnitDataOperation::SetPitch(float fPitch)
{
	m_pkCurData->fPitch = fPitch;
}

inline float TwUnitDataOperation::GetPitch(void)
{
	return m_pkCurData->fPitch;
}
inline float TwUnitDataOperation::GetTarFace(void) const
{
	return m_pkCurData->fFace;
}
inline float TwUnitDataOperation::GetScale(void) const
{
	return m_pkCurData->fScale;
}
inline int TwUnitDataOperation::GetAttackingUnitID(void) const
{
	return m_pkBackData->iAttackingUnitID;
}

inline void TwUnitDataOperation::SetAttackingUnitID(int iID, bool bIsOrb, bool bAttackPos)
{
	int iOldID = m_pkBackData->iAttackingUnitID;
	m_pkBackData->iAttackingUnitID = iID;
}

inline void TwUnitDataOperation::SetUsedSkillPoint(int iUsedPoint)
{
	m_pkBackData->iUsedSkillPoint = iUsedPoint;
}

inline int TwUnitDataOperation::GetUsedSkillPoint(void) const
{
	return m_pkBackData->iUsedSkillPoint;
}

inline int		TwUnitDataOperation::GetSkillPoint()
{
	return m_pkBackData->iLevel - m_pkBackData->iUsedSkillPoint;
}
inline const void TwUnitDataOperation::SetSkillDamagePer(float fPer) const
{
	m_pkCurData->fSkillDamagePer = fPer;
}

inline const float TwUnitDataOperation::GetSkillDamagePer(void) const
{
	return m_pkCurData->fSkillDamagePer;
}

inline const float TwUnitDataOperation::GetMagicDamage(void) const
{
	return m_pkCurData->fMagicDamage;
}

inline	const	float	TwUnitDataOperation::GetDamagePer()	const
{
	return m_pkCurData->fDamagePer;
}

inline	const	float	TwUnitDataOperation::GetBeDamagePer()	const
{
	return m_pkCurData->fBeDamagePer;
}

inline const float TwUnitDataOperation::GetSkillBlastPer(void) const
{
	return m_pkCurData->fSkillBlastPer;
}

inline const float TwUnitDataOperation::GetSkillBlastDmgPer(void) const
{
	return 1.5f;
}

inline void TwUnitDataOperation::SetAntiMagic(float fAntiMagic)
{
	m_pkCurData->fMagicArmor = fAntiMagic;
}

inline const float TwUnitDataOperation::GetMagicArmor(void) const
{
	return m_pkCurData->fMagicArmor;
}

inline const float TwUnitDataOperation::GetAddMagicArmor(void) const
{
	return m_pkCurData->fAddMagicArmor;
}

inline const float TwUnitDataOperation::GetDecMagicArmor(void) const
{
	return m_pkCurData->fDecAntiMagic;
}

inline const float TwUnitDataOperation::GetLeech(void) const
{
	return m_pkCurData->fLeech;
}

inline const float TwUnitDataOperation::GetMagicLeech(void) const
{
	return m_pkCurData->fMagicLeech;
}

inline const float TwUnitDataOperation::GetToughness(void) const
{
	return m_pkCurData->fToughness;
}
inline const float TwUnitDataOperation::GetDecArmor(void) const
{
	return m_pkCurData->fDecArmor;
}
inline const float TwUnitDataOperation::GetPerDecArmor(void) const
{
	return m_pkCurData->fPerDecArmor;
}
inline const	float	TwUnitDataOperation::GetPerDecMagicArmor()	const
{
	return m_pkCurData->fPerDecMagicArmor;
}
inline const float TwUnitDataOperation::GetDecMPCost(void) const
{
	return m_pkCurData->fDecMPCost;
}
inline const float TwUnitDataOperation::GetDecCDTime(void) const
{
	return m_pkCurData->fDecCDTime;
}
inline float	TwUnitDataOperation::GetPerCDTime()
{
	return m_pkCurData->fPerCDTime;
}
inline float	TwUnitDataOperation::GetBaoJiRate()	const
{
	return m_pkCurData->fBaoJi;
}
inline float	TwUnitDataOperation::GetBaoJiDamagePer()	const
{
	return m_pkCurData->fBaoJiDamagePer;
}
inline const float TwUnitDataOperation::GetAddEnmityPoint(void) const
{
	return m_pkCurData->fEnmityPoint;
}

inline const float TwUnitDataOperation::GetPerDamageReduce(void) const
{
	return m_pkCurData->fPerDamageReduce;
}
inline const float TwUnitDataOperation::GetEvadeRate(void) const
{
	return m_pkCurData->fEvadeRate;
}
