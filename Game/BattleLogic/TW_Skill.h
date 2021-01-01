#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <string>
#include "TW_Carry.h"
#include "TW_MemoryPool.h"
#include "TW_SkillDefine.h"
#include "Skill_table.hpp"
#include "TW_ShareUnitData.h"

class BeUnit;
class TwPtParam;

class BeSkill : public BeCarry
{
public:
    BeSkill(int iTypeID);
    ~BeSkill(void);

    virtual bool Initialize(int iTypeID) override;

    const int GetOperateType(void) const;
    bool HasProperty(int iProperty) const;
    bool IsHaloSkill(void) const;
    bool IsSwitch(void) const;
    int GetMaxLevel(void) const;
    void GetLvlData(SeSkillLvlData& rkLvlData, int iLevl) const;
    int GetLevel(void) const;
    bool IsActive(void) const;
    void CopySkillAttribute(BeSkill* pkSkill);
    const std::shared_ptr<const SkillTable> GetSkillRes(void) const;
    void SetUIShowPos(int iUIShowPos);
    int GetUIShowPos(void) const;
    int GetShareCDSkill(void) const;
    void SetCanForbid(bool bCan);
    bool GetCanForbid(void);
    void SetAddOnSkill(bool bCan);
    bool IsAddOnSkill(void);
    int GetParentSkill(void);
    void SetParentSkill(int iParent);
    float		GetAddOnSkillLiveTime();
    int GetUnitSkillType(void);
    void SetUnitSkillType(BeSkillType eSkillType);
    int	GetCDTime(std::shared_ptr<BeUnit> pkUnit);
    void SetLevel(int iLevel);
    void SetLastUseTime(std::shared_ptr<BeUnit> pkUnit, int iTime, bool bLearnSkill = false);
    bool CDComplete(std::shared_ptr<BeUnit> pkUnit);
    void Update(std::shared_ptr<BeUnit> pkUnit, int iItemPos = -1);
    int GetLastUseTime(const std::shared_ptr<BeUnit> pkUnit) const;
    int GetCurPileNums(void) const;
    void SetCurPileNums(int iPileNums);
    void SetActive(bool bActive);
    void ReInitAttr(void);
    void DefaultAutoUse(void);
    void SetLastTrgTime(int iTime);
    void	SetUICounter(int iValue);
    int		GetUICounter();
    int					GetAddonSkillStartTime();
    void				SetAddOnSkillStartTime(int iValue);

protected:
    std::shared_ptr<const SkillTable>   m_pkRes;
    BeSkillData			m_kData;
    bool				m_bCanForbid;
    int					m_iCurPileNums;
    bool				m_bAddOnSkill;
    int					m_iParentSkill;
    BeSkillType			m_eSkillType;
    int					m_iLastTrgTime;
    int					m_iSkillUICounter;
    int					m_iAddOnSkillStartTime;
};

inline int     BeSkill::GetAddonSkillStartTime()
{
    return m_iAddOnSkillStartTime;
}
inline void    BeSkill::SetAddOnSkillStartTime(int iValue)
{
    m_iAddOnSkillStartTime = iValue;
}

inline const int BeSkill::GetOperateType(void) const
{
    return m_pkRes->uiOperateType;
}

inline bool BeSkill::HasProperty(int iProperty) const
{
    return (m_pkRes->uiSkillProperty & iProperty) != 0;
}

inline bool BeSkill::IsHaloSkill(void) const
{
    return false;
}

inline bool BeSkill::IsSwitch(void) const
{
    return (m_pkRes->uiOperateType == E_SKILLTABLE_OPERATETYPE::SKILL_OPERATETYPE_SWITCH);
}

inline int BeSkill::GetMaxLevel(void) const
{
    return m_pkRes->iSkillMaxLevel;
}

inline void BeSkill::GetLvlData(SeSkillLvlData& rkLvlData, int iLevl) const
{
    if (iLevl == -1)
    {
        iLevl = GetLevel() - 1;
    }
    else
    {
        iLevl -= 1;
    }

    if (iLevl > 4)
    {
        iLevl = 4;
    }
    if (iLevl < 0)
    {
        iLevl = 0;
    }

    rkLvlData = SeSkillLvlData(m_pkRes, iLevl);
}

inline int BeSkill::GetLevel(void) const
{
    return m_kData.iCurLevel;
}

inline bool BeSkill::IsActive(void) const
{
    return m_kData.bIsActive;
}

inline void BeSkill::CopySkillAttribute(BeSkill* pkSkill)
{
    m_kData.iCurLevel = pkSkill->GetLevel();
    m_kData.bIsActive = false;
}

inline const std::shared_ptr<const SkillTable> BeSkill::GetSkillRes(void) const
{
    return m_pkRes;
}

inline void BeSkill::SetUIShowPos(int iUIShowPos)
{
    m_kData.iUIShowPos = iUIShowPos;
}

inline int BeSkill::GetUIShowPos(void) const
{
    return m_kData.iUIShowPos;
}

inline int BeSkill::GetShareCDSkill(void) const
{
    return m_iTypeID;
}

inline void BeSkill::SetCanForbid(bool bCan)
{
    m_bCanForbid = bCan;
}

inline bool BeSkill::GetCanForbid(void)
{
    return m_bCanForbid;
}

inline void BeSkill::SetAddOnSkill(bool bCan)
{
    m_bAddOnSkill = bCan;
}

inline bool BeSkill::IsAddOnSkill(void)
{
    return m_bAddOnSkill;
}

inline int BeSkill::GetParentSkill(void)
{
    return m_iParentSkill;
}

inline void BeSkill::SetParentSkill(int iParent)
{
    m_iParentSkill = iParent;
}

inline float		BeSkill::GetAddOnSkillLiveTime()
{
    return m_pkRes->fValueF[0];
}
inline int BeSkill::GetUnitSkillType(void)
{
    return (int)m_eSkillType;
}

inline void BeSkill::SetUnitSkillType(BeSkillType eSkillType)
{
    m_eSkillType = eSkillType;
}
inline void BeSkill::SetLastTrgTime(int iTime)
{
    m_iLastTrgTime = iTime;
}
inline void	BeSkill::SetUICounter(int iValue)
{
    m_iSkillUICounter = iValue;
}
inline int		BeSkill::GetUICounter()
{
    return m_iSkillUICounter;
}
