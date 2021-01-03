#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>

#include "TW_UnitDataDefine.h"
#include "TW_UnitBase.h"
#include "Unit_table.hpp"

class TwUnitData : public TwUnitBase
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
	std::shared_ptr<BeUnitData> GetCurData(void) const;
	float GetCollision(void) const;
	int GetOBSize(void) const;
	float GetTurnSpeed(void) const;
	const	std::shared_ptr<const UnitTable> GetRes();
	int GetOrgPlayer(void) const;
	int GetPlayer(void) const;
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
	const std::shared_ptr<BeUnitData> GetBackData() const;
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
    TwUnitData();
    TwUnitData(int iID);
    ~TwUnitData();

protected:
    std::shared_ptr<BeUnitData>                         m_pkBackData;
	std::shared_ptr<BeUnitData>                         m_pkCurData;
    std::vector<std::shared_ptr<BeUnitData>>            m_akUnitData;
};

inline void TwUnitData::SetMP(float fMP, bool bChange)
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

inline void TwUnitData::SetMaxHP(float fMaxHP)
{
	if (fMaxHP != m_pkCurData->fMaxHP)
	{
		SetOutputChangeFlag(BSUDCF_MAXHP);
	}
	m_pkCurData->fMaxHP = fMaxHP;
}

inline void TwUnitData::SetCurrentTypeID(int iTypeID)
{
	if (m_pkCurData->iTypeID != iTypeID)
	{
		SetOutputChangeFlag(BSUDCF_TYPEID);
	}
	m_pkCurData->iTypeID = iTypeID;
}

inline const std::shared_ptr<BeUnitData> TwUnitData::GetBackData() const
{
	return m_pkBackData;
}

inline bool TwUnitData::IsForbidItem(void) const
{
	return HasUnitCarryFlag(BUCF_ISFORBIDITEM);
}

inline bool TwUnitData::CanMove(void) const
{
	return (!HasUnitCarryFlag(BUCF_CANNOTMOVE)
		&& !HasUnitCarryFlag(BUCF_DIZZY)
		&& !HasUnitCarryFlag(BUCF_ISJUMPING)
		&& !HasUnitCarryFlag(BUCF_ISMOVING));
}

inline bool TwUnitData::CanSpell(void) const
{
	return (!HasUnitCarryFlag(BUCF_DIZZY)
		&& !HasUnitCarryFlag(BUCF_ISJUMPING)
		&& !HasUnitCarryFlag(BUCF_ISMOVING));
}

inline bool TwUnitData::CanAttack(void) const
{
	return !(HasUnitCarryFlag(BUCF_CANNOTATTACK)
		|| HasUnitCarryFlag(BUCF_ISJUMPING)
		|| HasUnitCarryFlag(BUCF_ISMOVING)
		);
}

inline int TwUnitData::GetAttackDamagePt(void) const
{
	return m_pkCurData->iAttackDamagePt;
}

inline int TwUnitData::GetAttackBackPt(void) const
{
	return m_pkCurData->iAttackBackPt;
}

inline void TwUnitData::SetAttackRange(float fRange)
{
	if (fRange != m_pkCurData->fAttackRange)
	{
	}
	m_pkCurData->fAttackRange = fRange;
}

inline float TwUnitData::GetRegenMP(void) const
{
	if (HasUnitCarryFlag(BUCF_NO_REVERTHPMP))
	{
		return 0.0f;
	}
	return m_pkCurData->fRegenMP;
}

inline float TwUnitData::GetArmor(void) const
{
	return m_pkCurData->fArmor;
}

inline float TwUnitData::GetAddArmor(void) const
{
	return m_pkCurData->fAddArmor;
}

inline float TwUnitData::GetMoveSpeed(void) const
{
	return m_pkCurData->fMoveSpeed;
}

inline float TwUnitData::GetBaseMoveSpeed(void) const
{
	return m_pkCurData->fBaseMoveSpeed;
}

inline void TwUnitData::SetRegenHP(float fRegenHP)
{
	if (m_pkCurData->fRegenHP != fRegenHP)
	{
		SetOutputChangeFlag(BSUDCF_REGENHP);
	}
	m_pkCurData->fRegenHP = fRegenHP;
}

inline float TwUnitData::GetRegenHP(void) const
{
	if (HasUnitCarryFlag(BUCF_NO_REVERTHPMP))
	{
		return 0.0f;
	}
	return m_pkCurData->fRegenHP;
}

inline float TwUnitData::GetMaxMP(void) const
{
	return m_pkCurData->fMaxMP;
}

