#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Carry.h"
#include "TW_ItemDefine.h"
#include "TW_MemoryPool.h"

class BeSkill;
class ItemTable;
class ZhanChangItemTable;

class BeCarrySkill : public BeCarry
{
    friend class BeUnit;
public:
    BeCarrySkill(int iID);
    ~BeCarrySkill();

    const std::vector<BeSkill*>& GetAllSkill(void) const;
protected:
    std::vector<BeSkill*>	m_akSkill;
};
class BeItem : public BeCarrySkill
{
    friend class BeUnit;

public:
    BeItem(int iID);
    ~BeItem(void);


    inline const ItemTable* GetResPtr(void) const
    {
        return m_pkRes;
    }

    inline void SetResPtr(const ItemTable* pkRes)
    {
        m_pkRes = pkRes;
    }
    inline const ZhanChangItemTable* GetZhanChangResPtr(void) const
    {
        return m_pkZRes;
    }

    inline void SetZhanChangResPtr(const ZhanChangItemTable* pkRes)
    {
        m_pkZRes = pkRes;
    }

    int GetCDSkillID()
    {
        return m_iSkillTypeID;
    }

    inline bool HasProperty(int iProperty) const
    {
        return (m_kData.iItemProperty & iProperty) != 0;
    }

    inline int AddProperty(int iProperty)
    {
        m_kData.iItemProperty |= iProperty;
        return m_kData.iItemProperty;
    }

    inline int DelProperty(int iProperty)
    {
        m_kData.iItemProperty &= ~iProperty;
        return m_kData.iItemProperty;
    }

    inline bool IsUsable(void) const
    {
        return false;
    }

    inline bool IsCanSplice(void) const
    {
        if (m_pkRes)
        {
            return m_pkRes->iPileCount > 1;
        }
        else if (m_pkZRes)
        {
            return m_pkZRes->iPileCount > 1;
        }
        return NULL;
    }

    inline int GetAttachSkillID(int iPos) const
    {
        if (iPos < 0 || iPos > 5)
        {
            return 0;
        }

        return m_pkRes->iItemSkill[iPos];
    }

    inline int GetPriceOfBuy(void) const
    {
        return m_pkRes->iItemPrice;
    }
    inline int GetOrgPileCount(void) const
    {
        return m_pkRes->iOrgPileCount;
    }

    inline int GetOrgUseCount(void) const
    {
        if (m_pkRes)
        {
            return m_pkRes->iOrgUseCount;
        }
        else if (m_pkZRes)
        {
            return m_pkZRes->iOrgUseCount;
        }
        return -1;
    }

    inline int GetMaxPileCount(void) const
    {
        return m_pkRes->iPileCount;
    }

    inline int GetStockCD(void) const
    {
        return m_pkRes->iStockCD;
    }

    inline int GetZItemTypeA(void) const
    {
        return m_pkZRes->uiAttrTypeA;
    }
    inline int GetZItemTypeB(void) const
    {
        return m_pkZRes->uiAttrTypeB;
    }
    inline int GetZItemTypeC(void) const
    {
        return m_pkZRes->uiAttrTypeC;
    }
    inline float GetZItemValueA(int iStrengLv) const
    {
        return m_pkZRes->fAttrGrowValueA * iStrengLv;
    }
    inline float GetZItemValueB(int iStrengLv) const
    {
        return m_pkZRes->fAttrGrowValueB * iStrengLv;
    }
    inline float GetZItemValueC(int iStrengLv) const
    {
        return m_pkZRes->fAttrGrowValueC * iStrengLv;
    }

    inline void SetPackagePos(int iPos)
    {
        m_kData.iPackagePos = iPos;
    }

    inline int GetPackagePos(void) const
    {
        return m_kData.iPackagePos;
    }

    inline void SetPileCount(int iPileCount)
    {
        m_kData.iPileCount = iPileCount;
    }

    inline int GetPileCount(void) const
    {
        return m_kData.iPileCount;
    }

    inline void SetUseCount(int iUseCount)
    {
        m_kData.iUseCount = iUseCount;
    }

    inline int GetUseCount(void) const
    {
        return m_kData.iUseCount;
    }

