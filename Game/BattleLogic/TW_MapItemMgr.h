#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include <vector>
#include <unordered_map>
#include "TW_EntityMgr.h"

class BeMapItem;
class BeShareMapItemData;

class BeMapItemMgr : public BeEntityMgr
{
public:
	BeMapItemMgr(void);
	~BeMapItemMgr(void);

	virtual bool Initialize(void) override;
	virtual void Update(int iDeltaTime) override;
	virtual void Finialize(void) override;

	virtual void Clear();

	std::shared_ptr<BeMapItem> AddMapItem(int iTypeID, bool bDrop = false, int iGroup = -1, int iPlayer = -1);
	void		DelMapItem(int iID);

	std::shared_ptr<BeMapItem> GetMapItemByID(int iID);
	bool HasMapItem(int iTypeID);
	void GetAreaGroup(std::vector<std::shared_ptr<BeMapItem>>& kGroup, float fX, float fY, float fRadius, int iPlayer) const;
	void GetGroupByType(std::vector<std::shared_ptr<BeMapItem>>& kGroup, int iTypeID);

	std::unordered_map<int, std::shared_ptr<BeMapItem>>& GetAllMapItem();
	void	ClrAllPureMapItem();
	void	PushNewMapItem(int iID);
	std::vector<int>& PureGetNewMapitemID();

	void	PushDelMapItem(int iID);
	std::vector<int>& PureGetDelMapitemID();
	void GetAllShareMapItemData(std::vector<BeShareMapItemData*>& rakAllMapItemData);

protected:
	virtual std::shared_ptr<BeMapItem> NewMapItem(int iID);

protected:
	std::unordered_map<int, std::shared_ptr<BeMapItem>>	m_kID2MapItem;

private:
	std::vector<int>	m_akPureNewMapitemID;
	std::vector<int>	m_akPureDelMapitemID;
};
