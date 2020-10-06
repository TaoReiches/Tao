#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "TW_EntityMgr.h"

class BeUnit;

typedef std::vector<BeUnit*> UnitGroup;
typedef std::vector<int> UnitGroupID;
typedef std::vector<std::shared_ptr<BeUnit>> SharedUnitGroup;
class BeUnitMgr : public BeEntityMgr
{
public:
	BeUnitMgr(void);
	~BeUnitMgr(void);

	bool Initialize(void);
	void Update(int iDeltaTime);
	void Finialize(void);

	void Clear();
	void OnUnitDead(BeUnit* pkUnit);

	void			ClearSelectUnits(bool bClear = true) {};
	void			AddSelectUnit(int iID) {};
	void			UpdateSelectUnits(void) {};

	BeUnit* AddUnit(int iTypeID, int iSkillLevel = -1, int iPlayer = -1,int iUnitID = -1, int iESLabel = 0, int iSkinIdx = -1);
	void		DelUnit(int iID);
	void		DelUnitIterator(int iID);

	BeUnit* GetUnitByID(int iID, bool bSuspend = false, bool bSoul = false);
	int			GetUnitIDFromSuspend(int ITypeID);

	void		GetAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, const BeUnit* pkSrcUnit, int iDynamicProperty, int iStaticProperty) const;

	void		GetAreaGroupByAttackedType(UnitGroup& kGroup, float fX, float fY, float fRadius, const BeUnit* pkSrcUnit, int iAttackedType) const;

	void		GetAreaGroupID(UnitGroupID& rkGroupID, float fX, float fY, float fRadius, const BeUnit* pkSrcUnit, int iDynamicProperty, int iStaticProperty) const;

	void  GetAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, int iPlayerIdx, int iFlag, bool bDead) const;

	void		GetAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, const BeUnit* pkSrcUnit, BeCommandType eCommand, int iData = 0) const;
	void		GetRectangleAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, float fDis, float fFace, const BeUnit* pkSrcUnit, BeCommandType eCommand, int iData) const;

	void		GetAreaGroupID(UnitGroupID& rkGroupID, float fX, float fY, float fRadius, const BeUnit* pkSrcUnit, BeCommandType eCommand, int iData = 0) const;

	BeUnit* GetGroupNearestUnit(const UnitGroup& kGroup, float fPosX, float fPosY, float fDistance = 0, float fRefX = 0, float fRefY = 0) const;
	int			GetGroupNearestUnit(const UnitGroupID& kGroupID, float fPosX, float fPosY) const;
	BeUnit* GetGroupFarestUnit(const UnitGroup& kGroup, float fPosX, float fPosY) const;
	void		GetBlockAreaGroup(UnitGroup& kGroup, float fX1, float fY1, float fX2, float fY2, int iPlayerIdx = -1, int iFlag = BSF_NORMAL);
	void		GetFanAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, float fB, float fE, int iPlayerIdx = -1, int iFlag = BSF_NORMAL);

	void		GetAllMapGroup(UnitGroup& kGroup, int iPlayerIdx = -1, int iFlag = BSF_NORMAL);

	void		GetAllMapGroup(UnitGroup& kGroup, const BeUnit* pkSrcUnit, BeCommandType eCommand, int iData = 0);

	const UnitGroup& GetAllHeroPtr();
	const UnitGroupID& GetAllHeroID();

	void		GetUnitGroupByID(UnitGroup& kGroup, int iID);

	void		GetUnitGroupByControl(UnitGroup& kGroup, int iIdx, bool bSuspend = false, bool bOtherPlayer = true, bool bSpecial = false);

	const std::unordered_map<int, BeUnit*>& GetID2Unit() const;

	const std::unordered_map<int, BeUnit*>& GetID2SuspendUnit() const;
	int						GetUnitNums();

	bool		GetUnitByTypeID(UnitGroup& kGroup, int iTypeID) const;
	bool		GetUnitByPlayer(UnitGroup& kGroup, const std::string& kPlayerName) const;
	void		GetUnitByCommand(UnitGroup& kGroup, const BeUnit* pkSrcUnit, BeCommandType eCommand, int iData) const;
public:
	BeUnit* GetUnitByTypeID(int iTypeID)
	{
		for (std::unordered_map<int, BeUnit*>::const_iterator itr = m_kID2Unit.begin(); itr != m_kID2Unit.end(); ++itr)
		{
			BeUnit* pkUnit = itr->second;
			if (pkUnit && !pkUnit->IsDead())
			{
				if (iTypeID == pkUnit->GetTypeID())
				{
					return pkUnit;
				}
			}
		}
		return NULL;
	}

protected:
	virtual void	SafeDeleteUnit(BeUnit*& pkUnit);
	virtual BeUnit* NewUnit(int iID);

	virtual void	OnDelUnit(BeUnit* pkUnit);

	bool		IsPassUnit(BeUnit* pkUnit, int iPlayerGroup, int iFlag, bool bDead = false) const;

protected:
	std::unordered_map<int, BeUnit*>	m_kID2Unit;
	std::unordered_map<int, BeUnit*>	m_kID2SuspendUnit;
	UnitGroup				m_kVHeroPtr;
	UnitGroupID				m_kVAllHeroID;

public:
	void				ClrPureData();
	void				PushNeedUpdateUnitID(int iID);
	const UnitGroupID& PureGetAllNeedUpdateUnitID();
protected:
	UnitGroupID				m_kVPureNeedUpdateID;
public:
	void					PushNeedDelNormalUnit(int iCamp, int iID);
	void					PushNeedDelHeroBossBuilding(int iID);
	const UnitGroupID& PureGetDelUnitID(int iCamp);
	const UnitGroupID& PureGetDelBoosBuildingID();
	const UnitGroupID& PureGetNewBossID();
	void					PushNewBossID(int iID);
protected:
	UnitGroupID				m_kPureDelID;
	UnitGroupID				m_kPureDelHeroBossBuildID;
	UnitGroupID				m_kPureNewBossID;

	std::vector<int>		m_aiVecRandomHeroID;
	std::unordered_map<int, UnitGroupID>		m_kLoseVisionUnitGroup;
};
