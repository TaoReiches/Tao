#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <vector>
#include <map>
#include "TW_FlagObject.h"
#include "TW_ChangeFlagObject.h"
#include "TW_LogicBase.h"
#include "TW_CarryDefine.h"
#include "TW_AttackedAttr.h"

class BeUnit;
class SkillTable;

class BeCarry : public BeFlagObj, public BeMainPtr, public BeUnitPtr, public BeChangeFlagObj
{
public:
    BeCarry(int iID);
    virtual ~BeCarry(void);


    inline BeCarryType GetType(void) const
    {
        return m_eType;
    }

    inline int GetID(void) const
    {
        return m_iID;
    }

    inline int GetTypeID(void) const
    {
        return m_iTypeID;
    }

    inline void SetTypeID(int iTypeID)
    {
        m_iTypeID = iTypeID;
    }

    inline int GetCarryFlag(void) const
    {
        return m_iCarryFlag;
    }

    inline int GetImmunityFlag(void) const
    {
        return m_iImmunityFlag;
    }

    inline bool HasCarryFlag(int iFlag) const
    {
        return ((m_iCarryFlag & iFlag) == iFlag);
    }

    inline bool HasImmunityFlag(int iFlag) const
    {
        return ((m_iImmunityFlag & iFlag) == iFlag);
    }

    inline void SetCarryFlag(int iFlag)
    {
        m_iCarryFlag |= iFlag;
    }

    inline void SetImmunityFlag(int iFlag)
    {
        m_iImmunityFlag |= iFlag;
    }

    inline void ClrCarryFlag(int iFlag)
    {
        m_iCarryFlag &= ~iFlag;
    }

    inline void ClrImmunityFlag(int iFlag)
    {
        m_iImmunityFlag &= ~iFlag;
    }

    inline void SetType(BeCarryType eType)
    {
        m_eType = eType;
    }

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
    void	InitNormalAttr(const SkillTable* pkSkillRes, int iSkillLevel, int iSkillTypeID = 0);
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

