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
class TePtParam;
#ifdef _WIN32
extern enum UnitAutoSpellType;
#else
//extern   int UnitAuotSpellType;
#endif
enum BeSkillType
{
    OtherSkill = 0,
    UIShowSkill,
    UnShowSkill,
    GenuisSkill,
    Genus,
};

#define MUTIL_EFFECT		1
#define MUTIL_TARGET		2
#define MUTIL_TARGET_AFTER	3
#define MUTIL_FACE_EFFECT	4
#define ATTACKE_SKILL		5
#define AUTOUSE_SKILL		6
#define BEDAMGED_SKILL		7
#define EFFECT_SKILLCD		8
#define KILL_SKILLCD		9
#define ATTACKE_SKILLCD		10
#define BEDAMGED_SKILLCD	11

#define IMBA_SKILL_MAJIATIME 3000

class BeSkill : public BeCarry
{
public:
    BeSkill(int iTypeID);
    ~BeSkill(void);

    inline const SkillTable* GetResPtr(void) const
    {
        return m_pkRes;
    }

    const int GetOperateType(void) const
    {
        return m_pkRes->uiOperateType;
    }

    bool HasProperty(int iProperty) const
    {
        return (m_pkRes->uiSkillProperty & iProperty) != 0;
    }

    inline bool IsHaloSkill(void) const
    {
        return (m_pkRes->uiSkillProperty & SKILL_SKILLPROPERTY_GUANGHUAN) != 0;
    }

    inline bool IsSwitch(void) const
    {
        return (m_pkRes->uiOperateType == SKILL_OPERATETYPE_KAIGUAN);
    }

    int GetMaxLevel(void) const
    {
        return m_pkRes->iSkillMaxLevel;
    }

    void GetLvlData(SeSkillLvlData& rkLvlData, int iLevl) const
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

    inline int GetLevel(void) const
    {
        return m_kData.iCurLevel;
    }

    inline bool IsActive(void) const
    {
        return m_kData.bIsActive;
    }

    inline void CopySkillAttribute(BeSkill* pkSkill)
    {
        m_kData.iCurLevel = pkSkill->GetLevel();
        m_kData.bIsActive = false;
    }

    inline const SkillTable* GetSkillRes(void) const
    {
        return m_pkRes;
    }

    inline void SetUIShowPos(int iUIShowPos)
    {
        m_kData.iUIShowPos = iUIShowPos;
    }

    inline int GetUIShowPos(void) const
    {
        return m_kData.iUIShowPos;
    }

    inline int GetShareCDSkill(void) const
    {
        return m_iTypeID;
    }

    inline void SetCanForbid(bool bCan)
    {
        m_bCanForbid = bCan;
    }

    inline bool GetCanForbid(void)
    {
        return m_bCanForbid;
    }

    inline void SetAddOnSkill(bool bCan)
    {
        m_bAddOnSkill = bCan;
    }

    inline bool IsAddOnSkill(void)
    {
        return m_bAddOnSkill;
    }

    inline int GetParentSkill(void)
    {
        return m_iParentSkill;
    }

    inline void SetParentSkill(int iParent)
    {
        m_iParentSkill = iParent;
    }

    int		GetAddOnSkillLiveTime()
    {
        return m_pkRes->fValueF[0];
    }

    inline void SetSkillLvlTipDec(const std::string& rkDesc)
    {
        m_kSkillLvlTipDec = rkDesc;
    }

    inline void SetSkillLvlLearnTip(const std::string& rkLearnTip)
    {
        m_kSkillLvlLearnTipDec = rkLearnTip;
    }

    inline int GetUnitSkillType(void)
    {
        return (int)m_eSkillType;
    }

    inline void SetUnitSkillType(BeSkillType eSkillType)
    {
        m_eSkillType = eSkillType;
    }

    int	GetCDTime(BeUnit* pkUnit);

    void SetLevel(int iLevel);
    void SetLastUseTime(BeUnit* pkUnit, int iTime, bool bLearnSkill = false);
    bool CDComplete(BeUnit* pkUnit);
    void Update(BeUnit* pkUnit, int iItemPos = -1);
    int GetLastUseTime(const BeUnit* pkUnit) const;
    int GetCurPileNums(void) const;
    void SetCurPileNums(int iPileNums);

    //	void UpdateSkillLvlTip(BeUnit* pkUnit = NULL);
    const std::string& GetSkillLvlTip(void);
    const std::string& GetSkillLvlLearnTip(void);

    virtual bool Initialize(int iTypeID);
    void SetActive(bool bActive);
    void ReInitAttr(void);

    void DefaultAutoUse(void);
#ifdef _WIN32
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
#else
    void SetAutoSpellFlag(int eType)
    {
        m_kAutoSpellFlag.SetFlag((int)eType);
    }
    void ClrAutoSpellFlag(int eType)
    {
        m_kAutoSpellFlag.ClrFlag((int)eType);
    }

    bool HasAutoSpellFlag(int eType)
    {
        return m_kAutoSpellFlag.HasFlag((int)eType);
    }
#endif
    void SetCoolDown(int iTime);

    void SetLastTrgTime(int iTime) { m_iLastTrgTime = iTime; }

    void	SetUICounter(int iValue)
    {
        m_iSkillUICounter = iValue;
    }
    int		GetUICounter()
    {
        return m_iSkillUICounter;
    }

public:
    virtual		void	HandleEvent(int iEvent, TePtParam& kParam);
    virtual		void	HandleDamageEvent(int iEvent, TePtParam& kParam);
    virtual		void	HandleDamageCDEvent(int iEvent, TePtParam& kParam);
    //	virtual		void	HandleBeDamagedEvent(int iEvent, TePtParam& kParam);
    virtual		void	HandleBeDamagedCDEvent(int iEvent, TePtParam& kParam);

protected:
    const SkillTable* m_pkRes;
    BeSkillData			m_kData;
    std::string			m_kSkillLvlTipDec;
    std::string			m_kSkillLvlLearnTipDec;

    bool				m_bCanForbid;
    int					m_iCurPileNums;
    //bool				m_bTmpFaBao;
    bool				m_bAddOnSkill;
    int					m_iParentSkill;
    BeSkillType			m_eSkillType;
    BeFlagObj			m_kAutoSpellFlag;
    int					m_iLastTrgTime;
    bool				m_bImbaAutoUse;

    int					m_iSkillUICounter;

public:
    int					GetAddonSkillStartTime()
    {
        return m_iAddOnSkillStartTime;
    }
    void				SetAddOnSkillStartTime(int iValue)
    {
        m_iAddOnSkillStartTime = iValue;
    }

private:
    int					m_iAddOnSkillStartTime;
};
