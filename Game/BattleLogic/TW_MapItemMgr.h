#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <vector>
#include <map>
#include "TW_EntityMgr.h"

class BeMapItem;

class BeMapItemMgr : public BeEntityMgr
{
public:
	BeMapItemMgr(void);
	~BeMapItemMgr(void);

	virtual bool Initialize(void);
	virtual void Update(int iDeltaTime);
	virtual void Finialize(void);

	virtual void Clear();

	BeMapItem* AddMapItem(int iTypeID, bool bDrop = false, int iGroup = -1, int iPlayer = -1);
	void		DelMapItem(int iID);

	BeMapItem* GetMapItemByID(int iID);
	BeMapItem* GetMapItemByGuanQiaID(int iID);
	bool HasMapItem(int iTypeID);
	void GetAreaGroup(std::vector<BeMapItem*>& kGroup, float fX, float fY, float fRadius, int iPlayer) const;
	void GetGroupByType(std::vector<BeMapItem*>& kGroup, int iTypeID);

	std::map<int, BeMapItem*>& GetAllMapItem();

protected:
	virtual BeMapItem* NewMapItem(int iID);

protected:
	std::map<int, BeMapItem*>	m_kID2MapItem;

public:
	void	PushNewMapItem(int iID);
	std::vector<int>& PureGetNewMapitemID();

	void	PushDelMapItem(int iID);
	std::vector<int>& PureGetDelMapitemID();
	void GetAllShareMapItemData(std::vector<BeShareMapItemData>& rakAllMapItemData);

private:
	std::vector<int>	m_akPureNewMapitemID;
	std::vector<int>	m_akPureDelMapitemID;

public:
	void	ClrAllPureMapItem();
};
