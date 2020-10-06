/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_MapItemMgr.h"
#include "TW_MapItem.h"

BeMapItemMgr::BeMapItemMgr(void)
{
}

BeMapItemMgr::~BeMapItemMgr(void)
{
	Clear();
}

bool BeMapItemMgr::Initialize(void)
{
	Clear();

	return BeEntityMgr::Initialize();
}

void BeMapItemMgr::Update(int iDeltaTime)
{
	BeEntityMgr::Update(iDeltaTime);

	for (std::map<int, BeMapItem*>::iterator itr = m_kID2MapItem.begin(); itr != m_kID2MapItem.end();)
	{
		BeMapItem* pkMapItem = itr->second;
		++itr;

		const ItemTable* pkRes = pkMapItem->GetItemRes();
		if (!pkRes)
		{
			continue;
		}

		if (pkMapItem->HasFlag(BIF_REMOVE))
		{
			DelMapItem(pkMapItem->GetID());
		}
		else
		{
			pkMapItem->Update(iDeltaTime);
		}
	}
}

void BeMapItemMgr::Finialize(void)
{
	Clear();

	BeEntityMgr::Finialize();
}

void BeMapItemMgr::Clear()
{
	for (std::map<int, BeMapItem*>::iterator itr = m_kID2MapItem.begin(); itr != m_kID2MapItem.end(); ++itr)
	{
		delete itr->second;
	}
	m_kID2MapItem.clear();
}

BeMapItem* BeMapItemMgr::NewMapItem(int iID)
{
	BeMapItem* pkMapItem = new BeMapItem(iID);
	pkMapItem->AttachMain(pkAttachMain);
	return pkMapItem;
}

BeMapItem* BeMapItemMgr::AddMapItem(int iTypeID, bool bDrop, int iGroup, int iPlayer)
{
	int iID = gMain.GenerateID(GIT_MAPITEM);

	BeMapItem* pkMapItem = NewMapItem(iID);
	pkMapItem->AttachMain(pkAttachMain);
	pkMapItem->SetShowGroup(iGroup);
	pkMapItem->SetShowPlayer(iPlayer);

	if (pkMapItem->Initialize(iTypeID))
	{
		m_kID2MapItem[iID] = pkMapItem;

		EsHandle kHandle((EsObjectType)5, pkMapItem->GetID(), 0);
		pkMapItem->SetHandle(kHandle);

		PushNewMapItem(iID);
		return pkMapItem;
	}
	else
	{
		SAFE_DELETE(pkMapItem);
		return NULL;
	}
}

void BeMapItemMgr::DelMapItem(int iID)
{
	std::map<int, BeMapItem*>::iterator itr = m_kID2MapItem.find(iID);
	if (itr != m_kID2MapItem.end())
	{
		BeMapItem* pkMapItem = itr->second;

		SAFE_DELETE(pkMapItem);
		m_kID2MapItem.erase(itr);

		PushDelMapItem(iID);

		BeShareMapItemData kData;
		kData.bRemove = true;
		kData.iLogicID = iID;
		gMain.AddMapItemData(kData);
	}
}

BeMapItem* BeMapItemMgr::GetMapItemByID(int iID)
{
	if (m_kID2MapItem.find(iID) == m_kID2MapItem.end())
	{
		return NULL;
	}
	return m_kID2MapItem[iID];
}

BeMapItem* BeMapItemMgr::GetMapItemByGuanQiaID(int iID)
{
	std::map<int, BeMapItem*>::iterator iter = m_kID2MapItem.begin();
	for (; iter != m_kID2MapItem.end(); iter++)
	{
		BeMapItem* pkItem = iter->second;
		if (pkItem)
		{
			if (pkItem->GetGuanQiaID() == iID)
			{
				return pkItem;
			}
		}
	}
	return NULL;
}