inline void TwUnitData::SetRegenMP(float fRegenMP)
{
	if (m_pkCurData->fRegenMP != fRegenMP)
	{
		SetOutputChangeFlag(BSUDCF_REGENMP);
	}
	m_pkCurData->fRegenMP = fRegenMP;
}

inline float TwUnitData::GetHP(void) const
{
	return m_pkCurData->fHP;
}

inline float TwUnitData::GetMP(void) const
{
	return m_pkCurData->fMP;
}

inline float TwUnitData::GetMaxHP(void) const
{
	if (0 == m_pkCurData->fMaxHP)
	{
		return 1.0f;
	}

	return m_pkCurData->fMaxHP;
}

inline float TwUnitData::GetHPPercent(void)
{
	if (m_pkCurData->fMaxHP <= 0.0f)
	{
		SetMaxHP(1.0f);
	}
	return (m_pkCurData->fHP) / (m_pkCurData->fMaxHP);
}

inline void TwUnitData::SetOrgPlayer(int iPlayerIdx)
{
	m_pkBackData->iOrgPlayer = iPlayerIdx;
}

inline bool TwUnitData::GetControl(int iPlayerIdx) const
{
	if (m_pkBackData->iControl & (1 << iPlayerIdx))
	{
		return true;
	}
	return false;
}

inline int TwUnitData::GetControl() const
{
	return m_pkBackData->iControl;
}

inline void TwUnitData::SetControlPure(int iControl)
{
	m_pkBackData->iControl = iControl;
}

inline void TwUnitData::SetControl(int iPlayerIdx, bool bAddPlayer, bool bNeedRecordChange)
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

inline void TwUnitData::ClrControl(int iPlayerIdx, bool bNeedRecordChange)
{
	if (bNeedRecordChange)
	{
	}
	m_pkBackData->iControl &= ~(1 << iPlayerIdx);
}

inline int TwUnitData::GetCurExp(void) const
{
	return m_pkCurData->iCurExp;
}

inline int TwUnitData::GetLevel(void) const
{
	return m_pkBackData->iLevel;
}

inline int TwUnitData::GetDeathMoney(void) const
{
	return 0;
}

inline int TwUnitData::GetDeathExp(void) const
{
	return 0;
}

inline float TwUnitData::GetModelScale(void) const
{
	return 0;
}

inline int		TwUnitData::GetDamageModle()
{
	return 0;
}

inline int TwUnitData::GetPlayer(void) const
{
	return m_pkBackData->iPlayer;
}

inline int TwUnitData::GetOrgPlayer(void) const
{
	return m_pkBackData->iOrgPlayer == -1 ? GetPlayer() : m_pkBackData->iOrgPlayer;
}

inline float TwUnitData::GetCollision(void) const
{
	return m_pkBackData->pkRes->fTouchRadius;
}

inline int TwUnitData::GetOBSize(void) const
{
	return m_pkBackData->pkRes->uiCollision;
}

inline float TwUnitData::GetTurnSpeed(void) const
{
	return m_pkCurData->pkRes->fTurnSpeed;
}

inline	const	std::shared_ptr<const UnitTable> TwUnitData::GetRes()
{
	return m_pkCurData->pkRes;
}

inline std::shared_ptr<BeUnitData> TwUnitData::GetCurData(void) const
{
	return m_pkCurData;
}

inline void TwUnitData::AddProperty(int iProperty)
{
	m_pkBackData->iUnitProperty |= iProperty;
}

inline int TwUnitData::GetProperty(void) const
{
	return m_pkBackData->iUnitProperty;
}

inline void TwUnitData::ClrProperty(int iFlag)
{
	m_pkBackData->iUnitProperty &= ~iFlag;
}

inline float TwUnitData::GetOrgAttackRange(void) const
{
	return m_pkCurData->pkRes->fAttackRange;
}

inline float TwUnitData::GetBackAttackRange(void) const
{
	return m_pkBackData->pkRes->fAttackRange;
}

inline float TwUnitData::GetMissileArc(void)
{
	return 0;
}

inline int TwUnitData::GetCurrentTypeID() const
{
	return m_pkCurData->iTypeID;
}

inline void TwUnitData::SetBackTypeID(int iTypeID)
{
	m_pkBackData->iTypeID = iTypeID;
}

inline int TwUnitData::GetBackTypeID() const
{
	return m_pkBackData->iTypeID;
}

inline bool TwUnitData::HasProperty(int iProperty) const
{
	return ((m_pkBackData->iUnitProperty & iProperty) == iProperty) ? true : false;
}