    inline void SetSpecialNums(int iSpecialNums)
    {
        m_kData.iSpecialNums = iSpecialNums;
    }

    inline int GetSpecialNums(void) const
    {
        return m_kData.iSpecialNums;
    }

    inline void SetOwnerPlay(int iOwnerPlay)
    {
        m_kData.iOwnerPlay = iOwnerPlay;
    }

    inline int GetOwnerPlay(void) const
    {
        return m_kData.iOwnerPlay;
    }

    inline void SetLastOwnerPlayer(int iOwnerPlay)
    {
        m_kData.iLastOwnerPlay = iOwnerPlay;
    }

    inline int GetLastOwnerPlay(void) const
    {
        return m_kData.iLastOwnerPlay;
    }

    inline void SetValidTime(int iValidTime)
    {
        m_kData.iValidTime = iValidTime;
    }

    inline int GetValidTime(void)
    {
        return m_kData.iValidTime;
    }

    inline void SetLastValidTime(int iValidTime)
    {
        m_kData.iLastValidTime = iValidTime;
    }

    inline int GetLastValidTime(void) const
    {
        return m_kData.iLastValidTime;
    }

    inline void SetOrgValid(bool bValid)
    {
        m_kData.bValid = bValid;
    }

    inline bool GetOrgValid(void) const
    {
        return m_kData.bValid;
    }

    inline void SetCanSell(bool bCan)
    {
        m_kData.bCanSell = bCan;
    }

    inline void SetCanShare(bool bShare)
    {
        m_kData.bCanShare = bShare;
    }

    inline bool CanShare(void) const
    {
        return m_kData.bCanShare;
    }

    inline void SetData(int iData)
    {
        m_kData.iData = iData;
    }

    inline int GetData(void) const
    {
        return m_kData.iData;
    }

    inline void SetItemData(const BeItemData& kData)
    {
        int iPackagePos = m_kData.iPackagePos;
        m_kData = kData;
        m_kData.iPackagePos = iPackagePos;
    }

    inline const BeItemData& GetItemData(void) const
    {
        return m_kData;
    }

    inline void SetDataTypeID(int iTypeID)
    {
        m_kData.iItemTypeID = iTypeID;
    }

    inline const int GetProperty(void) const
    {
        return m_kData.iItemProperty;
    }

    inline void SetProperty(int iProperty)
    {
        m_kData.iItemProperty = iProperty;
    }

    inline void SetBuyTime(int iTime)
    {
        m_kData.iBuyTime = iTime;
        if (iTime == 1)
        {
            SetBuyInDead(false);
        }
    }

    inline int GetBuyTime(void)
    {
        return m_kData.iBuyTime;
    }

    inline void SetRefreshTime(unsigned int uiTime)
    {
        m_kData.uiRefreshTime = uiTime;
    }

    inline unsigned int GetRefreshTime(void)
    {
        return m_kData.uiRefreshTime;
    }

    inline void SetRecycleTime(unsigned int uiTime)
    {
        m_kData.uiRecycleTime = uiTime;
    }

    inline unsigned GetRecycleTime(void)
    {
        return m_kData.uiRecycleTime;
    }

    inline void SetNeedRefreshBuyTime(bool bRefresh)
    {
        m_kData.bNeedRefreshBuyTime = bRefresh;
    }

    inline bool NeedRefreshBuyTime()
    {
        return m_kData.bNeedRefreshBuyTime;
    }

    inline void SetBuyInDead(bool bBuyInDead)
    {
        m_kData.bDeadBuyIn = bBuyInDead;
    }

    inline bool GetBuyInDead(void)
    {
        return m_kData.bDeadBuyIn;
    }

    inline void SetDirectBuyOrgUnitID(int iUnitID)
    {
        m_kData.iDirectBuyOrgUnitID = iUnitID;
    }

    inline int GetDirectBuyOrgUnitID(void)
    {
        return m_kData.iDirectBuyOrgUnitID;
    }

    inline void SetAscriptioneTime(unsigned int uiNowTime)
    {
        m_kData.uiAscriptionTime = uiNowTime;
    }

