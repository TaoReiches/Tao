#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>

#include "TW_UnitDataDefine.h"

class TwUnitData : public BeMainPtr
{
public:
    void OnInitAttribute(bool bCurrent, bool bNeedRecordChange);
    void OnPropertyUpdate(int iLevel);
    void SetCurExp(int iCurExp);
    void SetLevel(int iLevel, bool bNeedRecordChange);
    void UpdateAttribute(bool bUpdateNormal);

	int GetCurrentTypeID() const;
	void SetCurrentTypeID(int iTypeID);
	void SetBackTypeID(int iTypeID);
	int GetBackTypeID() const;
	bool HasProperty(int iProperty) const;
	bool HasTableProperty(int iValue);
	float GetMissileArc(void);
	float GetBackAttackRange(void) const;
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
	float GetAttackRange(const BeUnit* pkTarget) const;
	void SetAttackRange(float fRange);
	int GetAttackBackPt(void) const;
	int GetAttackDamagePt(void) const;
	int GetUnitCarryFlag(void) const;
	void SetUnitCarryFlag(int iFlag);
	bool HasUnitCarryFlag(int iFlag) const;
	void ClrUnitCarryFlag(int iFlag);
	bool CanAttack(void) const;
	bool CanSpell(void) const;
	bool CanMove(void) const;
	bool IsForbidItem(void) const;
	const std::shared_ptr<BeUnitData> GetBackData() const;
	void SetHP(float fHP, bool bChange = false);
	void SetShareUnitChangeFlag(int iFlag);
	bool HasShareUnitChangeFlag(int iFlag);
	void ClrShareUnitChangeFlag(int iFlag);
	int GetShareUnitChangeFlag() const;
	void AddLevel(int iAddLevel);
	void SetMP(float fMP, bool bChange = true);

protected:
    TwUnitData();
    ~TwUnitData();

protected:
	int							m_iCarryFlag;
	int						m_iShareUnitDataChangeFlag;
    std::shared_ptr<BeUnitData> m_pkBackData;
	std::shared_ptr<BeUnitData> m_pkCurData;
    std::vector<std::shared_ptr<BeUnitData>>					m_akUnitData;
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
		SetShareUnitChangeFlag(BSUDCF_CURMP);
	}
}

inline void TwUnitData::SetMaxHP(float fMaxHP)
{
	if (fMaxHP != m_pkCurData->fMaxHP)
	{
		SetShareUnitChangeFlag(BSUDCF_MAXHP);
	}
	m_pkCurData->fMaxHP = fMaxHP;
}

inline void TwUnitData::SetCurrentTypeID(int iTypeID)
{
	if (m_pkCurData->iTypeID != iTypeID)
	{
		SetShareUnitChangeFlag(BSUDCF_TYPEID);
	}
	m_pkCurData->iTypeID = iTypeID;
}

inline void TwUnitData::SetShareUnitChangeFlag(int iFlag)
{
	m_iShareUnitDataChangeFlag |= iFlag;
}

inline bool TwUnitData::HasShareUnitChangeFlag(int iFlag)
{
	return (m_iShareUnitDataChangeFlag & iFlag) != 0;
}

inline void TwUnitData::ClrShareUnitChangeFlag(int iFlag)
{
	m_iShareUnitDataChangeFlag &= ~iFlag;
}

inline int TwUnitData::GetShareUnitChangeFlag() const
{
	return m_iShareUnitDataChangeFlag;
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

inline int TwUnitData::GetUnitCarryFlag(void) const
{
	return m_iCarryFlag;
}

inline void TwUnitData::SetUnitCarryFlag(int iFlag)
{
	m_iCarryFlag |= iFlag;
}

inline bool TwUnitData::HasUnitCarryFlag(int iFlag) const
{
	return (m_iCarryFlag & iFlag) != 0;
}

inline void TwUnitData::ClrUnitCarryFlag(int iFlag)
{
	m_iCarryFlag &= ~iFlag;
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
		SetShareUnitChangeFlag(BSUDCF_REGENHP);
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
		SetShareUnitChangeFlag(BSUDCF_REGENMP);
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
	return m_pkBackData->pkRes->iDeathMoney;
}

inline int TwUnitData::GetDeathExp(void) const
{
	return m_pkBackData->pkRes->iDeathExp;
}

inline float TwUnitData::GetModelScale(void) const
{
	return m_pkCurData->pkRes->fModelScale;
}

inline int		TwUnitData::GetDamageModle()
{
	return m_pkCurData->pkRes->iTargetDamage;
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
	return m_pkBackData->pkRes->fCollision;
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
	float fSrcArc = m_pkCurData->pkRes->fMissileArc;
	float fValue = fSrcArc * 3.1415926f / 180.0f;
	return fValue;
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