inline bool	TwUnitData::HasTableProperty(int iValue)
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

inline int	TwUnitData::GetSkillPointSum(void)
{
	return (m_pkBackData->iSkillPointSum);
}

inline void TwUnitData::SetSkillPointSum(int iSkillPoint)
{
	(m_pkBackData->iSkillPointSum) = iSkillPoint;
}
inline void TwUnitData::DecUnitCurTime(int iDecTime)
{
	m_pkCurData->iUnitCurLiveTime -= iDecTime;
	if (m_pkCurData->iUnitCurLiveTime == 0)
	{
		m_pkCurData->iUnitCurLiveTime = -1;
	}
}

inline void TwUnitData::SetUnitCurLiveTime(int iCurLiveTime)
{
	m_pkCurData->iUnitCurLiveTime = iCurLiveTime;
}

inline int TwUnitData::GetUnitCurLiveTime(void) const
{
	return m_pkCurData->iUnitCurLiveTime;
}

inline unsigned int TwUnitData::GetUnitReliveTime(void) const
{
	return m_pkBackData->uiUnitReliveTime;
}

inline int TwUnitData::GetUnitAllLiveTime(void) const
{
	return m_pkCurData->iUnitAllLiveTime;
}

inline void TwUnitData::SetUnitAllLiveTime(int iAllTime)
{
	m_pkCurData->iUnitAllLiveTime = iAllTime;
}

inline void TwUnitData::SetUnitCreateTime(unsigned int dwCreateTime)
{
	m_pkCurData->dwUnitCreateTime = dwCreateTime;
}

inline unsigned int TwUnitData::GetUnitCreateTime() const
{
	return m_pkCurData->dwUnitCreateTime;
}

inline float TwUnitData::GetOrgMaxHP(void) const
{
	return m_pkCurData->fOrgMaxHP;
}

inline void TwUnitData::SetOrgMaxHP(float fOrgMaxHP)
{
	m_pkCurData->fOrgMaxHP = fOrgMaxHP;
}
inline float TwUnitData::GetOrgRegenHp(void) const
{
	return m_pkCurData->fOrgRegenHP;
}

inline void TwUnitData::SetOrgRegenHP(float fOrgDayRegenHP)
{
	m_pkCurData->fOrgRegenHP = fOrgDayRegenHP;
}

inline float TwUnitData::GetOrgMaxMP(void) const
{
	return m_pkCurData->fOrgMaxMP;
}

inline void TwUnitData::SetOrgMaxMP(float fOrgMaxMP)
{
	m_pkCurData->fOrgMaxMP = fOrgMaxMP;
}

inline float TwUnitData::GetOrgRegenMp(void) const
{
	return m_pkCurData->fOrgRegenMP;
}

inline void TwUnitData::SetOrgRegenMP(float fOrgRegenMP)
{
	m_pkCurData->fOrgRegenMP = fOrgRegenMP;
}

inline float TwUnitData::GetOrgMinDamage(void) const
{
	return m_pkCurData->fOrgDamage;
}

inline void TwUnitData::SetOrgMinDamage(float fOrgDamage)
{
	m_pkCurData->fOrgDamage = fOrgDamage;
}

inline float TwUnitData::GetOrgMaxDamage(void) const
{
	return m_pkCurData->fOrgDamage;
}

inline void TwUnitData::SetOrgMaxDamage(float fOrgMaxDamage)
{
	m_pkCurData->fOrgDamage = fOrgMaxDamage;
}

inline float TwUnitData::GetOrgArmor(void) const
{
	return m_pkCurData->fOrgArmor;
}

inline void TwUnitData::SetOrgArmor(float fOrgArmor)
{
	m_pkCurData->fOrgArmor = fOrgArmor;
}

inline float TwUnitData::GetOrgAntiMagic(void) const
{
	return m_pkCurData->fOrgMagicArmor;
}

inline void TwUnitData::SetOrgAntiMagic(float fOrgAntiMagic)
{
	m_pkCurData->fOrgMagicArmor = fOrgAntiMagic;
}

inline float TwUnitData::GetOrgMoveSpeed(void) const
{
	return m_pkCurData->fOrgMoveSpeed;
}

inline void TwUnitData::SetOrgMoveSpeed(float fOrgMoveSpeed)
{
	m_pkCurData->fOrgMoveSpeed = fOrgMoveSpeed;
}

inline int TwUnitData::GetOrgAttackCD(void) const
{
	return m_pkCurData->iOrgAttackCD;
}

