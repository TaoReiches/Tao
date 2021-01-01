/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Map.h"
#include "TW_IPathFinder.h"
#include "TW_Unit.h"
#include "Unit_table.hpp"
#include "TW_ShareUnitData.h"

TeMap::TeMap(void)
{
}

TeMap::~TeMap(void)
{
	TeMap::Finialize();
}

bool TeMap::Initialize()
{
	Finialize();
	//m_pkMapInfo = gMapInfo.RequestMapInfo(rkMapFile.c_str());
	//if (!m_pkMapInfo)
	//{
	//	return false;
	//}

	//int iWidth = GetWidth() / m_pkMapInfo->iGridSize;
	//int iHeight = GetHeight() / m_pkMapInfo->iGridSize;

	//m_iGrassWidth = GetWidth() / m_pkMapInfo->iGrassSize + 1;
	//m_iGrassHeight = GetHeight() / m_pkMapInfo->iGrassSize + 1;

	m_pkPathFinder.reset(CreatePathFinder());
	if (!m_pkPathFinder)
	{
		return false;
	}

	unsigned short iWidth = GetWidth() / GridSize;
	unsigned short iHeight = GetHeight() / GridSize;
	unsigned int total = iWidth * iHeight;
	unsigned short* grids = new unsigned short[total];
	std::memset(grids, 0, total);
	m_pkPathFinder->InitGrids(iWidth, iHeight, grids);
	delete[] grids;

	return true;
}

bool TeMap::IsObstacle(float fX, float fY, TwGridFlag iObstacle, int iSize)
{
	if (!m_pkPathFinder)
	{
		return false;
	}

	return m_pkPathFinder->IsObstacle(fX, fY, iObstacle, iSize);
}

bool TeMap::IsPointDirect(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize, float fDistance, TwGridFlag iObs)
{
	if (!m_pkPathFinder)
	{
		return false;
	}

	return m_pkPathFinder->IsPointDirect(fSrcX, fSrcY, iSrcSize, fDstX, fDstY, iDstSize, fDistance, iObs);
}

void TeMap::Finialize(void)
{
	if (m_pkPathFinder)
	{
		ReleasePathFinder(m_pkPathFinder.get());
		m_pkPathFinder.release();
	}
}

void TeMap::Update(int iDeltaTime)
{
}

void TeMap::AddUnitObstacle(std::shared_ptr<TwUnit> pkUnit, bool bAddFixed, bool bForce)
{
	if (!m_pkPathFinder)
	{
		return;
	}

	if (pkUnit->HasFlag(BUF_OBSTACLESET))
	{
		return;
	}
	if (pkUnit->HasFlag(BUF_NOOBSTACLE) || pkUnit->HasFlag(BUF_IGNOREUNITOBS))
	{
		return;
	}

	if (!pkUnit->GetNeedUpdateObstacle() && pkUnit->GetHaveSetObstacle() && !bForce)
	{
		return;
	}

	int iSize = pkUnit->GetOBSize();
	if (iSize <= 0)
	{
		return;
	}

	float fX = pkUnit->GetPosX();
	float fY = pkUnit->GetPosY();

	if (pkUnit->GetClass() == UNIT_CLASSTYPE_BUILDING)
	{
		if (bAddFixed)
		{
			pkUnit->SetFlag(BUF_OBSTACLESET);
			m_pkPathFinder->SetObstacle(fX, fY, TwGridFlag::TGF_DOODAD, iSize);
		}
	}
	else
	{
		if (pkUnit->HasFlag(BUF_MOVING))
		{
			pkUnit->SetFlag(BUF_OBSTACLESET);
			m_pkPathFinder->SetObstacle(fX, fY, TwGridFlag::TGF_TEMP, iSize);
		}
		else
		{
			pkUnit->SetFlag(BUF_OBSTACLESET);
			if (pkUnit->GetClass() == UNIT_CLASSTYPE_SOLIDER)
			{
				m_pkPathFinder->SetObstacle(fX, fY, TwGridFlag::TGF_SOLIDER | TwGridFlag::TGF_TEMP, iSize);
				m_pkPathFinder->SetObstacle(fX, fY, TwGridFlag::TGF_SOLIDER_COLLION, 4);
			}
			else
			{
				m_pkPathFinder->SetObstacle(fX, fY, TwGridFlag::TGF_UNIT_OTS, iSize);
			}
		}
	}

	if (!pkUnit->GetNeedUpdateObstacle())
	{
		pkUnit->SetHaveSetObstacle(true);
	}
}