bool BeMapItemMgr::HasMapItem(int iTypeID)
{
	for (std::map<int, BeMapItem*>::iterator itr = m_kID2MapItem.begin(); itr != m_kID2MapItem.end(); ++itr)
	{
		BeMapItem* pkMapItem = itr->second;
		if (pkMapItem && pkMapItem->GetTypeID() == iTypeID)
		{
			return true;
		}
	}

	return false;
}

void BeMapItemMgr::GetAreaGroup(std::vector<BeMapItem*>& kGroup, float fX, float fY, float fRadius, int iPlayer) const
{
	kGroup.clear();
	fRadius *= fRadius;
	for (std::map<int, BeMapItem*>::const_iterator itr = m_kID2MapItem.begin(); itr != m_kID2MapItem.end(); ++itr)
	{
		BeMapItem* pkItem = itr->second;
		if (pkItem)
		{
			int iTypeID = pkItem->GetTypeID();
			if (pkItem->GetOwnerPlay() == iPlayer || iPlayer == -1 ||
				iTypeID == 'I150' || iTypeID == 'I151' || iTypeID == 'I152' || iTypeID == 'I153')
			{
				float fDist2 = GetDistance2(fX, fY, pkItem->GetPosX(), pkItem->GetPosY());
				if (fDist2 <= fRadius)
				{
					kGroup.push_back(pkItem);
				}
			}
		}
	}
	std::sort(kGroup.begin(), kGroup.end(), MapItem2Pos_LessThan(fX, fY));
}

void BeMapItemMgr::GetGroupByType(std::vector<BeMapItem*>& kGroup, int iTypeID)
{
	kGroup.clear();
	for (std::map<int, BeMapItem*>::const_iterator itr = m_kID2MapItem.begin(); itr != m_kID2MapItem.end(); ++itr)
	{
		BeMapItem* pkItem = itr->second;
		if (pkItem && pkItem->GetTypeID() == iTypeID)
		{
			kGroup.push_back(pkItem);
		}
	}
}

std::map<int, BeMapItem*>& BeMapItemMgr::GetAllMapItem()
{
	return m_kID2MapItem;
}

void BeMapItemMgr::ClrAllPureMapItem()
{
	for (std::map<int, BeMapItem*>::iterator itr = m_kID2MapItem.begin(); itr != m_kID2MapItem.end(); itr++)
	{
		BeMapItem* pkMapItem = itr->second;
		if (pkMapItem)
		{
			pkMapItem->ClrAllPureData();
		}
	}

	m_akPureNewMapitemID.clear();
	m_akPureDelMapitemID.clear();
}

void BeMapItemMgr::PushNewMapItem(int iID)
{
	m_akPureNewMapitemID.push_back(iID);
}

std::vector<int>& BeMapItemMgr::PureGetNewMapitemID()
{
	return m_akPureNewMapitemID;
}

void BeMapItemMgr::PushDelMapItem(int iID)
{
	m_akPureDelMapitemID.push_back(iID);
}

std::vector<int>& BeMapItemMgr::PureGetDelMapitemID()
{
	return m_akPureDelMapitemID;
}

void BeMapItemMgr::GetAllShareMapItemData(std::vector<BeShareMapItemData>& rakAllMapItemData)
{
	rakAllMapItemData.clear();
	BeShareMapItemData	kData;

	for (std::map<int, BeMapItem*>::iterator itr = m_kID2MapItem.begin(); itr != m_kID2MapItem.end(); itr++)
	{
		BeMapItem* pkMapItem = itr->second;
		if (pkMapItem)
		{
			kData.iLogicID = pkMapItem->GetID();
			kData.fPosX = pkMapItem->GetPosX();
			kData.fPosY = pkMapItem->GetPosY();
			kData.iItemTypeID = pkMapItem->GetTypeID();
			kData.bRemove = false;
			kData.iShowGroup = pkMapItem->GetShowGroup();
			kData.iShowPlayer = pkMapItem->GetShowPlayer();
			rakAllMapItemData.push_back(kData);
		}
	}
}