inline void TwUnitData::SetOrgAttackCD(int iOrgAttackCD)
{
	if (iOrgAttackCD != m_pkCurData->iOrgAttackCD)
	{
	}
	m_pkCurData->iOrgAttackCD = iOrgAttackCD;
}

inline void TwUnitData::SetAttackType(BeAttackType eType)
{
	m_pkCurData->eAttackType = eType;
}

inline BeAttackType TwUnitData::GetAttackType(void) const
{
	return m_pkCurData->eAttackType;
}

inline int		TwUnitData::GetResUnitAttackType()
{
	return m_pkCurData->pkRes->uiAttackType;
}

inline int TwUnitData::GetWeaponType(void) const
{
	if (m_pkCurData->fAttackRange < 300.0f)
	{
		return 0;
	}
	return 1;
}

inline void TwUnitData::SetMissleModel(int iMissileModel)
{
	m_pkCurData->iMissileModel = iMissileModel;
}

inline int TwUnitData::GetMissleModel(void) const
{
	return m_pkCurData->iMissileModel;
}

inline int TwUnitData::GetResMissleModel(void) const
{
	return m_pkCurData->pkRes->uiMissileModel;
}

inline void TwUnitData::SetMissileSpeed(float fSpeed)
{
	m_pkCurData->fMissileSpeed = fSpeed;
}

inline float TwUnitData::GetMissileSpeed(void) const
{
	return m_pkCurData->fMissileSpeed;
}

inline float TwUnitData::GetBaseDamage(void) const
{
	return m_pkCurData->fBaseDamage;
}
inline float TwUnitData::GetAddDamage(void) const
{
	return m_pkCurData->fAddDamage;
}

inline float TwUnitData::GetBaseArmor(void) const
{
	return m_pkCurData->fBaseArmor;
}
inline float TwUnitData::GetAddAromorPer(void) const
{
	return m_pkCurData->fAddArmorPer;
}
inline float TwUnitData::GetAddAntiMagic(void)
{
	return m_pkCurData->fAddMagicArmor;
}
inline float TwUnitData::GetAddAntiMagicPer(void)
{
	return m_pkCurData->fAddMagicArmorPer;
}

inline float TwUnitData::GetBaseMaxHP(void)
{
	return m_pkCurData->fBaseMaxHP;
}
inline float TwUnitData::GetBaseMaxMP(void)
{
	return m_pkCurData->fBaseMaxMP;
}
inline float TwUnitData::GetBaseRegenMP(void)
{
	return m_pkCurData->fBaseRegenMP;
}
inline BeUnitAction TwUnitData::GetActionType(void) const
{
	return m_pkCurData->eActionType;
}

inline bool TwUnitData::IsActionNow(BeUnitAction eAction) const
{
	return (m_pkCurData->eActionType == eAction);
}

inline bool TwUnitData::IsActionCurTimeOut(int iDeltaTime) const
{
	return ((m_pkCurData->iActionCurTime + iDeltaTime) >= m_pkCurData->iActionAllTime);
}
inline int		TwUnitData::GetRealDamagePt()
{
	int iRealPtTime = m_pkCurData->iAttackDamagePt;

	return iRealPtTime;
}
inline int TwUnitData::GetActionCurTimeNeed(void) const
{
	return (m_pkCurData->iActionAllTime - m_pkCurData->iActionCurTime);
}

inline void TwUnitData::IncActionCurTime(int iDeltaTime)
{
	m_pkCurData->iActionCurTime += iDeltaTime;
	m_pkCurData->iAttackElapseTime += iDeltaTime;
}

inline int TwUnitData::GetActionCurTime(void) const
{
	return m_pkCurData->iActionCurTime;
}

inline void TwUnitData::SetActionCurTime(int iActionCurTime)
{
	m_pkCurData->iActionCurTime = iActionCurTime;
}

inline int TwUnitData::GetActionAllTime(void) const
{
	return m_pkCurData->iActionAllTime;
}

inline void TwUnitData::SetActionAllTime(int iTime)
{
	if (m_pkCurData->iActionAllTime != iTime)
	{
	}
	m_pkCurData->iActionAllTime = iTime;
}

inline int TwUnitData::GetAttackElapseTime(void) const
{
	return m_pkCurData->iAttackElapseTime;
}

inline void TwUnitData::ResetAttackElapseTime(void)
{
	m_pkCurData->iAttackElapseTime = 0;
}

inline float TwUnitData::GetPosX(void) const
{
	return m_pkCurData->fPosX;
}

inline float TwUnitData::GetPosY(void) const
{
	return m_pkCurData->fPosY;
}

