#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_MapDefine.h"

class TwPos2;

class ITwPathFinder
{
public:
	ITwPathFinder();
	virtual ~ITwPathFinder();

	virtual	const TwPos2*	GetPathPoint() const = 0;
	virtual	int				GetPathPointNum() const = 0;

	virtual	void			InitGrids(int iW, int iH, unsigned short* akGrids, bool bUseTStar = false, TwGridFlag iTStarObs = TwGridFlag::TGF_TERRAIN | TwGridFlag::TGF_DOODAD) = 0;

	virtual	void			ClrObstacle(float fX, float fY, TwGridFlag iObstacle = TwGridFlag::TGF_UNIT, int iSize = 2) = 0;
	virtual	void			SetObstacle(float fX, float fY, TwGridFlag iObstacle = TwGridFlag::TGF_UNIT, int iSize = 2) = 0;

	virtual	TwFindResult	FindPathUnit(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize = 0, int iDistance = 0, TwGridFlag iObs = TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_UNIT | TwGridFlag::TGF_SKILL) = 0;
	virtual	int				GetFirstCanStay(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, float& fX, float& fY, float fDistance, TwGridFlag iObs = TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_UNIT | TwGridFlag::TGF_SKILL) const = 0;
	virtual bool			GetNearestCanStay(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, float& fX, float& fY, float fDistance, TwGridFlag iObs = TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_UNIT | TwGridFlag::TGF_SKILL) const = 0;

	virtual bool			IsObstacle(float fX, float fY, TwGridFlag iObstacle = TwGridFlag::TGF_UNIT, int iSize = 2) = 0;
	virtual bool			IsPointDirect(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize, float fDistance, TwGridFlag iObs) = 0;

	virtual bool			UnitCanReach(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize = 0, int iDistance = 0, TwGridFlag iObs = TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_UNIT | TwGridFlag::TGF_SKILL) = 0;

	virtual void			CopyGridsFromServerGrids() = 0;
};

void InitServerPathGrids(int iW, int iH, unsigned short* akGrids);
void ReleaseServerPathGrids();
ITwPathFinder* CreatePathFinder();
void ReleasePathFinder(ITwPathFinder* pkPathFinder);
