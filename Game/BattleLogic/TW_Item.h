#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CarrySkill.h"
#include "TW_ItemDefine.h"

class ItemTable;

class BeItem : public BeCarrySkill
{
public:
    BeItem(int iID);
    ~BeItem();

    virtual bool Initialize(int iTypeID) override;
    bool	CDComplete(int iCoolTime)const;
    bool	GetCanSell(void) const;
    void	Update(BeUnit* pkUnit, int iDeltaTime);
    bool	GetValid(void);
    bool    GetIsUsableJudge(void);

    std::unique_ptr<BeSkill>&        GetSkillByTypeID(int iSkillTypeID);
    void            SetItemLeveUpState(bool bLevelUp);
    bool            IsItemLevelUp();
    bool		    IsComposeUse();
    void		    SetComposeUse(bool bValue);
    void		    InitItemSkill();
    void			AddForbidSkill(int iSkillTypeID);
    bool			IsForbidSkill(int iSkillTypeID);
    void			ClearForbidSkill();
    void			SetLastUseTime(int iLastTime);
    int				GetLastUseTime(void)	const;
    int				GetItemAllCDTime()	const;
    void			SetItemCDTime(int iTime);
    const std::shared_ptr<const ItemTable>& GetResPtr(void) const;
    void SetResPtr(std::shared_ptr<const ItemTable> pkRes);
    int GetCDSkillID();
    bool HasProperty(int iProperty) const;
    int AddProperty(int iProperty);
    int DelProperty(int iProperty);
    bool IsUsable(void) const;
    bool IsCanSplice(void) const;
    int GetAttachSkillID(int iPos) const;
    int GetPriceOfBuy(void) const;
    int GetOrgPileCount(void) const;
    int GetOrgUseCount(void) const;
    int GetMaxPileCount(void) const;
    void SetPackagePos(int iPos);
    int GetPackagePos(void) const;
    void SetPileCount(int iPileCount);
    int GetPileCount(void) const;
    void SetUseCount(int iUseCount);
    int GetUseCount(void) const;
    void SetSpecialNums(int iSpecialNums);
    int GetSpecialNums(void) const;
    void SetOwnerPlay(int iOwnerPlay);
    int GetOwnerPlay(void) const;
    void SetLastOwnerPlayer(int iOwnerPlay);
    int GetLastOwnerPlay(void) const;
    void SetValidTime(int iValidTime);
    int GetValidTime(void);
    void SetLastValidTime(int iValidTime);
    int GetLastValidTime(void) const;
    void SetOrgValid(bool bValid);
    bool GetOrgValid(void) const;
    void SetCanSell(bool bCan);
    void SetCanShare(bool bShare);
    bool CanShare(void) const;
    void SetData(int iData);
    int GetData(void) const;
    void SetItemData(const BeItemData& kData);
    const BeItemData& GetItemData(void) const;
    void SetDataTypeID(int iTypeID);
    const int GetProperty(void) const;
    void SetProperty(int iProperty);
    void SetBuyTime(int iTime);
    int GetBuyTime(void);
    void SetRefreshTime(unsigned int uiTime);
    unsigned int GetRefreshTime(void);
    void SetRecycleTime(unsigned int uiTime);
    unsigned GetRecycleTime(void);
    void SetNeedRefreshBuyTime(bool bRefresh);
    bool NeedRefreshBuyTime();
    int GetUniqueID(void);
    void SetUniqueID(int iUniqueID);

protected:
    std::shared_ptr<const ItemTable> m_pkRes;
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
inline const std::shared_ptr<const ItemTable>& BeItem::GetResPtr() const
{
    return m_pkRes;
}
inline void BeItem::SetResPtr(std::shared_ptr<const ItemTable> pkRes)
{
    m_pkRes = pkRes;
}
inline int BeItem::GetCDSkillID()
{
    return m_iSkillTypeID;
}
inline bool BeItem::HasProperty(int iProperty) const
{
    return (m_kData.iItemProperty & iProperty) != 0;
}
inline int BeItem::AddProperty(int iProperty)
{
    m_kData.iItemProperty |= iProperty;
    return m_kData.iItemProperty;
}
inline int BeItem::DelProperty(int iProperty)
{
    m_kData.iItemProperty &= ~iProperty;
    return m_kData.iItemProperty;
}
inline bool BeItem::IsUsable(void) const
{
    return false;
}
inline bool BeItem::IsCanSplice(void) const
{
    return false;
}
inline int BeItem::GetAttachSkillID(int iPos) const
{
    if (iPos < 0 || iPos > 5)
    {
        return 0;
    }

    //return m_pkRes->iItemSkill[iPos];
    return 0;
}
inline int BeItem::GetPriceOfBuy(void) const
{
    //return m_pkRes->iItemPrice;
    return 0;
}
inline int BeItem::GetOrgPileCount(void) const
{
    //return m_pkRes->iOrgPileCount;
    return 0;
}
inline int BeItem::GetOrgUseCount(void) const
{
    //if (m_pkRes)
    //{
    //    return m_pkRes->iOrgUseCount;
    //}
    //else if (m_pkZRes)
    //{
    //    return m_pkZRes->iOrgUseCount;
    //}
    return -1;
}
inline int BeItem::GetMaxPileCount(void) const
{
    //return m_pkRes->iPileCount;
    return 0;
}
inline void BeItem::SetPackagePos(int iPos)
{
    m_kData.iPackagePos = iPos;
}
inline int BeItem::GetPackagePos(void) const
{
    return m_kData.iPackagePos;
}
inline void BeItem::SetPileCount(int iPileCount)
{
    m_kData.iPileCount = iPileCount;
}
inline int BeItem::GetPileCount(void) const
{
    return m_kData.iPileCount;
}
inline void BeItem::SetUseCount(int iUseCount)
{
    m_kData.iUseCount = iUseCount;
}
inline int BeItem::GetUseCount(void) const
{
    return m_kData.iUseCount;
}
inline void BeItem::SetSpecialNums(int iSpecialNums)
{
    m_kData.iSpecialNums = iSpecialNums;
}
inline int BeItem::GetSpecialNums(void) const
{
    return m_kData.iSpecialNums;
}
inline void BeItem::SetOwnerPlay(int iOwnerPlay)
{
    m_kData.iOwnerPlay = iOwnerPlay;
}
inline int BeItem::GetOwnerPlay(void) const
{
    return m_kData.iOwnerPlay;
}
inline void BeItem::SetLastOwnerPlayer(int iOwnerPlay)
{
    m_kData.iLastOwnerPlay = iOwnerPlay;
}
inline int BeItem::GetLastOwnerPlay(void) const
{
    return m_kData.iLastOwnerPlay;
}
inline void BeItem::SetValidTime(int iValidTime)
{
    m_kData.iValidTime = iValidTime;
}
inline int BeItem::GetValidTime(void)
{
    return m_kData.iValidTime;
}
inline void BeItem::SetLastValidTime(int iValidTime)
{
    m_kData.iLastValidTime = iValidTime;
}
inline int BeItem::GetLastValidTime(void) const
{
    return m_kData.iLastValidTime;
}
inline void BeItem::SetOrgValid(bool bValid)
{
    m_kData.bValid = bValid;
}
inline bool BeItem::GetOrgValid(void) const
{
    return m_kData.bValid;
}
inline void BeItem::SetCanSell(bool bCan)
{
    m_kData.bCanSell = bCan;
}
inline void BeItem::SetCanShare(bool bShare)
{
    m_kData.bCanShare = bShare;
}
inline bool BeItem::CanShare(void) const
{
    return m_kData.bCanShare;
}
inline void BeItem::SetData(int iData)
{
    m_kData.iData = iData;
}
inline int BeItem::GetData(void) const
{
    return m_kData.iData;
}
inline void BeItem::SetItemData(const BeItemData& kData)
{
    int iPackagePos = m_kData.iPackagePos;
    m_kData = kData;
    m_kData.iPackagePos = iPackagePos;
}
inline const BeItemData& BeItem::GetItemData(void) const
{
    return m_kData;
}
inline void BeItem::SetDataTypeID(int iTypeID)
{
    m_kData.iItemTypeID = iTypeID;
}
inline const int BeItem::GetProperty(void) const
{
    return m_kData.iItemProperty;
}
inline void BeItem::SetProperty(int iProperty)
{
    m_kData.iItemProperty = iProperty;
}
inline void BeItem::SetBuyTime(int iTime)
{
    m_kData.iBuyTime = iTime;
}
inline int BeItem::GetBuyTime(void)
{
    return m_kData.iBuyTime;
}
inline void BeItem::SetRefreshTime(unsigned int uiTime)
{
    m_kData.uiRefreshTime = uiTime;
}
inline unsigned int BeItem::GetRefreshTime(void)
{
    return m_kData.uiRefreshTime;
}
inline void BeItem::SetRecycleTime(unsigned int uiTime)
{
    m_kData.uiRecycleTime = uiTime;
}
inline unsigned BeItem::GetRecycleTime(void)
{
    return m_kData.uiRecycleTime;
}
inline void BeItem::SetNeedRefreshBuyTime(bool bRefresh)
{
    m_kData.bNeedRefreshBuyTime = bRefresh;
}
inline bool BeItem::NeedRefreshBuyTime()
{
    return m_kData.bNeedRefreshBuyTime;
}
inline int BeItem::GetUniqueID(void)
{
    return m_iUniqueID;
}
inline void BeItem::SetUniqueID(int iUniqueID)
{
    m_iUniqueID = iUniqueID;
}
inline bool		BeItem::IsComposeUse()
{
    return m_bComposeFlag;
}
inline void		BeItem::SetComposeUse(bool bValue)
{
    m_bComposeFlag = bValue;
}