inline float TwUnitData::GetPosZ(void) const
{
	return m_pkCurData->fPosZ;
}

inline float TwUnitData::GetFace(void) const
{
	return m_pkCurData->fFace;
}

inline void TwUnitData::SetPitch(float fPitch)
{
	m_pkCurData->fPitch = fPitch;
}

inline float TwUnitData::GetPitch(void)
{
	return m_pkCurData->fPitch;
}
inline float TwUnitData::GetTarFace(void) const
{
	return m_pkCurData->fFace;
}
inline float TwUnitData::GetScale(void) const
{
	return m_pkCurData->fScale;
}
inline int TwUnitData::GetAttackingUnitID(void) const
{
	return m_pkBackData->iAttackingUnitID;
}

inline void TwUnitData::SetAttackingUnitID(int iID, bool bIsOrb, bool bAttackPos)
{
	int iOldID = m_pkBackData->iAttackingUnitID;
	m_pkBackData->iAttackingUnitID = iID;
}

inline void TwUnitData::SetUsedSkillPoint(int iUsedPoint)
{
	m_pkBackData->iUsedSkillPoint = iUsedPoint;
}

inline int TwUnitData::GetUsedSkillPoint(void) const
{
	return m_pkBackData->iUsedSkillPoint;
}

inline int		TwUnitData::GetSkillPoint()
{
	return m_pkBackData->iLevel - m_pkBackData->iUsedSkillPoint;
}
inline const void TwUnitData::SetSkillDamagePer(float fPer) const
{
	m_pkCurData->fSkillDamagePer = fPer;
}

inline const float TwUnitData::GetSkillDamagePer(void) const
{
	return m_pkCurData->fSkillDamagePer;
}

inline const float TwUnitData::GetMagicDamage(void) const
{
	return m_pkCurData->fMagicDamage;
}

inline	const	float	TwUnitData::GetDamagePer()	const
{
	return m_pkCurData->fDamagePer;
}

inline	const	float	TwUnitData::GetBeDamagePer()	const
{
	return m_pkCurData->fBeDamagePer;
}

inline const float TwUnitData::GetSkillBlastPer(void) const
{
	return m_pkCurData->fSkillBlastPer;
}

inline const float TwUnitData::GetSkillBlastDmgPer(void) const
{
	return 1.5f;
}

inline void TwUnitData::SetAntiMagic(float fAntiMagic)
{
	m_pkCurData->fMagicArmor = fAntiMagic;
}

inline const float TwUnitData::GetMagicArmor(void) const
{
	return m_pkCurData->fMagicArmor;
}

inline const float TwUnitData::GetAddMagicArmor(void) const
{
	return m_pkCurData->fAddMagicArmor;
}

inline const float TwUnitData::GetDecMagicArmor(void) const
{
	return m_pkCurData->fDecAntiMagic;
}

inline const float TwUnitData::GetLeech(void) const
{
	return m_pkCurData->fLeech;
}

inline const float TwUnitData::GetMagicLeech(void) const
{
	return m_pkCurData->fMagicLeech;
}

inline const float TwUnitData::GetToughness(void) const
{
	return m_pkCurData->fToughness;
}
inline const float TwUnitData::GetDecArmor(void) const
{
	return m_pkCurData->fDecArmor;
}
inline const float TwUnitData::GetPerDecArmor(void) const
{
	return m_pkCurData->fPerDecArmor;
}
inline const	float	TwUnitData::GetPerDecMagicArmor()	const
{
	return m_pkCurData->fPerDecMagicArmor;
}
inline const float TwUnitData::GetDecMPCost(void) const
{
	return m_pkCurData->fDecMPCost;
}
inline const float TwUnitData::GetDecCDTime(void) const
{
	return m_pkCurData->fDecCDTime;
}
inline float	TwUnitData::GetPerCDTime()
{
	return m_pkCurData->fPerCDTime;
}
inline float	TwUnitData::GetBaoJiRate()	const
{
	return m_pkCurData->fBaoJi;
}
inline float	TwUnitData::GetBaoJiDamagePer()	const
{
	return m_pkCurData->fBaoJiDamagePer;
}
inline const float TwUnitData::GetAddEnmityPoint(void) const
{
	return m_pkCurData->fEnmityPoint;
}

inline const float TwUnitData::GetPerDamageReduce(void) const
{
	return m_pkCurData->fPerDamageReduce;
}
inline const float TwUnitData::GetEvadeRate(void) const
{
	return m_pkCurData->fEvadeRate;
}
