#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Carry.h"
#include "TW_AttackAttr.h"
#include "TW_BuffDefine.h"

class SeCalSkillLvlData;
class BufferTable;

class BeBuffer : public BeCarry
{
    DECLARE_POOL1(BeBuffer);

public:
    BeBuffer(int iID);
    ~BeBuffer(void);

public:

    inline int GetLevel(void) const
    {
        return m_kData.iLevel;
    }

    inline void SetLevel(int iLevel)
    {
        m_kData.iLevel = iLevel;
    }

    inline unsigned int GetRemoveTime(void) const
    {
        return m_kData.uiRemoveTime;
    }

    inline int GetAllLiveTime(void) const
    {
        return m_kData.iAllLiveTime;
    }

    inline void SetAllLiveTime(unsigned int iAllTime)
    {
        m_kData.iAllLiveTime = iAllTime;
    }

    inline int GetCDTime(void) const
    {
        return m_kData.iCDTime;
    }

    inline void SetCDTime(int iCDTime, bool bForceUpdate = true, bool bLastTimeUpdate = false)
    {
        m_kData.iCDTime = iCDTime;
        if (bForceUpdate)
        {
            m_kData.iUpdateTime = iCDTime;
        }
        else
        {
        }

        m_kData.bLastTimeUpdate = bLastTimeUpdate;
    }

    inline int GetUpdateTime() const
    {
        return m_kData.iUpdateTime;
    }

    inline void SetUpdateTime(int iUpdateTime)
    {
        m_kData.iUpdateTime = iUpdateTime;
    }

    //inline bool HasProperty(int iProperty) const
    //{
    //    return ((m_pkRes->uiProperty & iProperty) == iProperty) ? true : false;
    //}

    //inline int GetSingleSpliceNum(void) const
    //{
    //    return m_pkRes->iSingleSpliceNum;
    //}

    //inline int GetMultiSpliceNum(void) const
    //{
    //    return m_pkRes->iMultiSpliceNum;
    //}

    //inline void SetSkillLvlData(SeCalSkillLvlData* pkData)
    //{
    //    m_kData.kSkillLvlData = *pkData;
    //}

    //inline const SeCalSkillLvlData* GetSkillLvlData(void) const
    //{
    //    return &(m_kData.kSkillLvlData);
    //}

    inline void SetSkillRes(const SkillTable* pkRes)
    {
        m_kData.pkSkillRes = pkRes;
    }

    inline const SkillTable* GetSkillRes(void) const
    {
        return m_kData.pkSkillRes;
    }

    inline BeUnit* GetUnit(void) const
    {
        return m_kData.kUnit.get();
    }

    inline void SetNeedUpdate(bool bNeedUpdate)
    {
        m_bNeedUpdate = bNeedUpdate;
    }

    inline bool GetNeedUpdate() const
    {
        return m_bNeedUpdate;
    }

    inline void SetSkillTypeID(int iTypeID)
    {
        m_kData.iSkillTypeID = iTypeID;
    }

    inline int GetSkillTypeID(void) const
    {
        return m_kData.iSkillTypeID;
    }

    inline void SetHaloBuffer(bool bHalo)
    {
        m_kData.bIsHaloBuffer = bHalo;
    }

    inline bool IsHaloBuffer(void)
    {
        return m_kData.bIsHaloBuffer;
    }

    inline void SetPersistBuffer(bool bPersist)
    {
        m_kData.bIsPersistBuffer = bPersist;
    }

    inline bool IsPersistBuffer(void)
    {
        return m_kData.bIsPersistBuffer;
    }

    inline void SetStartTime(int iTime)
    {
        m_kData.iStartTime = iTime;
    }

    inline int GetStartTime()
    {
        return m_kData.iStartTime;
    }

    inline void SetIntData(int iData)
    {
        m_kData.iData = iData;
    }

    inline int GetIntData(void)
    {
        return m_kData.iData;
    }

    inline void SetFloatData(float fData)
    {
        m_kData.fData = fData;
    }

