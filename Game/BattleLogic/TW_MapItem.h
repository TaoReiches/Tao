#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include "TW_Entity.h"
#include "TW_ChangeFlagObject.h"
#include "TW_ItemDefine.h"
#include "Item_table.hpp"
#include "TW_Functions.h"

class BeMapItem : public BeSingleLinkEntity, public BeChangeFlagObj, public BeEntity
{
public:
    BeMapItem(int iID);
    ~BeMapItem(void);

    void Link(float fX, float fY, BeEntityMgr* pkMgr) override {};
    void Unlink(void) override {};
    bool Initialize(int iTypeID);
    void Update(int iDeltaTime);

    int GetOrgPileCount(void) const;
    int GetOrgUseCount(void) const;
    bool HasProperty(int iProperty);
    int GetOwnerPlay(void) const;
    void SetOwnerPlay(int iPlay);
    float GetPosX(void) const;
    float GetPosY(void) const;
    float GetPosZ(void) const;
    float GetFace(void) const;
    void SetFace(float fFace);
    void SetPileCount(int iPileCount);
    int GetPileCount(void) const;
    void SetUseCount(int iUseCount);
    int GetUseCount(void) const;
    bool IsCanSplice(void) const;
    bool IsUsable(void) const;
    void SetValidTime(int iValidTime);
    int GetValidTime(void) const;
    void SetLastValidTime(int iValidTime);
    int GetLastValidTime(void) const;
    void SetOrgValid(bool bValid);
    bool GetOrgValid(void) const;
    void SetCanSell(bool bCan);
    bool GetCanSell(void) const;
    void SetCanShare(bool bShare);
    bool CanShare(void) const;
    void SetData(int iData);
    int GetData(void) const;
    void BeDamage(void);
    bool IsDead(void) const;
    void SetBuyTime(int iTime);
    int GetBuyTime(void);
    void SetRecycleTime(unsigned int uiTime);
    unsigned int GetRecycleTime(void);
    void SetRefreshTime(unsigned int uiTime);
    unsigned int GetRefreshTime(void);
    void SetItemData(const BeItemData& rkData);
    const BeItemData& GetItemData(void) const;
    int GetLastOwnerPlay(void);
    const std::shared_ptr<const ItemTable> GetItemRes(void) const;
    void	SetLiveTime(int iTime);
    void	SetCanPickUp(bool bValue);
    bool	GetCanPickUp();
    void SetPosition(float fX, float fY, float fFace = 1.5f * D3DX_PI, float fZ = 0.f);
    void									ClrAllPureData();

protected:
    std::shared_ptr<const ItemTable> m_pkRes;
    BeItemData			m_kData;
    int					m_iLife;
    int					m_iLiveTime;
    bool				m_bCanPickUp;
};

inline int BeMapItem::GetOrgPileCount(void) const
{
    return m_pkRes->iOrgPileCount;
}

inline int BeMapItem::GetOrgUseCount(void) const
{
    return m_pkRes->iOrgUseCount;
}

inline bool BeMapItem::HasProperty(int iProperty)
{
    return ((m_pkRes->uiItemProperty & iProperty) == iProperty) ? true : false;
}

inline int BeMapItem::GetOwnerPlay(void) const
{
    return m_kData.iOwnerPlay;
}

inline void BeMapItem::SetOwnerPlay(int iPlay)
{
    m_kData.iOwnerPlay = iPlay;
}

inline float BeMapItem::GetPosX(void) const
{
    return m_kData.fPosX;
}

inline float BeMapItem::GetPosY(void) const
{
    return m_kData.fPosY;
}

inline float BeMapItem::GetPosZ(void) const
{
    return m_kData.fPosZ;
}

inline float BeMapItem::GetFace(void) const
{
    return m_kData.fFace;
}

inline void BeMapItem::SetFace(float fFace)
{
    AdjustRadian(fFace);

    m_kData.fFace = fFace;
}

inline void BeMapItem::SetPileCount(int iPileCount)
{
    m_kData.iPileCount = iPileCount;
}

inline int BeMapItem::GetPileCount(void) const
{
    return m_kData.iPileCount;
}

inline void BeMapItem::SetUseCount(int iUseCount)
{
    m_kData.iUseCount = iUseCount;
}

inline int BeMapItem::GetUseCount(void) const
{
    return m_kData.iUseCount;
}

inline bool BeMapItem::IsCanSplice(void) const
{
    return false;
}

inline bool BeMapItem::IsUsable(void) const
{
    return ((m_pkRes->uiItemProperty & ITEM_ITEMPROPERTY_CANUSE) != 0);
}

inline void BeMapItem::SetValidTime(int iValidTime)
{
    m_kData.iValidTime = iValidTime;
}

inline int BeMapItem::GetValidTime(void) const
{
    return m_kData.iValidTime;
}

inline void BeMapItem::SetLastValidTime(int iValidTime)
{
    m_kData.iLastValidTime = iValidTime;
}

inline int BeMapItem::GetLastValidTime(void) const
{
    return m_kData.iLastValidTime;
}

inline void BeMapItem::SetOrgValid(bool bValid)
{
    m_kData.bValid = bValid;
}

inline bool BeMapItem::GetOrgValid(void) const
{
    return m_kData.bValid;
}

inline void BeMapItem::SetCanSell(bool bCan)
{
    m_kData.bCanSell = bCan;
}

inline bool BeMapItem::GetCanSell(void) const
{
    return m_kData.bCanSell;
}

inline void BeMapItem::SetCanShare(bool bShare)
{
    m_kData.bCanShare = bShare;
}

inline bool BeMapItem::CanShare(void) const
{
    return m_kData.bCanShare;
}

inline void BeMapItem::SetData(int iData)
{
    m_kData.iData = iData;
}

inline int BeMapItem::GetData(void) const
{
    return m_kData.iData;
}

inline void BeMapItem::BeDamage(void)
{
    --m_iLife;
}

inline bool BeMapItem::IsDead(void) const
{
    return (m_iLife <= 0);
}

inline void BeMapItem::SetBuyTime(int iTime)
{
    m_kData.iBuyTime = iTime;
}

inline int BeMapItem::GetBuyTime(void)
{
    return m_kData.iBuyTime;
}

inline void BeMapItem::SetRecycleTime(unsigned int uiTime)
{
    m_kData.uiRecycleTime = uiTime;
}

inline unsigned int BeMapItem::GetRecycleTime(void)
{
    return m_kData.uiRecycleTime;
}

inline void BeMapItem::SetRefreshTime(unsigned int uiTime)
{
    m_kData.uiRefreshTime = uiTime;
}

inline unsigned int BeMapItem::GetRefreshTime(void)
{
    return m_kData.uiRefreshTime;
}

inline void BeMapItem::SetItemData(const BeItemData& rkData)
{
    m_kData = rkData;
}

inline const BeItemData& BeMapItem::GetItemData(void) const
{
    return m_kData;
}

inline int BeMapItem::GetLastOwnerPlay(void)
{
    return m_kData.iLastOwnerPlay;
}

inline const std::shared_ptr<const ItemTable> BeMapItem::GetItemRes(void) const
{
    return m_pkRes;
}

inline void	BeMapItem::SetLiveTime(int iTime)
{
    m_iLiveTime = iTime;
}

inline void	BeMapItem::SetCanPickUp(bool bValue)
{
    m_bCanPickUp = bValue;
}
inline bool	BeMapItem::GetCanPickUp()
{
    return m_bCanPickUp;
}
