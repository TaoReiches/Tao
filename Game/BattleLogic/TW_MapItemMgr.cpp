/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_MapItemMgr.h"
#include "TW_MapItem.h"
#include "TW_MapItemDefine.h"
#include "TW_Main.h"
#include <TW_Define.h>
#include "TW_ShareUnitData.h"
#include "TW_SortStruct.h"
#include "TW_MemoryObject.h"

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

	//return TwEntityMgr::Initialize();
    return true;
}

void BeMapItemMgr::Update(int iDeltaTime)
{
	//BeEntityMgr::Update(iDeltaTime);

	for (auto itr = m_kID2MapItem.begin(); itr != m_kID2MapItem.end();)
	{
		auto pkMapItem = itr->second;
		++itr;

		auto pkRes = pkMapItem->GetItemRes();
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

	//TwEntityMgr::Finialize();
}

void BeMapItemMgr::Clear()
{
	for (auto itr = m_kID2MapItem.begin(); itr != m_kID2MapItem.end(); ++itr)
	{
		auto pkMapItem = itr->second;
		if (pkMapItem)
		{
			mpMapItem.free(pkMapItem.get());
			pkMapItem.reset();
		}
	}
	m_kID2MapItem.clear();
}

std::shared_ptr<BeMapItem> BeMapItemMgr::NewMapItem(int iID)
{
	auto pkMapItem = std::shared_ptr<BeMapItem>(mpMapItem.alloc(iID));
	pkMapItem->AttachMain(pkAttachMain);
	return pkMapItem;
}

std::shared_ptr<BeMapItem> BeMapItemMgr::AddMapItem(int iTypeID, bool bDrop, int iGroup, int iPlayer)
{
	int iID = gMain->GenerateID(BeGenIDType::GIT_MAPITEM);

	auto pkMapItem = NewMapItem(iID);
	pkMapItem->AttachMain(pkAttachMain);

	if (pkMapItem->Initialize(iTypeID))
	{
		m_kID2MapItem[iID] = pkMapItem;

		PushNewMapItem(iID);
		return pkMapItem;
	}
	else
	{
		mpMapItem.free(pkMapItem.get());
		pkMapItem.reset();
		return nullptr;
	}
}

void BeMapItemMgr::DelMapItem(int iID)
{
	auto itr = m_kID2MapItem.find(iID);
	if (itr != m_kID2MapItem.end())
	{
		auto pkMapItem = itr->second;

		mpMapItem.free(pkMapItem.get());
		pkMapItem.reset();
		m_kID2MapItem.erase(itr);

		PushDelMapItem(iID);

		BeShareMapItemData kData;
		kData.bRemove = true;
		kData.iLogicID = iID;
		//gMain->AddMapItemData(kData);
	}
}

std::shared_ptr<BeMapItem> BeMapItemMgr::GetMapItemByID(int iID)
{
	if (m_kID2MapItem.find(iID) == m_kID2MapItem.end())
	{
		return nullptr;
	}
	return m_kID2MapItem[iID];
}

bool BeMapItemMgr::HasMapItem(int iTypeID)
{
	for (auto itr = m_kID2MapItem.begin(); itr != m_kID2MapItem.end(); ++itr)
	{
		auto pkMapItem = itr->second;
		if (pkMapItem && pkMapItem->GetTypeID() == iTypeID)
		{
			return true;
		}
	}

	return false;
}

void BeMapItemMgr::GetAreaGroup(std::vector<std::shared_ptr<BeMapItem>>& kGroup, float fX, float fY, float fRadius, int iPlayer) const
{
	kGroup.clear();
	fRadius *= fRadius;
	for (auto itr = m_kID2MapItem.begin(); itr != m_kID2MapItem.end(); ++itr)
	{
		auto pkItem = itr->second;
		if (pkItem)
		{
			int iTypeID = pkItem->GetTypeID();
			if (pkItem->GetOwnerPlay() == iPlayer || iPlayer == -1)
			{
				float fDist2 = GetDistance2(fX, fY, pkItem->GetPosX(), pkItem->GetPosY());
				if (fDist2 <= fRadius)
				{
					kGroup.push_back(pkItem);
				}
			}
		}
	}
}

void BeMapItemMgr::GetGroupByType(std::vector<std::shared_ptr<BeMapItem>>& kGroup, int iTypeID)
{
	kGroup.clear();
	for (auto itr = m_kID2MapItem.begin(); itr != m_kID2MapItem.end(); ++itr)
	{
		auto pkItem = itr->second;
		if (pkItem && pkItem->GetTypeID() == iTypeID)
		{
			kGroup.push_back(pkItem);
		}
	}
}

std::unordered_map<int, std::shared_ptr<BeMapItem>>& BeMapItemMgr::GetAllMapItem()
{
	return m_kID2MapItem;
}

void BeMapItemMgr::ClrAllPureMapItem()
{
	for (auto itr = m_kID2MapItem.begin(); itr != m_kID2MapItem.end(); itr++)
	{
		auto pkMapItem = itr->second;
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

void BeMapItemMgr::GetAllShareMapItemData(std::vector<BeShareMapItemData*>& rakAllMapItemData)
{
	rakAllMapItemData.clear();
	BeShareMapItemData	kData;

	for (auto itr = m_kID2MapItem.begin(); itr != m_kID2MapItem.end(); itr++)
	{
		auto pkMapItem = itr->second;
		if (pkMapItem)
		{
			kData.iLogicID = pkMapItem->GetID();
			kData.fPosX = pkMapItem->GetPosX();
			kData.fPosY = pkMapItem->GetPosY();
			kData.iItemTypeID = pkMapItem->GetTypeID();
			kData.bRemove = false;
			rakAllMapItemData.push_back(&kData);
		}
	}
}
