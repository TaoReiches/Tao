#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <vector>
#include <map>
#include <memory>
#include "TW_FlagObject.h"
#include "TW_ChangeFlagObject.h"
#include "TW_LogicBase.h"
#include "TW_CarryDefine.h"
#include "TW_AttackedAttr.h"

class TwUnit;
class SkillTable;

class BeCarry : public BeFlagObj, public TwMainPtr, public TwUnitPtr, public BeChangeFlagObj
{
public:
    BeCarry(int iID);
    virtual ~BeCarry(void);

    BeCarryType GetType(void) const;
    int GetID(void) const;
    int GetTypeID(void) const;
    void SetTypeID(int iTypeID);
    int GetCarryFlag(void) const;
    int GetImmunityFlag(void) const;
    bool HasCarryFlag(int iFlag) const;
    bool HasImmunityFlag(int iFlag) const;
    void SetCarryFlag(int iFlag);
    void SetImmunityFlag(int iFlag);
    void ClrCarryFlag(int iFlag);
    void ClrImmunityFlag(int iFlag);
    void SetType(BeCarryType eType);

    void	SetNormalAttr(BeNormalAttType eType, float fValue, bool bForceChange = false, bool bAdd = false);
    std::vector<BeNormalAtt>& GetNormalAttr(void);
    void	ApplyNormalAttr(float afChange[BeCarryType::BCT_NUM][BeNormalAttType::NAT_MAX_NUM][2], bool bGhost = false, int iImmunityFlag = 0);
    bool	IsImmunity(const BeNormalAtt& rkAttr, int iImmunityFlag);
    float	GetNormalAttrValue(BeNormalAttType eType);

    void    SetMissileModel(int iMissileModel);
    int		GetMissileModel(void);
    bool	GetAttackMissPhysic(void);

    void	SetAttackedAttr(BeAttackedAttr& rkAttackedAttr);
    std::vector<BeAttackedAttr>& GetAttackedAttr(void);

    float	GetAttackedAvoidPhysic(void);
    float	GetShortRangeAttackedResistPhysic(void);
    BeResistPhysicAttr* GetShortRangeAttackedResistPhysicAttr(void);
    float	GetEjectAttackedResistPhysic(void);
    BeResistPhysicAttr* GetEjectAttackedResistPhysicAttr(void);
    float	GetAttackedAntiPhysic(void);
    float	GetAttackedAntiMagic(void);
    float	GetAttackedAntiLeech(void);
    float	GetAttackedReboundSkill(void);
    float	GetAttackedReboundSkillAll(void);
    float	GetAttackedReboundPhysicShortRange(void);
    void	GetAttackedReboundPhysicBoth(float& fShorRange, float& fEject);
    void	ClearAttr(void);
    void	SetNormalAttrByData(int eType, float fValue, bool bAdd = false);
    virtual bool Initialize(int iTypeID) = 0;

protected:
    void	InitNormalAttr(const std::shared_ptr<const SkillTable>& pkSkillRes, int iSkillLevel, int iSkillTypeID = 0);
    void	InitAttrFromSkill(int iSkillTypeID, int iSkillLevel = 1);

protected:
    BeCarryType		m_eType;
    int				m_iID;
    int				m_iTypeID;
    int				m_iCarryFlag;
    int				m_iImmunityFlag;
    int				m_iOrbSkill;
    int				m_iOrbLevel;
    int				m_iMissileModel;

    std::vector<BeNormalAtt>		m_akNormalAttr;
    std::vector<BeNormalAtt>		m_akBackNormalAttr;
    std::vector<BeAttackedAttr>		m_akAttackedAttr;
};

inline BeCarryType BeCarry::GetType(void) const
{
    return m_eType;
}
inline int BeCarry::GetID(void) const
{
    return m_iID;
}
inline int BeCarry::GetTypeID(void) const
{
    return m_iTypeID;
}
inline void BeCarry::SetTypeID(int iTypeID)
{
    m_iTypeID = iTypeID;
}
inline int BeCarry::GetCarryFlag(void) const
{
    return m_iCarryFlag;
}
inline int BeCarry::GetImmunityFlag(void) const
{
    return m_iImmunityFlag;
}
inline bool BeCarry::HasCarryFlag(int iFlag) const
{
    return ((m_iCarryFlag & iFlag) == iFlag);
}
inline bool BeCarry::HasImmunityFlag(int iFlag) const
{
    return ((m_iImmunityFlag & iFlag) == iFlag);
}
inline void BeCarry::SetCarryFlag(int iFlag)
{
    m_iCarryFlag |= iFlag;
}
inline void BeCarry::SetImmunityFlag(int iFlag)
{
    m_iImmunityFlag |= iFlag;
}
inline void BeCarry::ClrCarryFlag(int iFlag)
{
    m_iCarryFlag &= ~iFlag;
}
inline void BeCarry::ClrImmunityFlag(int iFlag)
{
    m_iImmunityFlag &= ~iFlag;
}
inline void BeCarry::SetType(BeCarryType eType)
{
    m_eType = eType;
}