void TeMap::DelUnitObstacle(std::shared_ptr<TwUnit> pkUnit, bool bDelFixed, bool bForce)
{
	if (!m_pkPathFinder)
	{
		return;
	}

	if (!pkUnit->HasFlag(BUF_OBSTACLESET))
	{
		return;
	}
	if (pkUnit->HasFlag(BUF_NOOBSTACLE) || pkUnit->HasFlag(BUF_IGNOREUNITOBS))
	{
		return;
	}

	if (!pkUnit->GetNeedUpdateObstacle() && pkUnit->GetHaveSetObstacle() && !bForce)
	{
		return;
	}

	int iSize = pkUnit->GetOBSize();
	if (iSize <= 0)
	{
		return;
	}

	float fX = pkUnit->GetPosX();
	float fY = pkUnit->GetPosY();

	if (pkUnit->GetClass() == UNIT_CLASSTYPE_BUILDING)
	{
		if (bDelFixed)
		{
			pkUnit->ClrFlag(BUF_OBSTACLESET);
			m_pkPathFinder->ClrObstacle(fX, fY, TwGridFlag::TGF_DOODAD, iSize);
		}
	}
	else
	{
		pkUnit->ClrFlag(BUF_OBSTACLESET);
		if (pkUnit->GetClass() == UNIT_CLASSTYPE_SOLIDER)
		{
			m_pkPathFinder->ClrObstacle(fX, fY, TwGridFlag::TGF_SOLIDER | TwGridFlag::TGF_TEMP, iSize);
			m_pkPathFinder->ClrObstacle(fX, fY, TwGridFlag::TGF_SOLIDER_COLLION, 4);
		}
		else
		{
			m_pkPathFinder->ClrObstacle(fX, fY, TwGridFlag::TGF_UNIT_OTS, iSize);
		}
	}
}

void TeMap::AddObstacle(float fX, float fY, TwGridFlag iObs, int iSize)
{
	if (!m_pkPathFinder)
	{
		return;
	}

	m_pkPathFinder->SetObstacle(fX, fY, iObs, iSize);
}

void TeMap::DelObstacle(float fX, float fY, TwGridFlag iObs, int iSize)
{
	if (!m_pkPathFinder)
	{
		return;
	}

	m_pkPathFinder->ClrObstacle(fX, fY, iObs, iSize);
}

TwFindResult TeMap::FindPath(std::list<TwPos2>& akPath, std::shared_ptr<TwUnit> pkUnit, float fTargetX, float fTargetY, float fDistance, TwGridFlag iObs)
{
	if (!m_pkPathFinder)
	{
		return TwFindResult::TFR_NONE;
	}

	akPath.clear();

	if (pkUnit->HasFlag(BUF_IGNOREUNITOBS))
	{
		iObs &= ~TwGridFlag::TGF_UNIT_OTS;
		iObs &= ~TwGridFlag::TGF_SOLIDER;
	}

	if (pkUnit->HasFlag(BUF_IGNOREFIXEDOBS))
	{
		iObs &= ~TwGridFlag::TGF_FIXED_OTS;
	}

	if (!(bool)iObs)
	{
		float fSrcX = pkUnit->GetPosX();
		float fSrcY = pkUnit->GetPosY();

		float fDistance2 = GetDistance2(fSrcX, fSrcY, fTargetX, fTargetY);
		if (fDistance2 <= fDistance * fDistance)
		{
			return TwFindResult::TFR_ARRIVED;
		}
		else
		{
			fDistance -= 4.0f;
			if (fDistance <= 0.0f)
			{
				TwPos2 kPos;
				kPos.fX = fTargetX;
				kPos.fY = fTargetY;
				akPath.push_back(kPos);
			}
			else
			{
				float fL = GetDistance(fSrcX, fSrcY, fTargetX, fTargetY);
				float fMX = (fL - fDistance) * fTargetX / fL + fDistance * fSrcX / fL;
				float fMY = (fL - fDistance) * fTargetY / fL + fDistance * fSrcY / fL;
				TwPos2 kPos;
				kPos.fX = fMX;
				kPos.fY = fMY;
				akPath.push_back(kPos);
			}
			return TwFindResult::TFR_DIRECT;
		}
		return TwFindResult::TFR_DIRECT;
	}

	float fSrcX = pkUnit->GetPosX();
	float fSrcY = pkUnit->GetPosY();
	float fDstX = fTargetX;
	float fDstY = fTargetY;
	int iSize = pkUnit->GetOBSize();

	if (iSize <= 0)
	{
		iSize = 2;
	}

	TwFindResult eRet = m_pkPathFinder->FindPathUnit(fSrcX, fSrcY, iSize, fDstX, fDstY, 0, (int)fDistance, iObs);
	int iPoints = m_pkPathFinder->GetPathPointNum();
	if (iPoints > 0)
	{
		const TwPos2* pkPos = m_pkPathFinder->GetPathPoint();
		for (int i = 0; i < iPoints; i++)
		{
			akPath.push_back(TwPos2(pkPos[i].fX, pkPos[i].fY));
		}
		TwPos2& kPos = akPath.back();
		if (pkPos[iPoints - 1].fX == fDstX && pkPos[iPoints - 1].fY == fDstY)
		{
			kPos.fX = fTargetX;
			kPos.fY = fTargetY;
		}
	}

	return eRet;
}

