/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Map.h"
#include "TW_IPathFinder.h"

TeMap::TeMap(void)
{
	m_pkMapInfo = nullptr;
	m_pkPathFinder = nullptr;
}

TeMap::~TeMap(void)
{
	TeMap::Finialize();
}

bool TeMap::Initialize()
{
	Finialize();
	m_pkMapInfo = gMapInfo.RequestMapInfo(rkMapFile.c_str());
	if (!m_pkMapInfo)
	{
		return false;
	}

	int iWidth = GetWidth() / m_pkMapInfo->iGridSize;
	int iHeight = GetHeight() / m_pkMapInfo->iGridSize;

	m_iGrassWidth = GetWidth() / m_pkMapInfo->iGrassSize + 1;
	m_iGrassHeight = GetHeight() / m_pkMapInfo->iGrassSize + 1;

	m_pkPathFinder = CreatePathFinder();
	if (!m_pkPathFinder)
	{
		return false;
	}

	m_pkPathFinder->InitGrids(iWidth, iHeight, m_pkMapInfo->abyGrid);

	return true;
}

bool TeMap::IsObstacle(float fX, float fY, int iObstacle, int iSize)
{
	if (!m_pkPathFinder)
	{
		return false;
	}

	return m_pkPathFinder->IsObstacle(fX, fY, iObstacle, iSize);
}

bool TeMap::IsPointDirect(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize, float fDistance, int iObs)
{
	if (!m_pkPathFinder)
	{
		return false;
	}

	return m_pkPathFinder->IsPointDirect(fSrcX, fSrcY, iSrcSize, fDstX, fDstY, iDstSize, fDistance, iObs);
}

void TeMap::Finialize(void)
{
	m_pkAVision = nullptr;
	m_pkTVision = nullptr;
	m_pkCVision = nullptr;

	m_iVisionWidth = 0;
	m_iVisionHeight = 0;

	ReleaseVision();
	if (m_pkPathFinder)
	{
		ReleasePathFinder(m_pkPathFinder);
		m_pkPathFinder = nullptr;
	}
}

void TeMap::Update(int iDeltaTime)
{
}

void TeMap::AddUnitObstacle(TeUnit* pkUnit, bool bAddFixed, bool bForce)
{
	if (!m_pkPathFinder)
	{
		return;
	}

	if (pkUnit->HasFlag(TUF_OTSTACLESET))
	{
		return;
	}
	if (pkUnit->HasFlag(TUF_NOOTSTACLE) || pkUnit->HasFlag(TUF_IGNOREUNITOTS))
	{
		return;
	}

	if (!pkUnit->GetNeedUpdateObstacle() && pkUnit->GetHaveSetObstacle() && !bForce)
	{
		return;
	}

	int iSize = pkUnit->GetOTSize();
	if (iSize <= 0)
	{
		return;
	}

	float fX = pkUnit->GetPosX();
	float fY = pkUnit->GetPosY();

	if (pkUnit->GetClass() == UNIT_CLASSTYPE_TUILDING)
	{
		if (bAddFixed)
		{
			pkUnit->SetFlag(TUF_OTSTACLESET);
			m_pkPathFinder->SetObstacle(fX, fY, TGF_DOODAD, iSize);
		}
	}
	else
	{
		if (pkUnit->HasFlag(TUF_MOVING))
		{
			pkUnit->SetFlag(TUF_OTSTACLESET);
			m_pkPathFinder->SetObstacle(fX, fY, TGF_TEMP, iSize);
		}
		else
		{
			pkUnit->SetFlag(TUF_OTSTACLESET);
			if (pkUnit->GetClass() == UNIT_CLASSTYPE_SOLIDER)
			{
				m_pkPathFinder->SetObstacle(fX, fY, TGF_SOLIDER | TGF_TEMP, iSize);
				m_pkPathFinder->SetObstacle(fX, fY, TGF_SOLIDER_COLLION, 4);
			}
			else
			{
				m_pkPathFinder->SetObstacle(fX, fY, TGF_UNIT_OTS, iSize);
			}
		}
	}

	if (!pkUnit->GetNeedUpdateObstacle())
	{
		pkUnit->SetHaveSetObstacle(true);
	}
}