    inline float GetFloatData(void)
    {
        return m_kData.fData;
    }

    inline void SetHasDel(bool bHasDel = true)
    {
        m_kData.bHasDel = bHasDel;
    }

    inline bool GetHasDel(void)
    {
        return m_kData.bHasDel;
    }

    inline int GetNotInvisCampFlag(void) const
    {
        return m_kData.iNotInivsCampFlag;
    }

    inline bool HasNotInvisCampFlag(int iFlag) const
    {
        return (m_kData.iNotInivsCampFlag & iFlag) == iFlag;
    }

    inline void SetNotInvisCampFlag(int iFlag)
    {
        m_kData.iNotInivsCampFlag |= iFlag;
    }

    inline void SetDeadNoRemove(bool bNoRemove)
    {
        m_bDeadNoRemove = bNoRemove;
    }

    inline bool IsDeadNoRemove()
    {
        return m_bDeadNoRemove;
    }

    inline bool IsCopyFromOthers(void)
    {
        return m_bCopyFromOhers;
    }

    inline void SetCopyFromOther(bool bCopy)
    {
        m_bCopyFromOhers = bCopy;
    }

    inline bool IsSkillRealize(void)
    {
        return m_bSkillRealize;
    }

    inline void SetSkillRealize(bool bReal)
    {
        m_bSkillRealize = bReal;
    }

    inline bool HasDecreased(void)
    {
        return m_bHasDecreased;
    }

    inline void SetDecreased(bool bDecre)
    {
        m_bHasDecreased = bDecre;
    }

    inline void SetaiData(const std::vector<int>& riData)
    {
        m_kData.aiData = riData;
    }

    std::vector<int>& GetaiData()
    {
        return m_kData.aiData;
    }

    inline void SetafData(const std::vector<float>& riData)
    {
        m_kData.afData = riData;
    }

    inline const std::vector<float>& GetafData()
    {
        return m_kData.afData;
    }

    inline void SetShieldDefendType(int DefendType)
    {
        m_kData.iShieldDefendType = DefendType;
    }
    inline bool HasShieldDefendType(int DefendType)
    {
        if (m_kData.iShieldDefendType == BAT_SKILL && DefendType != BAT_SKILL)
        {
            return false;
        }
        return true;
    }

    void SetOrgUnitID(int iUnitID);

    int GetOrgUnitID() const
    {
        return m_kData.iOrgUnitID;
    }

    bool	IsBadBuffer(int iCamp)
    {
        if (m_iOrgCamp == -1)
        {
            return false;
        }
        if (iCamp == m_iOrgCamp)
        {
            return false;
        }

        return true;
    }

    void	SetAttr(BeAttackingAttr& kValue)
    {
        m_kAttr = kValue;
    }
    BeAttackingAttr GetAttr()
    {
        return m_kAttr;
    }

    void	SetInterrupt(bool bFlag)
    {
        m_bIsInterrupt = bFlag;
    }
    bool	IsInterrupt()
    {
        return m_bIsInterrupt;
    }

    bool IsClientNeed() const;
    void SetRemoveTime(unsigned int uiTime, bool bForceChange = false, bool bNeedRecordChange = true);
    void Update(BeUnit* pkUnit, int iDeltaTime);

    bool Initialize(int iTypeID);
    SeCalSkillLvlData* GetSkillLvlData(void);
    void	SetUnitID(int iUnitID);
    int		GetBufferUnitID() const;
protected:
    const BufferTable* m_pkRes;
    BeBufferData	m_kData;
    int				m_iRefreshTime;
    bool			m_bNeedUpdate;
    int				m_iBufferEffectPlayer;
    int				m_iBUfferEffectGroup;
    float			m_fBufferEffectScale;
    bool			m_bDeadNoRemove;
    bool			m_bCopyFromOhers;
    bool			m_bSkillRealize;
    bool			m_bHasDecreased;
    bool			m_bHasBufVoice;
    bool			m_bIsInterrupt;

    int				m_iOrgCamp;
    BeAttackingAttr	m_kAttr;
};