int TeMap::GetFirstCanStay(std::shared_ptr<TwUnit> pkUnit, float fTargetX, float fTargetY, float& fWalkX, float& fWalkY, float fDistance, TwGridFlag iObs)
{
	if (pkUnit->HasFlag(BUF_IGNOREUNITOBS))
	{
		iObs &= ~TwGridFlag::TGF_UNIT_OTS;
	}
	if (pkUnit->HasFlag(BUF_IGNOREFIXEDOBS))
	{
		iObs &= ~TwGridFlag::TGF_FIXED_OTS;
	}

	if (!(bool)iObs)
	{
		fWalkX = fTargetX;
		fWalkY = fTargetY;
		return true;
	}

	float fSrcX = pkUnit->GetPosX();
	float fSrcY = pkUnit->GetPosY();
	float fDstX = fTargetX;
	float fDstY = fTargetY;
	int iSrcSize = pkUnit->GetOBSize();
	if (iObs == TwGridFlag::TGF_SOLIDER_COLLION)
	{
		iSrcSize = 4;
	}
	int bRet = GetFirstCanStay(fSrcX, fSrcY, iSrcSize, fTargetX, fTargetY, fWalkX, fWalkY, fDistance, iObs);
	return bRet;
}

int TeMap::GetFirstCanStay(float fSrcX, float fSrcY, int iSrcSize, float fTargetX, float fTargetY, float& fWalkX, float& fWalkY, float fDistance, TwGridFlag iObs)
{
	if (!m_pkPathFinder)
	{
		return false;
	}

	float fSX = fSrcX;
	float fSY = fSrcY;
	float fDstX = fTargetX;
	float fDstY = fTargetY;
	bool bRet = m_pkPathFinder->GetFirstCanStay(fSX, fSY, iSrcSize, fDstX, fDstY, fWalkX, fWalkY, fDistance, iObs);
	if (bRet)
	{
		if (fWalkX == fDstX && fWalkY == fDstY)
		{
			fWalkX = fTargetX;
			fWalkY = fTargetY;
		}
	}
	return bRet;
}

bool TeMap::GetNearestCanStay(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, float& fX, float& fY, float fDistance, TwGridFlag iObs) const
{
	if (m_pkPathFinder)
	{
		bool bRet = m_pkPathFinder->GetNearestCanStay(fSrcX, fSrcY, iSrcSize, fDstX, fDstY, fX, fY, fDistance, iObs);
		return bRet;
	}

	return false;
}