    inline bool GetAscriptioned(unsigned int uiNowTime)
    {
        if (m_kData.uiAscriptionTime == 0 || int(uiNowTime - m_kData.uiAscriptionTime) >= 30 * 1000)
        {
            return true;
        }
        return false;
    }

    inline int GetRSGroupIndex()
    {
        return m_kData.iRSGroupIndex;
    }

    inline void SetDropSkillTypeID(int iTypeID)
    {
        m_kData.iDropSkillTypeID = iTypeID;
    }

    inline int GetDropSkillTypeID()
    {
        return m_kData.iDropSkillTypeID;
    }

    inline void SetFCampData(int iFCamp)
    {
        m_kData.iFCampData = iFCamp;
    }

    inline int GetFCampData(void)
    {
        return m_kData.iFCampData;
    }

    inline int GetUniqueID(void)
    {
        return m_iUniqueID;
    }
    inline void SetUniqueID(int iUniqueID)
    {
        m_iUniqueID = iUniqueID;
    }

    virtual bool Initialize(int iTypeID);
    bool	CDComplete(int iCoolTime)const;

    bool	GetCanSell(void) const;
    void	Update(BeUnit* pkUnit, int iDeltaTime);

    bool	GetValid(void);
    bool    GetIsUsableJudge(void);

    BeSkill* GetSkillByTypeID(int iSkillTypeID);
    int		GetUseSkillTypeID(void) const;

    bool IsUnitZhuanShu(BeUnit* pkUnit);
    void SetItemLeveUpState(bool bLevelUp);
    bool IsItemLevelUp();

    void CheckItemSkillCD(BeUnit* pkUnit);

    bool		IsComposeUse()
    {
        return m_bComposeFlag;
    }
    void		SetComposeUse(bool bValue)
    {
        m_bComposeFlag = bValue;
    }

    void		InitItemSkill();

    inline	void			AddForbidSkill(int iSkillTypeID);
    inline	bool			IsForbidSkill(int iSkillTypeID);
    inline	void			ClearForbidSkill();
    inline	void			SetLastUseTime(int iLastTime);
    inline	int				GetLastUseTime(void)	const;
    inline	int				GetItemAllCDTime()	const;
    inline	void			SetItemCDTime(int iTime);
protected:
    const ItemTable* m_pkRes;
    const ZhanChangItemTable* m_pkZRes;
    BeItemData				m_kData;

    int						m_iUniqueID;
    int						m_iSkillCDTime;
    int						m_iSkillTypeID;
    bool					m_bComposeFlag;

    std::vector<int>		m_kFrobidSkill;
};

inline	void			BeItem::AddForbidSkill(int iSkillTypeID)
{
    m_kFrobidSkill.push_back(iSkillTypeID);
}
inline	bool			BeItem::IsForbidSkill(int iSkillTypeID)
{
    if (!m_kFrobidSkill.empty())
    {
        for (int iIndex = 0; iIndex < m_kFrobidSkill.size(); ++iIndex)
        {
            if (m_kFrobidSkill[iIndex] == iSkillTypeID)
            {
                return true;
            }
        }
    }
    return false;
}
inline	void			BeItem::ClearForbidSkill()
{
    if (!m_kFrobidSkill.empty())
    {
        m_kFrobidSkill.clear();
    }
}
inline	void			BeItem::SetLastUseTime(int iLastTime)
{
    m_kData.iLastUseTime = iLastTime;
}
inline	int				BeItem::GetLastUseTime(void)	const
{
    return m_kData.iLastUseTime;
}
inline	int				BeItem::GetItemAllCDTime()	const
{
    return m_iSkillCDTime;
}
inline	void			BeItem::SetItemCDTime(int iTime)
{
    m_iSkillCDTime = iTime;
}

class BeEquip : public BeCarrySkill
{
    friend class BeUnit;
public:
    DECLARE_POOL1(BeEquip);

    BeEquip(int iID);
    ~BeEquip(void);
    virtual	bool Initialize(int iTypeID);
    bool	Strengthen(int iCardID, int iStrengthenID);
    //	void	SetValueByAttr(SeTownItemDataType eType,const std::string& rkValue);
    void	SetSuitNumber(int iNumber);
    void	SetRandomID(int iID);

protected:
    int		m_iSuitNumber;
    int		m_iRandomID;
};
