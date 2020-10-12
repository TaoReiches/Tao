#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

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

    inline int GetOrgPileCount(void) const
    {
        return m_pkRes->iOrgPileCount;
    }

    inline int GetOrgUseCount(void) const
    {
        return m_pkRes->iOrgUseCount;
    }

    inline bool HasProperty(int iProperty)
    {
        return ((m_pkRes->uiItemProperty & iProperty) == iProperty) ? true : false;
    }

    inline int GetOwnerPlay(void) const
    {
        return m_kData.iOwnerPlay;
    }

    inline void SetOwnerPlay(int iPlay)
    {
        m_kData.iOwnerPlay = iPlay;
    }

    inline float GetPosX(void) const
    {
        return m_kData.fPosX;
    }

    inline float GetPosY(void) const
    {
        return m_kData.fPosY;
    }

    inline float GetPosZ(void) const
    {
        return m_kData.fPosZ;
    }

    inline float GetFace(void) const
    {
        return m_kData.fFace;
    }

    inline void SetFace(float fFace)
    {
        AdjustRadian(fFace);

        m_kData.fFace = fFace;
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

    bool IsCanSplice(void) const
    {
        return false;
    }

    bool IsUsable(void) const
    {
        return ((m_pkRes->uiItemProperty & ITEM_ITEMPROPERTY_CANUSE) != 0);
    }

    inline void SetValidTime(int iValidTime)
    {
        m_kData.iValidTime = iValidTime;
    }

    inline int GetValidTime(void) const
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

    inline bool GetCanSell(void) const
    {
        return m_kData.bCanSell;
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

    inline void BeDamage(void)
    {
        --m_iLife;
    }

    inline bool IsDead(void) const
    {
        return (m_iLife <= 0);
    }

    inline void SetBuyTime(int iTime)
    {
        m_kData.iBuyTime = iTime;
        if (iTime == 1)
        {
            m_kData.bDeadBuyIn = false;
        }
    }

    inline int GetBuyTime(void)
    {
        return m_kData.iBuyTime;
    }

    inline void SetRecycleTime(unsigned int uiTime)
    {
        m_kData.uiRecycleTime = uiTime;
    }

    inline unsigned int GetRecycleTime(void)
    {
        return m_kData.uiRecycleTime;
    }

    inline void SetRefreshTime(unsigned int uiTime)
    {
        m_kData.uiRefreshTime = uiTime;
    }

    inline unsigned int GetRefreshTime(void)
    {
        return m_kData.uiRefreshTime;
    }

    inline void SetItemData(const BeItemData& rkData)
    {
        m_kData = rkData;
    }

    inline const BeItemData& GetItemData(void) const
    {
        return m_kData;
    }

    inline int GetLastOwnerPlay(void)
    {
        return m_kData.iLastOwnerPlay;
    }

    inline const ItemTable* GetItemRes(void) const
    {
        return m_pkRes;
    }

    inline void SetFCampData(int iFCamp)
    {
        m_kData.iFCampData = iFCamp;
    }

    inline int GetFCampData(void)
    {
        return m_kData.iFCampData;
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

    inline void SetRSGroupIndex(int iIndex)
    {
        m_kData.iRSGroupIndex = iIndex;
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

    void	SetLiveTime(int iTime)
    {
        m_iLiveTime = iTime;
    }

    void	SetCanPickUp(bool bValue)
    {
        m_bCanPickUp = bValue;
    }
    bool	GetCanPickUp()
    {
        return m_bCanPickUp;
    }

    void	SetGuanQiaID(int iValue)
    {
        m_iGuanQiaID = iValue;
    }
    int		GetGuanQiaID()
    {
        return m_iGuanQiaID;
    }
    void	SetShowGroup(int iGroup)
    {
        m_kData.iShowGroup = iGroup;
    }
    int	    GetShowGroup()
    {
        return m_kData.iShowGroup;
    }
    void	SetShowPlayer(int iPlayer)
    {
        m_kData.iShowPlayer = iPlayer;
    }
    int	    GetShowPlayer()
    {
        return m_kData.iShowPlayer;
    }

    bool Initialize(int iTypeID);
    void Update(int iDeltaTime);

    void SetPosition(float fX, float fY, float fFace = 1.5f * D3DX_PI, float fZ = 0.f);

protected:
    const ItemTable* m_pkRes;
    BeItemData			m_kData;
    int					m_iLife;
    int					m_iLiveTime;
    bool				m_bCanPickUp;
    int					m_iGuanQiaID;

public:
    void									ClrAllPureData();
};
