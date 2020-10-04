#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_MapDefine.h"

class TePos2;

class IPathFinder
{
public:
	IPathFinder();
	virtual ~IPathFinder();

	virtual	const TePos2* GetPathPoint() const = 0;
	virtual	int				GetPathPointNum() const = 0;

	virtual	void			InitGrids(int iW, int iH, unsigned short* akGrids, bool bUseBStar = false, int iBStarObs = BGF_TERRAIN | BGF_DOODAD) = 0;

	virtual	void			ClrObstacle(float fX, float fY, int iObstacle = BGF_UNIT, int iSize = 2) = 0;
	virtual	void			SetObstacle(float fX, float fY, int iObstacle = BGF_UNIT, int iSize = 2) = 0;

	virtual	BeFindResult	FindPathUnit(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize = 0, int iDistance = 0, int iObs = BGF_FIXED_OBS | BGF_UNIT | BGF_SKILL) = 0;
	virtual	int				GetFirstCanStay(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, float& fX, float& fY, float fDistance, int iObs = BGF_FIXED_OBS | BGF_UNIT | BGF_SKILL) const = 0;
	virtual bool			GetNearestCanStay(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, float& fX, float& fY, float fDistance, int iObs = BGF_FIXED_OBS | BGF_UNIT | BGF_SKILL) const = 0;

	virtual bool			ExecuteCommand(const char* strCmd) = 0;
	virtual bool			IsObstacle(float fX, float fY, int iObstacle = BGF_UNIT, int iSize = 2) = 0;
	virtual bool			IsPointDirect(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize, float fDistance, int iObs) = 0;

	virtual bool			UnitCanReach(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize = 0, int iDistance = 0, int iObs = BGF_FIXED_OBS | BGF_UNIT | BGF_SKILL) = 0;

	virtual void			CopyGridsFromServerGrids() = 0;

	static void InitServerPathGrids(int iW, int iH, unsigned short* akGrids);
	static void ReleaseServerPathGrids();
	static IPathFinder* CreatePathFinder();
	static void ReleasePathFinder(IPathFinder* pkPathFinder);
};
