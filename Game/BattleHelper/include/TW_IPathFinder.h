#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_MapDefine.h"

struct TePos2;

class IPathFinder
{
public:
	IPathFinder();
	virtual ~IPathFinder();

	virtual	const TePos2* GetPathPoint() const = 0;
	virtual	int				GetPathPointNum() const = 0;

	virtual	void			InitGrids(int iW, int iH, unsigned short* akGrids, bool bUseTStar = false, int iTStarObs = TGF_TERRAIN | TGF_DOODAD) = 0;

	virtual	void			ClrObstacle(float fX, float fY, int iObstacle = TGF_UNIT, int iSize = 2) = 0;
	virtual	void			SetObstacle(float fX, float fY, int iObstacle = TGF_UNIT, int iSize = 2) = 0;

	virtual	TeFindResult	FindPathUnit(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize = 0, int iDistance = 0, int iObs = TGF_FIXED_OTS | TGF_UNIT | TGF_SKILL) = 0;
	virtual	int				GetFirstCanStay(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, float& fX, float& fY, float fDistance, int iObs = TGF_FIXED_OTS | TGF_UNIT | TGF_SKILL) const = 0;
	virtual bool			GetNearestCanStay(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, float& fX, float& fY, float fDistance, int iObs = TGF_FIXED_OTS | TGF_UNIT | TGF_SKILL) const = 0;

	virtual bool			IsObstacle(float fX, float fY, int iObstacle = TGF_UNIT, int iSize = 2) = 0;
	virtual bool			IsPointDirect(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize, float fDistance, int iObs) = 0;

	virtual bool			UnitCanReach(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize = 0, int iDistance = 0, int iObs = TGF_FIXED_OTS | TGF_UNIT | TGF_SKILL) = 0;

	virtual void			CopyGridsFromServerGrids() = 0;

	static void InitServerPathGrids(int iW, int iH, unsigned short* akGrids);
	static void ReleaseServerPathGrids();
	static IPathFinder* CreatePathFinder();
	static void ReleasePathFinder(IPathFinder* pkPathFinder);
};
