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
class TwUnit;
class TeDoodad;
class TeEffect;

class TeMap : public TwMainPtr
{
public:
	TeMap(void);
	~TeMap(void);

	inline	int	GetWidth()	const
	{
		return 320000;
	}
	inline	int	GetHeight()	const
	{
		return 320000;
	}

	static const int GridSize = 32;

	bool	Initialize();
	void	Update(int iDeltaTime);
	void	Finialize(void);

	void			AddUnitObstacle(std::shared_ptr<TwUnit> pkUnit, bool bAddFixed = true, bool bForce = true);
	void			DelUnitObstacle(std::shared_ptr<TwUnit> pkUnit, bool bDelFixed = true, bool bForce = true);

	void			AddObstacle(float fX, float fY, TwGridFlag iObs, int iSize);
	void			DelObstacle(float fX, float fY, TwGridFlag iObs, int iSize);

	TwFindResult	FindPath(std::list<TwPos2>& akPath, std::shared_ptr<TwUnit> pkUnit, float fTargetX, float fTargetY, float fDistance, TwGridFlag iObs);
	int				GetFirstCanStay(std::shared_ptr<TwUnit> pkUnit, float fTargetX, float fTargetY, float& fWalkX, float& fWalkY, float fDistance, TwGridFlag iObs);
	int				GetFirstCanStay(float fSrcX, float fSrcY, int iSrcSize, float fTargetX, float fTargetY, float& fWalkX, float& fWalkY, float fDistance, TwGridFlag iObs);

	bool			GetNearestCanStay(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, float& fX, float& fY, float fDistance, TwGridFlag iObs) const;

	bool			GetUnitCanReach(std::shared_ptr<TwUnit> pkUnit, float fTargetX, float fTargetY, float fDistance, TwGridFlag iObs);

	bool			SetUnitPosition(std::shared_ptr<TwUnit> pkUnit, float fTargetX, float fTargetY, float fTargetZ, float fDistance, bool bCanStayOnObstacle, TwGridFlag iObs, TwGridFlag iReachableObs, bool bNeedRecordChange);

	bool			IsObstacle(float fX, float fY, TwGridFlag iObstacle, int iSize);
	bool			IsPointDirect(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize, float fDistance, TwGridFlag iObs);

private:
	std::unique_ptr<ITwPathFinder>	m_pkPathFinder;
};
