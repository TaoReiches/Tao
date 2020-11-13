#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include <list>
#include "TW_LogicBase.h"
#include "TW_MapDefine.h"
#include "TW_Pos2.h"

class ITwPathFinder;
class BeUnit;
class TeDoodad;
class TeEffect;

class TeMap : public BeMainPtr
{
public:
	TeMap(void);
	~TeMap(void);

	inline	int	GetWidth()	const
	{
		return  100000;
	}
	inline	int	GetHeight()	const
	{
		return 100000;
	}

	static const int GridSize = 10;

	bool	Initialize();
	void	Update(int iDeltaTime);
	void	Finialize(void);

	void			AddUnitObstacle(BeUnit* pkUnit, bool bAddFixed = true, bool bForce = true);
	void			DelUnitObstacle(BeUnit* pkUnit, bool bDelFixed = true, bool bForce = true);

	void			AddObstacle(float fX, float fY, int iObs, int iSize);
	void			DelObstacle(float fX, float fY, int iObs, int iSize);

	TwFindResult	FindPath(std::list<TwPos2>& akPath, BeUnit* pkUnit, float fTargetX, float fTargetY, float fDistance, int iObs);
	int				GetFirstCanStay(BeUnit* pkUnit, float fTargetX, float fTargetY, float& fWalkX, float& fWalkY, float fDistance, int iObs);
	int				GetFirstCanStay(float fSrcX, float fSrcY, int iSrcSize, float fTargetX, float fTargetY, float& fWalkX, float& fWalkY, float fDistance, int iObs);

	bool			GetNearestCanStay(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, float& fX, float& fY, float fDistance, int iObs) const;

	bool			GetUnitCanReach(BeUnit* pkUnit, float fTargetX, float fTargetY, float fDistance, int iObs);

	bool			SetUnitPosition(BeUnit* pkUnit, float fTargetX, float fTargetY, float fTargetZ, float fDistance, bool bCanStayOnObstacle, int iObs, int iReachableObs, bool bNeedRecordChange);

	bool			IsObstacle(float fX, float fY, int iObstacle, int iSize);
	bool			IsPointDirect(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize, float fDistance, int iObs);

private:
	std::unique_ptr<ITwPathFinder>	m_pkPathFinder;
};