void TeMap::DelUnitObstacle(TeUnit* pkUnit, bool bDelFixed, bool bForce)
{
	if (!m_pkPathFinder)
	{
		return;
	}

	if (!pkUnit->HasFlag(TUF_OTSTACLESET))
	{
		return;
	}
	if (pkUnit->HasFlag(TUF_NOOTSTACLE) || pkUnit->HasFlag(TUF_IGNOREUNITOTS))
	{
		return;
	}

	if (!pkUnit->GetNeedUpdateObstacle() && pkUnit->GetHaveSetObstacle() && !bForce)
	{
		return;
	}

	int iSize = pkUnit->GetOTSize();
	if (iSize <= 0)
	{
		return;
	}

	float fX = pkUnit->GetPosX();
	float fY = pkUnit->GetPosY();

	if (pkUnit->GetClass() == UNIT_CLASSTYPE_TUILDING)
	{
		if (bDelFixed)
		{
			pkUnit->ClrFlag(TUF_OTSTACLESET);
			m_pkPathFinder->ClrObstacle(fX, fY, TGF_DOODAD, iSize);
		}
	}
	else
	{
		pkUnit->ClrFlag(TUF_OTSTACLESET);
		if (pkUnit->GetClass() == UNIT_CLASSTYPE_SOLIDER)
		{
			m_pkPathFinder->ClrObstacle(fX, fY, TGF_SOLIDER | TGF_TEMP, iSize);
			m_pkPathFinder->ClrObstacle(fX, fY, TGF_SOLIDER_COLLION, 4);
		}
		else
		{
			m_pkPathFinder->ClrObstacle(fX, fY, TGF_UNIT_OTS, iSize);
		}
	}
}

void TeMap::AddObstacle(float fX, float fY, int iObs, int iSize)
{
	if (!m_pkPathFinder)
	{
		return;
	}

	m_pkPathFinder->SetObstacle(fX, fY, iObs, iSize);
}

void TeMap::DelObstacle(float fX, float fY, int iObs, int iSize)
{
	if (!m_pkPathFinder)
	{
		return;
	}

	m_pkPathFinder->ClrObstacle(fX, fY, iObs, iSize);
}

TeFindResult TeMap::FindPath(std::list<TePos2>& akPath, TeUnit* pkUnit, float fTargetX, float fTargetY, float fDistance, int iObs)
{
	if (!m_pkPathFinder)
	{
		return TFR_NONE;
	}

	akPath.clear();

	if (pkUnit->HasFlag(TUF_IGNOREUNITOTS))
	{
		iObs &= ~TGF_UNIT_OTS;
		iObs &= ~TGF_SOLIDER;
	}

	if (pkUnit->HasFlag(TUF_IGNOREFIXEDOTS))
	{
		iObs &= ~TGF_FIXED_OTS;
	}

	if (!iObs)
	{
		float fSrcX = pkUnit->GetPosX();
		float fSrcY = pkUnit->GetPosY();

		float fDistance2 = GetDistance2(fSrcX, fSrcY, fTargetX, fTargetY);
		if (fDistance2 <= fDistance * fDistance)
		{
			return TFR_ARRIVED;
		}
		else
		{
			fDistance -= 4.0f;
			if (fDistance <= 0.0f)
			{
				TePos2 kPos;
				kPos.fX = fTargetX;
				kPos.fY = fTargetY;
				akPath.push_back(kPos);
			}
			else
			{
				float fL = GetDistance(fSrcX, fSrcY, fTargetX, fTargetY);
				float fMX = (fL - fDistance) * fTargetX / fL + fDistance * fSrcX / fL;
				float fMY = (fL - fDistance) * fTargetY / fL + fDistance * fSrcY / fL;
				TePos2 kPos;
				kPos.fX = fMX;
				kPos.fY = fMY;
				akPath.push_back(kPos);
			}
			return TFR_DIRECT;
		}
		return TFR_DIRECT;
	}

	float fSrcX = pkUnit->GetPosX();
	float fSrcY = pkUnit->GetPosY();
	float fDstX = fTargetX;
	float fDstY = fTargetY;
	int iSize = pkUnit->GetOTSize();

	if (iSize <= 0)
	{
		iSize = 2;
	}

	TeFindResult eRet = m_pkPathFinder->FindPathUnit(fSrcX, fSrcY, iSize, fDstX, fDstY, 0, (int)fDistance, iObs);
	int iPoints = m_pkPathFinder->GetPathPointNum();
	if (iPoints > 0)
	{
		const TePos2* pkPos = m_pkPathFinder->GetPathPoint();
		for (int i = 0; i < iPoints; i++)
		{
			akPath.push_back(TePos2(pkPos[i].fX, pkPos[i].fY));
		}
		TePos2& kPos = akPath.back();
		if (pkPos[iPoints - 1].fX == fDstX && pkPos[iPoints - 1].fY == fDstY)
		{
			kPos.fX = fTargetX;
			kPos.fY = fTargetY;
		}
	}

	return eRet;
}

