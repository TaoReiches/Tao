#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <list>
#include "TW_LogicBase.h"
#include "TW_MapDefine.h"
#include "TW_Pos2.h"

class IPathFinder;
class TeUnit;
struct TeMapInfo;
class TeDoodad;
class TeEffect;

class TeMap : public BeMainPtr
{
public:
	TeMap(void);
	~TeMap(void);

	inline	int						GetWidth()						const;
	inline	int						GetHeight()						const;
	inline	const	TeMapInfo* GetMapInfo(void)				const;
	inline	int						GetVisionIndex(float fPos);

	bool	Initialize();
	void	Update(int iDeltaTime);
	void	Finialize(void);

	void			AddUnitObstacle(TeUnit* pkUnit, bool bAddFixed = true, bool bForce = true);
	void			DelUnitObstacle(TeUnit* pkUnit, bool bDelFixed = true, bool bForce = true);

	void			AddObstacle(float fX, float fY, int iObs, int iSize);
	void			DelObstacle(float fX, float fY, int iObs, int iSize);

	bool			IsInSight(const TeUnit* pkSrcUnit, const TeUnit* pkDstUnit);
	bool			IsInSight(int iPlayGroup, const TeUnit* pkDstUnit);
	bool			IsInSight(int iPlayGroup, float fPosX, float fPosY);
	void* GetGroupVision(int eGroup);
	bool            IsInSight(int iPlayGroup, TeEffect* pkDstEffect);

	void			AddVisionOT(float fX, float fY);
	void			DelVisionOT(float fX, float fY);
	void			ReleaseVision(void);

	void			ReleaseUnitVision(TeUnit* pkUnit);
	void			UpdateUnitVision(TeUnit* pkUnit);

	TeFindResult	FindPath(std::list<TePos2>& akPath, TeUnit* pkUnit, float fTargetX, float fTargetY, float fDistance, int iObs);
	int				GetFirstCanStay(TeUnit* pkUnit, float fTargetX, float fTargetY, float& fWalkX, float& fWalkY, float fDistance, int iObs);
	int				GetFirstCanStay(float fSrcX, float fSrcY, int iSrcSize, float fTargetX, float fTargetY, float& fWalkX, float& fWalkY, float fDistance, int iObs);

	bool			GetNearestCanStay(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, float& fX, float& fY, float fDistance, int iObs) const;

	bool			GetUnitCanReach(TeUnit* pkUnit, float fTargetX, float fTargetY, float fDistance, int iObs);

	bool			SetUnitPosition(TeUnit* pkUnit, float fTargetX, float fTargetY, float fTargetZ, float fDistance, bool bCanStayOnObstacle, int iObs, int iReachableObs, bool bNeedRecordChange);
	void			EnableAllMap(void);

	bool			IsObstacle(float fX, float fY, int iObstacle, int iSize);
	bool			IsPointDirect(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize, float fDistance, int iObs);

	int				GetGrassIndex(float fX, float fY);
	bool			IsInSight(int iX, int iY, int eGroup);

private:
	TeMapInfo* m_pkMapInfo;
	IPathFinder* m_pkPathFinder;

	unsigned	short* m_pkAVision;
	unsigned	short* m_pkTVision;
	unsigned	short* m_pkCVision;
	unsigned	int					m_iVisionWidth;
	unsigned	int					m_iVisionHeight;
	unsigned	int					m_iGrassWidth;
	unsigned	int					m_iGrassHeight;
};

inline	int TeMap::GetWidth() const
{
	return  m_pkMapInfo->iWidth;
}
inline	int TeMap::GetHeight() const
{
	return m_pkMapInfo->iHeight;
}
inline	const TeMapInfo* TeMap::GetMapInfo(void) const
{
	return  m_pkMapInfo;
}
