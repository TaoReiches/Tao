/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_MapItem.h"
#include "TW_Main.h"
#include "TW_Functions.h"
#include "TW_ShareUnitData.h"
#include "TW_MapItemDefine.h"

BeMapItem::BeMapItem(int iID) : BeSingleLinkEntity(iID)
{
	m_pkRes.reset();
	m_iLife = 2;
	m_iLiveTime = -1;
	m_bCanPickUp = true;
}

BeMapItem::~BeMapItem(void)
{
}

bool BeMapItem::Initialize(int iTypeID)
{
	m_pkRes = ItemTableMgr::Get()->GetItemTable(iTypeID);
	if (!m_pkRes)
	{
		return false;
	}
	m_kData.iItemProperty = m_pkRes->uiItemProperty;
	m_kData.iUseCount = m_pkRes->uiOrgUseCount;
	m_kData.iPileCount = m_pkRes->uiOrgPileCount;
	m_kData.iBuyTime = 1;

	m_iLife = 100;
	m_iTypeID = iTypeID;
	m_iFlag = 0;
	SetFace(1.5f * D3DX_PI);

	return true;
}

void BeMapItem::SetPosition(float fX, float fY, float fFace, float fZ)
{
	m_kData.fPosX = fX;
	m_kData.fPosY = fY;
	m_kData.fPosZ = fZ;
	m_kData.fFace = fFace;

	BeShareMapItemData	kData;
	kData.iLogicID = GetID();
	kData.fPosX = GetPosX();
	kData.fPosY = GetPosY();
	kData.iItemTypeID = GetTypeID();
	kData.bRemove = false;

	// gMain->AddMapItemData(kData);
}

void BeMapItem::Update(int iDeltaTime)
{
	if (gTime >= GetRecycleTime())
	{
		SetFlag(BIF_REMOVE);
	}

	BeEntity::Update(iDeltaTime);
}

void BeMapItem::ClrAllPureData()
{
	m_iChangeFlag = 0;
}