int TeMap::GetFirstCanStay(TeUnit* pkUnit, float fTargetX, float fTargetY, float& fWalkX, float& fWalkY, float fDistance, int iObs)
{
	if (pkUnit->HasFlag(TUF_IGNOREUNITOTS))
	{
		iObs &= ~TGF_UNIT_OTS;
	}
	if (pkUnit->HasFlag(TUF_IGNOREFIXEDOTS))
	{
		iObs &= ~TGF_FIXED_OTS;
	}

	if (!iObs)
	{
		fWalkX = fTargetX;
		fWalkY = fTargetY;
		return true;
	}

	float fSrcX = pkUnit->GetPosX();
	float fSrcY = pkUnit->GetPosY();
	float fDstX = fTargetX;
	float fDstY = fTargetY;
	int iSrcSize = pkUnit->GetOTSize();
	if (iObs == TGF_SOLIDER_COLLION)
	{
		iSrcSize = 4;
	}
	int bRet = GetFirstCanStay(fSrcX, fSrcY, iSrcSize, fTargetX, fTargetY, fWalkX, fWalkY, fDistance, iObs);
	return bRet;
}

int TeMap::GetFirstCanStay(float fSrcX, float fSrcY, int iSrcSize, float fTargetX, float fTargetY, float& fWalkX, float& fWalkY, float fDistance, int iObs)
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

bool TeMap::GetNearestCanStay(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, float& fX, float& fY, float fDistance, int iObs) const
{
	if (m_pkPathFinder)
	{
		bool bRet = m_pkPathFinder->GetNearestCanStay(fSrcX, fSrcY, iSrcSize, fDstX, fDstY, fX, fY, fDistance, iObs);
		return bRet;
	}

	return false;
}

bool TeMap::SetUnitPosition(TeUnit* pkUnit, float fTargetX, float fTargetY, float fTargetZ, float fDistance, bool bCanStayOnObstacle, int iObs, int iReachableObs, bool bNeedRecordChange)
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
	if (pkUnit->HasFlag(TUF_IGNOREUNITOTS))
	{
		iObs &= ~TGF_UNIT_OTS;
	}
	if (pkUnit->HasFlag(TUF_IGNOREFIXEDOTS))
	{
		iObs &= ~TGF_FIXED_OTS;
	}
	if (!iObs)
	{
		bCanStayOnObstacle = true;
	}

	pkUnit->ClrFlag(TUF_MOVING);

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
		int iSrcSize = pkUnit->GetOTSize();
		if (iSrcSize == 0)
		{
			iSrcSize = 1;
		}
		bRet = GetNearestCanStay(fSrcX, fSrcY, iSrcSize, fTargetX, fTargetY, fX, fY, fDistance, iObs);
		bool bFind = true;
		if (iReachableObs)
		{
			bFind = false;
			int iCount = 2;
			std::vector<TePos2> kPos;
			do
			{
				std::list<TePos2> kPath;
				TeFindResult eRet = FindPath(kPath, pkUnit, fX, fY, 0.0f, iReachableObs);
				if (TFR_NONE == eRet || TFR_NOT_ARRIVE == eRet)
				{
					m_pkPathFinder->SetObstacle(fX, fY, iReachableObs, iSrcSize);
					kPos.push_back(TePos2(fX, fY));
					TePos2 kLastPos = m_pkPathFinder->GetPathPoint()[127];
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

void TeMap::EnableAllMap(void)
{
	m_kVision.EnableAllVision();
}

bool TeMap::GetUnitCanReach(TeUnit* pkUnit, float fTargetX, float fTargetY, float fDistance, int iObs)
{
	if (!m_pkPathFinder)
	{
		return false;
	}
	{
		if (pkUnit->HasFlag(TUF_IGNOREUNITOTS))
		{
			iObs &= ~TGF_UNIT_OTS;
		}

		if (pkUnit->HasFlag(TUF_IGNOREFIXEDOTS))
		{
			iObs &= ~TGF_FIXED_OTS;
		}
		if (pkUnit->HasFlag(TUF_IGNOREDOODADOTS))
		{
			iObs &= ~TGF_DOODAD;
			iObs &= ~TGF_SKILL;
			iObs &= ~TGF_GUTDOODAD;
		}
		if (pkUnit->HasFlag(TUF_IGNORETERRAINOTS))
		{
			iObs &= ~TGF_TERRAIN;
		}
		if (!iObs)
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
	int iSize = pkUnit->GetOTSize();

	if (iSize <= 0)
	{
		iSize = 2;
	}
	bool bNeedChangeObs = false;
	if (pkUnit->HasFlag(TUF_OTSTACLESET))
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

int		TeMap::GetGrassIndex(float fX, float fY)
{
	int iW = fX / m_pkMapInfo->iGrassSize;
	int iH = fY / m_pkMapInfo->iGrassSize;
	int iIndex = iH * m_iGrassWidth + iW;
	if (iW > m_iGrassWidth || iH > m_iGrassHeight)
	{
		return 0;
	}

	return m_pkMapInfo->abyGrass[iIndex];
}
