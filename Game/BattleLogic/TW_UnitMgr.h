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
#include "TW_Command.h"
#include "TW_UnitMgrDefine.h"

class TwUnit;

typedef std::vector<std::shared_ptr<TwUnit>> UnitGroup;
typedef std::vector<int> UnitGroupID;

class TwUnitMgr final : public TwEntityMgr
{
public:
    TwUnitMgr(void);
    ~TwUnitMgr(void);

    bool Initialize(void);
    void Update(int iDeltaTime);
    void Finialize(void);

    void Clear();
    void OnUnitDead(std::shared_ptr<TwUnit> pkUnit);

    std::shared_ptr<TwUnit> AddUnit(int iTypeID, int iSkillLevel = -1, std::uint64_t iPlayer = -1,int iUnitID = -1);
    void DelUnit(int iID);

    std::shared_ptr<TwUnit> GetUnitByID(int iID, bool bSuspend = false, bool bSoul = false);
    int			GetUnitIDFromSuspend(int ITypeID);

    void		GetAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, const std::shared_ptr<TwUnit> pkSrcUnit, int iDynamicProperty, int iStaticProperty) const;

    void		GetAreaGroupByAttackedType(UnitGroup& kGroup, float fX, float fY, float fRadius, const std::shared_ptr<TwUnit> pkSrcUnit, int iAttackedType) const;

	void		GetAreaGroupID(UnitGroupID& rkGroupID, float fX, float fY, float fRadius, const std::shared_ptr<TwUnit> pkSrcUnit, int iDynamicProperty, int iStaticProperty) const;

	void  GetAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, int iPlayerIdx, int iFlag, bool bDead) const;

	void		GetAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, const std::shared_ptr<TwUnit> pkSrcUnit, TwCommandType eCommand, int iData = 0) const;
	void		GetRectangleAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, float fDis, float fFace, const std::shared_ptr<TwUnit> pkSrcUnit, TwCommandType eCommand, int iData) const;

	void		GetAreaGroupID(UnitGroupID& rkGroupID, float fX, float fY, float fRadius, const std::shared_ptr<TwUnit> pkSrcUnit, TwCommandType eCommand, int iData = 0) const;

	std::shared_ptr<TwUnit> GetGroupNearestUnit(const UnitGroup& kGroup, float fPosX, float fPosY, float fDistance = 0, float fRefX = 0, float fRefY = 0) const;
	int			GetGroupNearestUnit(const UnitGroupID& kGroupID, float fPosX, float fPosY) const;
	std::shared_ptr<TwUnit> GetGroupFarestUnit(const UnitGroup& kGroup, float fPosX, float fPosY) const;
	void		GetBlockAreaGroup(UnitGroup& kGroup, float fX1, float fY1, float fX2, float fY2, int iPlayerIdx = -1, int iFlag = BSF_NORMAL);
	void		GetFanAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, float fB, float fE, int iPlayerIdx = -1, int iFlag = BSF_NORMAL);

	void		GetAllMapGroup(UnitGroup& kGroup, int iPlayerIdx = -1, int iFlag = BSF_NORMAL);

	void		GetAllMapGroup(UnitGroup& kGroup, const std::shared_ptr<TwUnit> pkSrcUnit, TwCommandType eCommand, int iData = 0);

	const UnitGroup& GetAllHeroPtr();
	const UnitGroupID& GetAllHeroID();

	void		GetUnitGroupByID(UnitGroup& kGroup, int iID);

	const std::unordered_map<int, std::shared_ptr<TwUnit>>& GetID2Unit() const;

	const std::unordered_map<int, std::shared_ptr<TwUnit>>& GetID2SuspendUnit() const;
	int						GetUnitNums();

	bool		GetUnitByTypeID(UnitGroup& kGroup, int iTypeID) const;
	bool		GetUnitByPlayer(UnitGroup& kGroup, const std::string& kPlayerName) const;
public:
	std::shared_ptr<TwUnit> GetUnitByTypeID(int iTypeID);

protected:
	virtual void	SafeDeleteUnit(std::shared_ptr<TwUnit>& pkUnit);
	virtual std::shared_ptr<TwUnit> NewUnit(int iID);

	virtual void	OnDelUnit(std::shared_ptr<TwUnit> pkUnit);

	bool		IsPassUnit(std::shared_ptr<TwUnit> pkUnit, int iPlayerGroup, int iFlag, bool bDead = false) const;

protected:
	std::unordered_map<int, std::shared_ptr<TwUnit>>	m_kID2Unit;
	std::unordered_map<int, std::shared_ptr<TwUnit>>	m_kID2SuspendUnit;
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
