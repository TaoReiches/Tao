#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Carry.h"
#include "TW_AttackAttr.h"
#include "TW_BuffDefine.h"

class BufferTable;

class BeBuffer : public BeCarry
{
public:
    BeBuffer(int iID);
    ~BeBuffer(void);

    int GetLevel(void) const;
    void SetLevel(int iLevel);
    unsigned int GetRemoveTime(void) const;
    int GetAllLiveTime(void) const;
    void SetAllLiveTime(unsigned int iAllTime);
    int GetCDTime(void) const;
    void SetCDTime(int iCDTime, bool bForceUpdate = true, bool bLastTimeUpdate = false);
    int GetUpdateTime() const;
    void SetUpdateTime(int iUpdateTime);
    bool HasProperty(int iProperty) const;
    void SetSkillRes(const SkillTable* pkRes);
    const SkillTable* GetSkillRes(void) const;
    std::shared_ptr<TwUnit> GetUnit(void) const;
    void SetNeedUpdate(bool bNeedUpdate);
    bool GetNeedUpdate() const;
    void SetSkillTypeID(int iTypeID);
    int GetSkillTypeID(void) const;
    void SetStartTime(int iTime);
    int GetStartTime();
    void SetIntData(int iData);
    int GetIntData(void);
    void SetFloatData(float fData);
    float GetFloatData(void);
    void SetHasDel(bool bHasDel = true);
    bool GetHasDel(void);
    void SetDeadNoRemove(bool bNoRemove);
    bool IsDeadNoRemove();
    bool IsCopyFromOthers(void);
    void SetCopyFromOther(bool bCopy);
    bool IsSkillRealize(void);
    void SetSkillRealize(bool bReal);
    bool HasDecreased(void);
    void SetDecreased(bool bDecre);
    void SetaiData(const std::vector<int>& riData);
    std::vector<int>& GetaiData();
    void    SetafData(const std::vector<float>& riData);
    const   std::vector<float>& GetafData();
    void    SetOrgUnitID(int iUnitID);
    int     GetOrgUnitID() const;
    void	SetInterrupt(bool bFlag);
    bool	IsInterrupt();

    bool IsClientNeed() const;
    void SetRemoveTime(unsigned int uiTime, bool bForceChange = false, bool bNeedRecordChange = true);
    void Update(std::shared_ptr<TwUnit> pkUnit, int iDeltaTime);

    bool Initialize(int iTypeID);
    void	SetUnitID(int iUnitID);
    int		GetBufferUnitID() const;

private:
    std::shared_ptr<const BufferTable> m_pkRes;
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
    BeAttackingAttr	m_kAttr;
};

inline bool	BeBuffer::IsInterrupt()
{
    return m_bIsInterrupt;
}
inline void	BeBuffer::SetInterrupt(bool bFlag)
{
    m_bIsInterrupt = bFlag;
}
inline int BeBuffer::GetOrgUnitID() const
{
    return m_kData.iOrgUnitID;
}
inline void BeBuffer::SetafData(const std::vector<float>& riData)
{
    m_kData.afData = riData;
}
inline const std::vector<float>& BeBuffer::GetafData()
{
    return m_kData.afData;
}
inline bool BeBuffer::HasDecreased(void)
{
    return m_bHasDecreased;
}
inline void BeBuffer::SetDecreased(bool bDecre)
{
    m_bHasDecreased = bDecre;
}
inline void BeBuffer::SetaiData(const std::vector<int>& riData)
{
    m_kData.aiData = riData;
}
inline std::vector<int>& BeBuffer::GetaiData()
{
    return m_kData.aiData;
}
inline bool BeBuffer::GetHasDel(void)
{
    return m_kData.bHasDel;
}
inline void BeBuffer::SetDeadNoRemove(bool bNoRemove)
{
    m_bDeadNoRemove = bNoRemove;
}
inline bool BeBuffer::IsDeadNoRemove()
{
    return m_bDeadNoRemove;
}
inline bool BeBuffer::IsCopyFromOthers(void)
{
    return m_bCopyFromOhers;
}
inline void BeBuffer::SetCopyFromOther(bool bCopy)
{
    m_bCopyFromOhers = bCopy;
}
inline bool BeBuffer::IsSkillRealize(void)
{
    return m_bSkillRealize;
}
inline void BeBuffer::SetSkillRealize(bool bReal)
{
    m_bSkillRealize = bReal;
}
inline int BeBuffer::GetStartTime()
{
    return m_kData.iStartTime;
}
inline void BeBuffer::SetIntData(int iData)
{
    m_kData.iData = iData;
}
inline int BeBuffer::GetIntData(void)
{
    return m_kData.iData;
}
inline void BeBuffer::SetFloatData(float fData)
{
    m_kData.fData = fData;
}
inline float BeBuffer::GetFloatData(void)
{
    return m_kData.fData;
}
inline void BeBuffer::SetHasDel(bool bHasDel)
{
    m_kData.bHasDel = bHasDel;
}
inline void BeBuffer::SetSkillRes(const SkillTable* pkRes)
{
    m_kData.pkSkillRes = pkRes;
}
inline const SkillTable* BeBuffer::GetSkillRes() const
{
    return m_kData.pkSkillRes;
}
inline std::shared_ptr<TwUnit> BeBuffer::GetUnit() const
{
    return m_kData.kUnit;
}
inline void BeBuffer::SetNeedUpdate(bool bNeedUpdate)
{
    m_bNeedUpdate = bNeedUpdate;
}
inline bool BeBuffer::GetNeedUpdate() const
{
    return m_bNeedUpdate;
}
inline void BeBuffer::SetSkillTypeID(int iTypeID)
{
    m_kData.iSkillTypeID = iTypeID;
}
inline int BeBuffer::GetSkillTypeID() const
{
    return m_kData.iSkillTypeID;
}
inline void BeBuffer::SetStartTime(int iTime)
{
    m_kData.iStartTime = iTime;
}
inline int BeBuffer::GetAllLiveTime() const
{
    return m_kData.iAllLiveTime;
}
inline void BeBuffer::SetAllLiveTime(unsigned int iAllTime)
{
    m_kData.iAllLiveTime = iAllTime;
}
inline int BeBuffer::GetCDTime() const
{
    return m_kData.iCDTime;
}
inline void BeBuffer::SetCDTime(int iCDTime, bool bForceUpdate, bool bLastTimeUpdate)
{
    m_kData.iCDTime = iCDTime;
    if (bForceUpdate)
    {
        m_kData.iUpdateTime = iCDTime;
    }
    m_kData.bLastTimeUpdate = bLastTimeUpdate;
}
inline int BeBuffer::GetUpdateTime() const
{
    return m_kData.iUpdateTime;
}
inline void BeBuffer::SetUpdateTime(int iUpdateTime)
{
    m_kData.iUpdateTime = iUpdateTime;
}
inline int BeBuffer::GetLevel() const
{
    return m_kData.iLevel;
}
inline void BeBuffer::SetLevel(int iLevel)
{
    m_kData.iLevel = iLevel;
}
inline unsigned int BeBuffer::GetRemoveTime() const
{
    return m_kData.uiRemoveTime;
}
inline int BeBuffer::GetBufferUnitID() const
{
    return m_kData.iUnitID;
}
inline void BeBuffer::SetOrgUnitID(int iUnitID)
{
    m_kData.iOrgUnitID = iUnitID;
}
inline bool BeBuffer::IsClientNeed() const
{
    return false;
}