bool TeMap::SetUnitPosition(std::shared_ptr<TwUnit> pkUnit, float fTargetX, float fTargetY, float fTargetZ, float fDistance, bool bCanStayOnObstacle, TwGridFlag iObs, TwGridFlag iReachableObs, bool bNeedRecordChange)
{
	if (!pkUnit)
	{
		return false;
	}

	if (!m_pkPathFinder)
	{
		return false;
	}

	bool bRet = true;
	if (pkUnit->HasFlag(BUF_IGNOREUNITOBS))
	{
		iObs &= ~TwGridFlag::TGF_UNIT_OTS;
	}
	if (pkUnit->HasFlag(BUF_IGNOREFIXEDOBS))
	{
		iObs &= ~TwGridFlag::TGF_FIXED_OTS;
	}
	if (!(bool)iObs)
	{
		bCanStayOnObstacle = true;
	}

	pkUnit->ClrFlag(BUF_MOVING);

	DelUnitObstacle(pkUnit);
	if (bCanStayOnObstacle)
	{
		pkUnit->SetPosition(fTargetX, fTargetY, fTargetZ, !bNeedRecordChange);
	}
	else
	{
		float fSrcX = pkUnit->GetPosX();
		float fSrcY = pkUnit->GetPosY();
		float fX = fSrcX;
		float fY = fSrcY;
		int iSrcSize = pkUnit->GetOBSize();
		if (iSrcSize == 0)
		{
			iSrcSize = 1;
		}
		bRet = GetNearestCanStay(fSrcX, fSrcY, iSrcSize, fTargetX, fTargetY, fX, fY, fDistance, iObs);
		bool bFind = true;
		if ((bool)iReachableObs)
		{
			bFind = false;
			int iCount = 2;
			std::vector<TwPos2> kPos;
			do
			{
				std::list<TwPos2> kPath;
				TwFindResult eRet = FindPath(kPath, pkUnit, fX, fY, 0.0f, iReachableObs);
				if (TwFindResult::TFR_NONE == eRet || TwFindResult::TFR_NOT_ARRIVE == eRet)
				{
					m_pkPathFinder->SetObstacle(fX, fY, iReachableObs, iSrcSize);
					kPos.push_back(TwPos2(fX, fY));
					TwPos2 kLastPos = m_pkPathFinder->GetPathPoint()[127];
					fX = kLastPos.fX;
					fY = kLastPos.fY;
					GetNearestCanStay(fSrcX, fSrcY, iSrcSize, fX, fY, fX, fY, fDistance, iObs);
				}
				else
				{
					bFind = true;
					break;
				}
			} while (--iCount > 0);

			for (int i = 0; i < (int)kPos.size(); ++i)
			{
				m_pkPathFinder->ClrObstacle(kPos[i].fX, kPos[i].fY, iReachableObs, iSrcSize);
			}
		}
		if (!bFind)
		{
			bRet = GetNearestCanStay(fSrcX, fSrcY, iSrcSize, fTargetX, fTargetY, fX, fY, fDistance, iObs);
		}
		pkUnit->SetPosition(fX, fY, fTargetZ, !bNeedRecordChange);
	}
	AddUnitObstacle(pkUnit);

	return bRet;
}

bool TeMap::GetUnitCanReach(std::shared_ptr<TwUnit> pkUnit, float fTargetX, float fTargetY, float fDistance, TwGridFlag iObs)
{
	if (!m_pkPathFinder)
	{
		return false;
	}
	{
		if (pkUnit->HasFlag(BUF_IGNOREUNITOBS))
		{
			iObs &= ~TwGridFlag::TGF_UNIT_OTS;
		}

		if (pkUnit->HasFlag(BUF_IGNOREFIXEDOBS))
		{
			iObs &= ~TwGridFlag::TGF_FIXED_OTS;
		}
		if (pkUnit->HasFlag(BUF_IGNOREDOODADOBS))
		{
			iObs &= ~TwGridFlag::TGF_DOODAD;
			iObs &= ~TwGridFlag::TGF_SKILL;
			iObs &= ~TwGridFlag::TGF_GUTDOODAD;
		}
		if (pkUnit->HasFlag(BUF_IGNORETERRAINOBS))
		{
			iObs &= ~TwGridFlag::TGF_TERRAIN;
		}
		if (!(bool)iObs)
		{
			return true;
		}
	}

	if (!pkUnit->UnitCanMove(true))
	{
		float fDist2 = GetDistance2(pkUnit->GetPosX(), pkUnit->GetPosY(), fTargetX, fTargetY);
		if (fDist2 <= fDistance * fDistance)
		{
			return true;
		}
		return false;
	}

	float fSrcX = pkUnit->GetPosX();
	float fSrcY = pkUnit->GetPosY();
	float fDstX = fTargetX;
	float fDstY = fTargetY;
	int iSize = pkUnit->GetOBSize();

	if (iSize <= 0)
	{
		iSize = 2;
	}
	bool bNeedChangeObs = false;
	if (pkUnit->HasFlag(BUF_OBSTACLESET))
	{
		bNeedChangeObs = true;
	}

	if (bNeedChangeObs)
	{
		DelUnitObstacle(pkUnit, false);
	}
	bool bRet = m_pkPathFinder->UnitCanReach(fSrcX, fSrcY, iSize, fDstX, fDstY, 0, (int)fDistance, iObs);
	if (bNeedChangeObs)
	{
		AddUnitObstacle(pkUnit, false);
	}
	return bRet;
}
