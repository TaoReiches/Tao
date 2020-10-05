/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_PathFinder.h"
#include "TW_PathFinderFormular.h"

#pragma push_macro("new")
#undef new

IMPLEMENT_POOL(TePathMem, 1, 1);

#pragma pop_macro("new")

static TePathFinder gPthFinder;
IPathFinder* CreatePathFinder()
{
	return new TePathFinder;
}

void ReleasePathFinder(IPathFinder* pkPathFinder)
{
	if (pkPathFinder)
	{
		delete (pkPathFinder);
	}
}

void InitServerPathGrids(int iW, int iH, unsigned short* akGrids)
{
	gPthFinder.Release();
	if (akGrids)
	{
		gPthFinder.m_iWidth = iW;
		gPthFinder.m_iHeight = iH;
		gPthFinder.m_iMaxIdx = iW * iH;

		if (gPthFinder.m_bUseTStar)
		{
			gPthFinder.m_akTGrid = new TeTMapGrid[gPthFinder.m_iMaxIdx];
			memset(gPthFinder.m_akTGrid, 0, sizeof(TeTMapGrid) * gPthFinder.m_iMaxIdx);

			for (int h = 0; h < iH; h++)
			{
				for (int w = 0; w < iW; w++, akGrids++)
				{
					gPthFinder.m_akTGrid[h * iW + w].iObstacle = *akGrids;
					gPthFinder.m_akTGrid[h * iW + w].iObstacle &= TGF_TERRAIN | TGF_DOODAD;

					bool bIsObs = gPthFinder.m_akTGrid[h * iW + w].IsObs(gPthFinder.m_iTStarObs, true);
					gPthFinder.m_akTGrid[h * iW + w].iState = bIsObs ? TMPS_TLOCK : TMPS_NONE;
				}
			}
		}
		else
		{
			gPthFinder.m_akGrid = new TeMapGrid[gPthFinder.m_iMaxIdx];
			memset(gPthFinder.m_akGrid, 0, sizeof(TeMapGrid) * gPthFinder.m_iMaxIdx);

			for (int h = 0; h < iH; h++)
			{
				for (int w = 0; w < iW; w++, akGrids++)
				{
					gPthFinder.m_akGrid[h * iW + w].iObstacle = *akGrids;
					gPthFinder.m_akGrid[h * iW + w].iObstacle &= TGF_TERRAIN | TGF_DOODAD;
				}
			}
		}
	}
}

void ReleaseServerPathGrids()
{
	gPthFinder.Release();
}

IPathFinder::IPathFinder()
{
}

IPathFinder::~IPathFinder()
{
}

inline int GetDistance2Path(int iSrcX, int iSrcY, int iDstX, int iDstY)
{
	int iDX = iSrcX - iDstX;
	int iDY = iSrcY - iDstY;
	return (iDX * iDX + iDY * iDY);
}

inline int GetDistancePath(int iSrcX, int iSrcY, int iDstX, int iDstY)
{
	return (int)GetDistancePath((float)iSrcX, (float)iSrcY, (float)iDstX, (float)iDstY);
}

inline float GetDistance2Path(float fSrcX, float fSrcY, float fDstX, float fDstY)
{
	float fDX = fSrcX - fDstX;
	float fDY = fSrcY - fDstY;
	return (fDX * fDX + fDY * fDY);
}
inline float GetDistancePath(float fSrcX, float fSrcY, float fDstX, float fDstY)
{
	return static_cast<float>(sqrt(GetDistance2Path(fSrcX, fSrcY, fDstX, fDstY)));
}

TePathFinder::TePathFinder()
{
	m_iWidth = 0;
	m_iHeight = 0;
	m_iMaxIdx = 0;
	m_akGrid = NULL;
	m_iPathPointCount = 0;
	m_akTGrid = NULL;

	m_piOpen = NULL;
	m_iT = 0;
	m_iE = 0;

	m_piChange = NULL;
	m_iChangeCount = 0;

	m_piPath = NULL;
	m_iPathCount = 0;

	OpenListClear();

	m_iTlockType = 0;
	m_iTlockX = 0;
	m_iTlockY = 0;

	m_iChangeListLimit = PATH_CHANGE_LIST;


	m_iVisionWidth = 0;
	m_iVisionHeight = 0;
	m_akVision = NULL;

	m_bUseTStar = false;
	m_iTStarObs = TGF_TERRAIN | TGF_DOODAD;
}
TePathFinder::~TePathFinder()
{
	Release();
}

inline void TePathFinder::OpenListPushTack(int iIndex)
{
	if (m_iE < PATH_OPEN_LIST)
	{
		m_piOpen[m_iE++] = iIndex;
	}
}

inline int TePathFinder::OpenListPopHead(void)
{
	if (m_iT > m_iE)
	{
		m_iT = m_iE;
	}
	return m_piOpen[m_iT++];
}

inline int TePathFinder::OpenListGetCount(void)
{
	int iRet = m_iE - m_iT;
	if (iRet <= 0)
	{
		OpenListClear();
	}
	return iRet;
}
inline void TePathFinder::OpenListClear(void)
{
	m_iT = 0;
	m_iE = 0;
}

inline void TePathFinder::ChangeListPush(int iIndex)
{
	if (m_iChangeCount < PATH_CHANGE_LIST)
	{
		m_piChange[m_iChangeCount] = iIndex;
		m_iChangeCount++;
	}
}

inline bool TePathFinder::ChangeListFull(void)
{
	return m_iChangeCount > (m_iChangeListLimit - 32);
}

void TePathFinder::CreateList(void)
{
	ReleaseList();
	m_piChange = (int*)TePathMem::NEW();
	m_piOpen = m_piChange + PATH_CHANGE_LIST;
	m_piPath = m_piOpen + PATH_OPEN_LIST;
}

void TePathFinder::ReleaseList(void)
{
	m_iChangeCount = 0;
	if (m_piChange)
	{
		TePathMem::DEL((TePathMem*)m_piChange);
		m_piChange = NULL;
	}

	OpenListClear();
	m_piOpen = NULL;

	m_iPathCount = 0;
	m_piPath = NULL;
}

void TePathFinder::Release()
{
	ReleaseList();
	SAFE_DELETE(m_akGrid);
	SAFE_DELETE(m_akTGrid);
	m_iWidth = 0;
	m_iHeight = 0;
	m_iMaxIdx = 0;
	m_iPathPointCount = 0;

	m_iVisionWidth = 0;
	m_iVisionHeight = 0;
	m_akVision = NULL;

	for (int i = 0; i < 128; ++i)
	{
		m_piPathPoint[i].fX = 0.0f;
		m_piPathPoint[i].fY = 0.0f;
	}
}

const TePos2* TePathFinder::GetPathPoint() const
{
	return m_piPathPoint;
}

int	TePathFinder::GetPathPointNum() const
{
	return m_iPathPointCount;
}

void TePathFinder::InitVision(int iW, int iH, unsigned short* pVision)
{
	m_iVisionWidth = iW;
	m_iVisionHeight = iH;
	m_akVision = pVision;
}

void TePathFinder::CopyGridsFromServerGrids()
{
	Release();

	if (gPthFinder.m_bUseTStar)
	{
		if (gPthFinder.m_akTGrid)
		{
			m_iWidth = gPthFinder.m_iWidth;
			m_iHeight = gPthFinder.m_iHeight;
			m_iMaxIdx = gPthFinder.m_iMaxIdx;
			m_akTGrid = new TeTMapGrid[gPthFinder.m_iMaxIdx];
			memcpy(m_akTGrid, gPthFinder.m_akTGrid, sizeof(TeTMapGrid) * m_iMaxIdx);
		}
	}
	else
	{
		if (gPthFinder.m_akGrid)
		{
			m_iWidth = gPthFinder.m_iWidth;
			m_iHeight = gPthFinder.m_iHeight;
			m_iMaxIdx = gPthFinder.m_iMaxIdx;
			m_akGrid = new TeMapGrid[gPthFinder.m_iMaxIdx];
			memcpy(m_akGrid, gPthFinder.m_akGrid, sizeof(TeMapGrid) * m_iMaxIdx);
		}
	}
}

void TePathFinder::InitGrids(int iW, int iH, unsigned short* akGrids, bool bUseTStar, int iTStarObs)
{
	m_bUseTStar = bUseTStar;
	m_iTStarObs = iTStarObs;

	Release();
	if (akGrids)
	{
		m_iWidth = iW;
		m_iHeight = iH;
		m_iMaxIdx = iW * iH;
		if (m_bUseTStar)
		{
			m_akTGrid = new TeTMapGrid[m_iMaxIdx];
			memset(m_akTGrid, 0, sizeof(TeTMapGrid) * m_iMaxIdx);

			for (int h = 0; h < iH; h++)
			{
				for (int w = 0; w < iW; w++, akGrids++)
				{
					m_akTGrid[h * iW + w].x = w;
					m_akTGrid[h * iW + w].y = h;

					m_akTGrid[h * iW + w].iObstacle = *akGrids;
					m_akTGrid[h * iW + w].iObstacle &= TGF_TERRAIN | TGF_DOODAD | TGF_GUTDOODAD;

					bool bIsObs = m_akTGrid[h * iW + w].IsObs(m_iTStarObs, true);
					m_akTGrid[h * iW + w].iState = bIsObs ? TMPS_TLOCK : TMPS_NONE;

					m_akTGrid[h * iW + w].iIndex = h * iW + w;
				}
			}
		}
		else
		{
			m_akGrid = new TeMapGrid[m_iMaxIdx];
			memset(m_akGrid, 0, sizeof(TeMapGrid) * m_iMaxIdx);

			for (int h = 0; h < iH; h++)
			{
				for (int w = 0; w < iW; w++, akGrids++)
				{
					m_akGrid[h * iW + w].iObstacle = *akGrids;
					m_akGrid[h * iW + w].iObstacle &= TGF_TERRAIN | TGF_DOODAD | TGF_GUTDOODAD;
				}
			}
		}
	}
}

void TePathFinder::ClrObstacle(float fX, float fY, int iObstacle, int iSize)
{
	int iX(Map2Grid(fX)), iY(Map2Grid(fY));
	int iTX = iX - (iSize / 2);
	int iTY = iY - (iSize / 2);
	int iEX = iTX + iSize;
	int iEY = iTY + iSize;

	iTX = iTX < 0 ? 0 : iTX;
	iTY = iTY < 0 ? 0 : iTY;

	iEX = iEX >= m_iWidth ? m_iWidth - 1 : iEX;
	iEY = iEY >= m_iHeight ? m_iHeight - 1 : iEY;

	if (m_bUseTStar)
	{
		for (int y = iTY; y < iEY; y++)
		{
			for (int x = iTX; x < iEX; x++)
			{
				m_akTGrid[y * m_iWidth + x].iObstacle &= ~iObstacle;

				bool bIsObs = m_akTGrid[y * m_iWidth + x].IsObs(m_iTStarObs, true);
				m_akTGrid[y * m_iWidth + x].iState = bIsObs ? TMPS_TLOCK : TMPS_NONE;
			}
		}
	}
	else
	{
		for (int y = iTY; y < iEY; y++)
		{
			for (int x = iTX; x < iEX; x++)
			{
				m_akGrid[y * m_iWidth + x].iObstacle &= ~iObstacle;
			}
		}
	}
}

void TePathFinder::SetObstacle(float fX, float fY, int iObstacle, int iSize)
{
	int iX(Map2Grid(fX)), iY(Map2Grid(fY));
	int iTX = iX - (iSize / 2);
	int iTY = iY - (iSize / 2);
	int iEX = iTX + iSize;
	int iEY = iTY + iSize;

	iTX = iTX < 0 ? 0 : iTX;
	iTY = iTY < 0 ? 0 : iTY;

	iEX = iEX >= m_iWidth ? m_iWidth - 1 : iEX;
	iEY = iEY >= m_iHeight ? m_iHeight - 1 : iEY;

	if (m_bUseTStar)
	{
		for (int y = iTY; y < iEY; y++)
		{
			int iYvalue = y * m_iWidth;
			for (int x = iTX; x < iEX; x++)
			{
				m_akTGrid[iYvalue + x].iObstacle |= iObstacle;

				bool bIsObs = m_akTGrid[iYvalue + x].IsObs(m_iTStarObs, true);
				m_akTGrid[iYvalue + x].iState = bIsObs ? TMPS_TLOCK : TMPS_NONE;
			}
		}
	}
	else
	{
		for (int y = iTY; y < iEY; y++)
		{
			int iYvalue = y * m_iWidth;
			for (int x = iTX; x < iEX; x++)
			{
				m_akGrid[iYvalue + x].iObstacle |= iObstacle;
			}
		}
	}
}

inline bool TePathFinder::CheckInput(float& fSrcX, float& fSrcY, float& fDstX, float& fDstY) const
{
	fSrcX = fSrcX < GRID_COORD_SIZE ? GRID_COORD_SIZE : fSrcX;
	fSrcX = fSrcX > (float)((m_iWidth - 1) * GRID_COORD_SIZE) ? (float)((m_iWidth - 1) * GRID_COORD_SIZE) : fSrcX;

	fSrcY = fSrcY < GRID_COORD_SIZE ? GRID_COORD_SIZE : fSrcY;
	fSrcY = fSrcY > (float)((m_iHeight - 1) * GRID_COORD_SIZE) ? (float)((m_iHeight - 1) * GRID_COORD_SIZE) : fSrcY;

	fDstX = fDstX < GRID_COORD_SIZE ? GRID_COORD_SIZE : fDstX;
	fDstX = fDstX > (float)((m_iWidth - 1) * GRID_COORD_SIZE) ? (float)((m_iWidth - 1) * GRID_COORD_SIZE) : fDstX;

	fDstY = fDstY < GRID_COORD_SIZE ? GRID_COORD_SIZE : fDstY;
	fDstY = fDstY > (float)((m_iHeight - 1) * GRID_COORD_SIZE) ? (float)((m_iHeight - 1) * GRID_COORD_SIZE) : fDstY;

	return true;
}

TeFindResult TePathFinder::FindPathUnit(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize, int iDistance, int iObs)
{
	m_iChangeListLimit = PATH_CHANGE_LIST;

	CheckInput(fSrcX, fSrcY, fDstX, fDstY);
	TeFindResult eRet = TFR_NONE;

	if (fSrcX == fDstX && fSrcY == fDstY)
	{
		eRet = TFR_ARRIVED;
	}
	else
	{
		if (IsPointDirect(fSrcX, fSrcY, iSrcSize, fDstX, fDstY, iDstSize, (float)iDistance, iObs))
		{
			m_iPathPointCount = 0;
			m_piPathPoint[m_iPathPointCount++] = TePos2(fDstX, fDstY);
			eRet = TFR_DIRECT;
		}
	}

	if (eRet == TFR_NONE)
	{
		if (m_bUseTStar)
		{
			CreateList();

			eRet = FindPathTStarGrid(Map2Grid(fSrcX), Map2Grid(fSrcY), iSrcSize, Map2Grid(fDstX), Map2Grid(fDstY));

			ReleaseList();
		}
		else
		{
			CreateList();

			eRet = FindPathGrid_Edge_Safe(Map2Grid(fSrcX), Map2Grid(fSrcY), iSrcSize, Map2Grid(fDstX), Map2Grid(fDstY), iDistance, iObs);

			SmoothPath(fSrcX, fSrcY, iSrcSize, fDstX, fDstY, iDstSize, iDistance, iObs, eRet);

			ReleaseList();
		}
	}

	return eRet;
}

#define INC_VALUE (GRID_COORD_SIZE)
inline int IncGrid(int x)
{
	if (x % INC_VALUE)
	{
		if ((x & (INC_VALUE - 1)) == (INC_VALUE - 1))
		{
			return x + 1;
		}
		return (x | (INC_VALUE - 1));
	}
	else
	{
		return x + 1;
	}
}

inline int DecGrid(int x)
{
	if (x % INC_VALUE)
	{
		return (x & ~(INC_VALUE - 1));
	}
	else
	{
		return x - 1;
	}
}

#define FLOAT_ERROR (0.02f)
#define FLOAT_STEP (32.0f)

inline float IncGrid(float x)
{
	int iX = IncGrid((int)x);
	float fX = (float)iX;
	if (iX & 1)
	{
		fX = fX + 0.998f;
	}
	return fX;
}

inline float DecGrid(float x)
{
	int iX = DecGrid((int)x);
	float fX = (float)iX;
	if (iX & 1)
	{
		fX = fX + 0.998f;
	}
	return fX;
}

inline float f_line_y(float x, float x1, float y1, float x2, float y2, float fScope)
{
	return ((y1 - y2) * x + (x1 * y2 - x2 * y1)) * fScope; ///(x1-x2);
}

inline float f_line_x(float y, float x1, float y1, float x2, float y2, float fScope)
{
	return ((x1 - x2) * y + (x2 * y1 - x1 * y2)) * fScope;///(y1-y2);
}

int		TePathFinder::GetFirstCanStay(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, float& fX, float& fY, float fDistance, int iObs) const
{
	CheckInput(fSrcX, fSrcY, fDstX, fDstY);
	float iX1 = fSrcX;
	float iY1 = fSrcY;
	float iX2 = fDstX;
	float iY2 = fDstY;

	m_iTlockType = 0;
	m_kSrcPos.fX = fSrcX;
	m_kSrcPos.fY = fSrcY;

	m_kDstPos.fX = fDstX;
	m_kDstPos.fY = fDstY;
	if ((int)iX1 == (int)iX2 && (int)iY1 == (int)iY2)
	{
		fX = fDstX;
		fY = fDstY;
		return 1;
	}

	float iX = fSrcX;
	float iY = fSrcY;
	float dx = iX2 - iX1;
	float dy = iY2 - iY1;

	fDistance += 1.0f;
	float iDis2 = (fDistance * fDistance);

	bool bFirst = false;
	if (abs(dx) >= abs(dy))
	{
		if (dx > 0)
		{
			float fScope = 1.0f / (fSrcX - fDstX);
			float fTaseY = f_line_y(iX1, fSrcX, fSrcY, fDstX, fDstY, fScope);
			float fDeltaY = fTaseY - iY1;
			float y;
			for (float x = iX1; x <= iX2; x = (float)IncGrid(x))
			{
				// float y = iY1 + dy * (x - iX1) / dx;
				y = f_line_y(x, fSrcX, fSrcY, fDstX, fDstY, fScope) - fDeltaY;
				if (!CanStayPos((float)x, (float)y, iSrcSize, iObs))
				{
					if (bFirst)
					{
						bFirst = false;
						continue;
					}
					fX = (float)iX;
					fY = (float)iY;
					return 2;

				}
				bFirst = false;
				iX = x;
				iY = y;

				if (((x - iX1) * (x - iX1) + (y - iY1) * (y - iY1)) >= iDis2)
				{
					fX = (float)iX;
					fY = (float)iY;
					return 3;
				}
			}
		}
		else
		{
			float fScope = 1.0f / (fSrcX - fDstX);
			float fTaseY = f_line_y(iX1, fSrcX, fSrcY, fDstX, fDstY, fScope);
			float fDeltaY = fTaseY - iY1;
			float y;
			for (float x = iX1; x >= iX2; x = (float)DecGrid(x))
			{
				y = f_line_y(x, fSrcX, fSrcY, fDstX, fDstY, fScope) - fDeltaY;
				if (!CanStayPos((float)x, (float)y, iSrcSize, iObs))
				{
					if (bFirst)
					{
						bFirst = false;
						continue;
					}
					fX = (float)iX;
					fY = (float)iY;
					return 2;

				}
				bFirst = false;
				iX = x;
				iY = y;

				if (((x - iX1) * (x - iX1) + (y - iY1) * (y - iY1)) >= iDis2)
				{
					fX = (float)iX;
					fY = (float)iY;
					return 3;
				}
			}
		}
	}
	else
	{
		if (dy > 0)
		{
			float fScope = 1.0f / (fSrcY - fDstY);
			float fTaseX = f_line_x(iY1, fSrcX, fSrcY, fDstX, fDstY, fScope);
			float fDeltaX = fTaseX - iX1;
			float x;
			for (float y = iY1; y <= iY2; y = (float)IncGrid(y))
			{
				// float x = iX1 + dx * (y - iY1) / dy;
				x = f_line_x(y, fSrcX, fSrcY, fDstX, fDstY, fScope) - fDeltaX;
				if (!CanStayPos((float)x, (float)y, iSrcSize, iObs))
				{
					if (bFirst)
					{
						bFirst = false;
						continue;
					}
					fX = (float)iX;
					fY = (float)iY;
					return 2;

				}
				bFirst = false;
				iX = x;
				iY = y;

				if (((x - iX1) * (x - iX1) + (y - iY1) * (y - iY1)) >= iDis2)
				{
					fX = (float)iX;
					fY = (float)iY;
					return 3;
				}
			}
		}
		else
		{
			float fScope = 1.0f / (fSrcY - fDstY);
			float fTaseX = f_line_x(iY1, fSrcX, fSrcY, fDstX, fDstY, fScope);
			float fDeltaX = fTaseX - iX1;
			float x;
			for (float y = iY1; y >= iY2; y = (float)DecGrid(y))
			{
				x = f_line_x(y, fSrcX, fSrcY, fDstX, fDstY, fScope) - fDeltaX;
				if (!CanStayPos((float)x, (float)y, iSrcSize, iObs))
				{
					if (bFirst)
					{
						bFirst = false;
						continue;
					}
					fX = (float)iX;
					fY = (float)iY;
					return 2;

				}
				bFirst = false;
				iX = x;
				iY = y;

				if (((x - iX1) * (x - iX1) + (y - iY1) * (y - iY1)) >= iDis2)
				{
					fX = (float)iX;
					fY = (float)iY;
					return 3;
				}
			}
		}
	}

	if (CanStayPos(fDstX, fDstY, iSrcSize, iObs))
	{
		fX = fDstX;
		fY = fDstY;
		return 5;
	}
	else
	{
		fX = (float)iX;
		fY = (float)iY;
		return 4;
	}
}

bool TePathFinder::GetNearestCanStay(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, float& fX, float& fY, float fDistance, int iObs) const
{
	CheckInput(fSrcX, fSrcY, fDstX, fDstY);

	fX = fSrcX;
	fY = fSrcY;
	int iSrcX = (int)fSrcX;
	int iSrcY = (int)fSrcY;
	int iDstX = (int)fDstX;
	int iDstY = (int)fDstY;

	if (CanStayPos(fDstX, fDstY, iSrcSize, iObs))
	{
		fX = fDstX;
		fY = fDstY;
		return true;
	}

	int iDistanceTempLeft = GetDistance2Path(iSrcX, iSrcY, iDstX, iDstY);
	int iDistanceTempRight = static_cast<int>(fDistance * fDistance);
	int iDistance2 = iDistanceTempLeft > iDistanceTempRight ? iDistanceTempLeft : iDistanceTempRight;
	int iDX = iSrcX;
	int iDY = iSrcY;
	for (int iRange = (GRID_COORD_SIZE / 2); iRange <= (int)fDistance + GRID_COORD_SIZE; iRange += (GRID_COORD_SIZE / 2))
	{
		bool bFind = false;
		int iLeft = iDstX - iRange;
		iLeft = iDstX > iRange ? iDstX - iRange : 0;
		int iRight = iDstX + iRange < m_iWidth * GRID_COORD_SIZE ? iDstX + iRange : m_iWidth * GRID_COORD_SIZE - 1;
		int iTop = iDstY > iRange ? iDstY - iRange : 0;
		int iTottom = iDstY + iRange < m_iHeight * GRID_COORD_SIZE ? iDstY + iRange : m_iHeight * GRID_COORD_SIZE - 1;
		int iX = 0;
		int iY = 0;

		iY = iTop;
		for (iX = iLeft; iX < iRight; iX = IncGrid(iX))
		{
			if (CanStayPos(iX, iY, iSrcSize, iObs))
			{
				int iD2 = GetDistance2Path(iX, iY, iDstX, iDstY);
				if (iD2 < iDistance2)
				{
					bFind = true;
					iDistance2 = iD2;
					iDX = iX;
					iDY = iY;
				}
			}
		}

		iX = iRight;
		for (iY = iTop; iY < iTottom; iY = IncGrid(iY))
		{
			if (CanStayPos(iX, iY, iSrcSize, iObs))
			{
				int iD2 = GetDistance2Path(iX, iY, iDstX, iDstY);
				if (iD2 < iDistance2)
				{
					bFind = true;
					iDistance2 = iD2;
					iDX = iX;
					iDY = iY;
				}
			}
		}

		iY = iTottom;
		for (iX = iRight; iX > iLeft; iX = DecGrid(iX))
		{
			if (CanStayPos(iX, iY, iSrcSize, iObs))
			{
				int iD2 = GetDistance2Path(iX, iY, iDstX, iDstY);
				if (iD2 < iDistance2)
				{
					bFind = true;
					iDistance2 = iD2;
					iDX = iX;
					iDY = iY;
				}
			}
		}

		iX = iLeft;
		for (iY = iTottom; iY > iTop; iY = DecGrid(iY))
		{
			if (CanStayPos(iX, iY, iSrcSize, iObs))
			{
				int iD2 = GetDistance2Path(iX, iY, iDstX, iDstY);
				if (iD2 < iDistance2)
				{
					bFind = true;
					iDistance2 = iD2;
					iDX = iX;
					iDY = iY;
				}
			}
		}
		if (bFind)
		{
			fX = (float)iDX;
			fY = (float)iDY;
			return true;
		}
	}
	return false;
}

bool TePathFinder::IsObstacle(float fX, float fY, int iObstacle, int iSize)
{
	return !CanStayPos(fX, fY, iSize, iObstacle);
}

inline bool TePathFinder::CanStayPos(float fX, float fY, int iSize, int iObs) const
{
	return CanStayGrid(Map2Grid(fX), Map2Grid(fY), iSize, iObs);
}

inline bool TePathFinder::CanStayPos(int iX, int iY, int iSize, int iObs) const
{
	return CanStayGrid(Map2Grid(iX), Map2Grid(iY), iSize, iObs);
}

inline bool TePathFinder::CanStayGrid(int iGridX, int iGridY, int iSize, int iObs) const
{
	int iTX = iGridX - (iSize / 2);
	int iTY = iGridY - (iSize / 2);
	int iEX = iTX + iSize;
	int iEY = iTY + iSize;
	m_iTlockX = iGridX;
	m_iTlockY = iGridY;
	if (iTX < 0 || iTY < 0 || iEX >= m_iWidth || iEY >= m_iHeight)
	{
		return false;
	}

	if (m_bUseTStar)
	{
		for (int y = iTY; y < iEY; y++)
		{
			int iYValue = y * m_iWidth;
			for (int x = iTX; x < iEX; x++)
			{
				int iIndex = iYValue + x;
				if (m_akTGrid[iIndex].IsObs(iObs, HasVision(x, y)))
				{
					m_iTlockType = m_akTGrid[iIndex].iObstacle & iObs;

					return false;
				}
			}
		}
	}
	else
	{
		for (int y = iTY; y < iEY; y++)
		{
			int iYValue = y * m_iWidth;
			for (int x = iTX; x < iEX; x++)
			{
				int iIndex = iYValue + x;
				if (m_akGrid[iIndex].IsObs(iObs, HasVision(x, y)))
				{
					m_iTlockType = m_akGrid[iIndex].iObstacle & iObs;

					return false;
				}
			}
		}
	}

	return true;
}

inline bool TePathFinder::CanStayGrid(int iIndex, int iSize, int iObs) const
{
	int iIndexTop = iIndex - (iSize / 2) * m_iWidth - (iSize / 2);
	int iIndexTottom = iIndexTop + iSize * m_iWidth;
	int iIndexTottomRight = iIndexTottom + iSize;
	if (iIndexTop < 0 || iIndexTottomRight >= m_iMaxIdx)
	{
		return false;
	}

	if (m_bUseTStar)
	{
		if (m_akTGrid[iIndex].IsObs(iObs, HasVision(iIndex % m_iWidth, iIndex / m_iWidth)))
		{
			return false;
		}

		for (int iIndexStart = iIndexTop; iIndexStart < iIndexTottom; iIndexStart += m_iWidth)
		{
			for (int iOff = 0; iOff < iSize; ++iOff)
			{
				if (iIndexStart + iOff >= m_iMaxIdx)
				{
					continue;
				}
				int iIndex = iIndexStart + iOff;
				if (m_akTGrid[iIndexStart + iOff].IsObs(iObs, HasVision(iIndex % m_iWidth, iIndex / m_iWidth)))
				{
					return false;
				}
			}
		}
	}
	else
	{
		if (m_akGrid[iIndex].IsObs(iObs, HasVision(iIndex % m_iWidth, iIndex / m_iWidth)))
		{
			return false;
		}

		for (int iIndexStart = iIndexTop; iIndexStart < iIndexTottom; iIndexStart += m_iWidth)
		{
			for (int iOff = 0; iOff < iSize; ++iOff)
			{
				if (iIndexStart + iOff >= m_iMaxIdx)
				{
					continue;
				}
				int iIndex = iIndexStart + iOff;
				if (m_akGrid[iIndexStart + iOff].IsObs(iObs, HasVision(iIndex % m_iWidth, iIndex / m_iWidth)))
				{
					return false;
				}
			}
		}
	}

	return true;
}

void TePathFinder::CreatePath(int iNearestIndex, int iStart)
{
	const int aiOff[][5] =
	{
		{-1,-1,1,7,-m_iWidth - 1}, {0,-1,2,5,-m_iWidth},{1,-1,3,7,-m_iWidth + 1},{1,0,4,5,1},
		{1,1,5,7,m_iWidth + 1},{0,1,6,5,m_iWidth},{-1,1,7,7,m_iWidth - 1},{-1,0,8,5,-1},
	};
	int iLastGridX = iNearestIndex % m_iWidth;
	int iLastGridY = iNearestIndex / m_iWidth;
	m_piPathPoint[PATH_RESULT - 1] = TePos2(Grid2Map(iLastGridX), Grid2Map(iLastGridY));

	while (iNearestIndex != iStart)
	{
		if (m_iPathCount >= PATH_CHANGE_LIST || iNearestIndex < 0 || iNearestIndex >= m_iMaxIdx)
		{
			break;
		}
		m_piPath[m_iPathCount++] = iNearestIndex;

		TeMapGrid& kGrid = m_akGrid[iNearestIndex];
		int iParent = kGrid.iParent - 1;
		int iGridX = iNearestIndex % m_iWidth;
		int iGridY = iNearestIndex / m_iWidth;
		if (iParent < 0 || iParent >= sizeof(aiOff) / sizeof(aiOff[0]))
		{
			break;
		}

		iGridX -= aiOff[iParent][0];
		iGridY -= aiOff[iParent][1];

		iNearestIndex = GridIndex(iGridX, iGridY);
	}

	for (int i = 0; i < m_iChangeCount; ++i)
	{
		int iIdx = m_piChange[i];
		if (iIdx < 0 || iIdx >= m_iMaxIdx)
		{
			continue;
		}
		TeMapGrid& kGrid = m_akGrid[iIdx];
		kGrid.iParent = 0;
		kGrid.iState = TGS_NONE;
	}
	m_iChangeCount = 0;
}

bool TePathFinder::IsPointDirect(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize, float fDistance, int iObs)
{
	float fX(fSrcX), fY(fSrcY);
	GetFirstCanStay(fSrcX, fSrcY, iSrcSize, fDstX, fDstY, fX, fY, 512 * 32, iObs);
	if (fX == fDstX && fY == fDstY)
	{
		return true;
	}
	else
	{
		float fDistance2 = GetDistance2Path(fX, fY, fDstX, fDstY);
		if (fDistance2 <= fDistance * fDistance)
		{
			return true;
		}
	}
	return false;
}


void TePathFinder::SmoothPath(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize, int iDistance, int iObs, TeFindResult eRet)
{
	m_iPathPointCount = 0;
	if (m_iPathCount <= 0)
	{
		return;
	}
	int iEnd = 0;
	int iStart = m_iPathCount - 1;

	float fPosX = fSrcX;
	float fPosY = fSrcY;
	float fEndX = fDstX;
	float fEndY = fDstY;
	int iPos1 = iStart;
	float fPos1X = fSrcX;
	float fPos1Y = fSrcY;

	int iPos2 = iPos1;
	int iPos3 = iPos1;
	int iFindEnd = m_iPathCount - 100;
	if (iFindEnd < 0)
	{
		iFindEnd = 0;
	}
	for (int iPos = iFindEnd; iPos < m_iPathCount; ++iPos)
	{
		int iIndex = m_piPath[iPos];
		int iX = iIndex % m_iWidth;
		int iY = iIndex / m_iWidth;
		fPos1X = Grid2Map(iX);
		fPos1Y = Grid2Map(iY);
		if (iPos == 0 && eRet != TFR_NOT_ARRIVE && iDistance == 0)
		{
			fPos1X = fEndX;
			fPos1Y = fEndY;
		}
		if (IsPointDirect(fPosX, fPosY, iSrcSize, fPos1X, fPos1Y, iDstSize, 0, iObs))
		{
			iPos1 = iPos;
			break;
		}
	}

	iPos3 = iPos1;

	int iIndex3 = m_piPath[iPos3];
	int iX3 = iIndex3 % m_iWidth;
	int iY3 = iIndex3 / m_iWidth;
	fPos1X = Grid2Map(iX3);
	fPos1Y = Grid2Map(iY3);
	if (iPos3 == 0 && eRet != TFR_NOT_ARRIVE && iDistance == 0)
	{
		fPos1X = fEndX;
		fPos1Y = fEndY;
	}

	if (iPos1 > iFindEnd)
	{
		iPos2 = iPos1 - 1;
		int iIndex = m_piPath[iPos2];
		int iX = iIndex % m_iWidth;
		int iY = iIndex / m_iWidth;
		fPosX = Grid2Map(iX);
		fPosY = Grid2Map(iY);
		if (iPos2 == iEnd && eRet != TFR_NOT_ARRIVE && iDistance == 0)
		{
			fPosX = fEndX;
			fPosY = fEndY;
		}

		float fX = fSrcX;
		float fY = fSrcY;
		float fDistance = GetDistancePath(fPos1X, fPos1Y, fSrcX, fSrcY);
		int iCount = 0;
		float fDD = 64.f;
		for (float fD = fDD; fD <= fDistance; fD += fDD)
		{
			if (3 == GetFirstCanStay(fSrcX, fSrcY, iSrcSize, fPos1X, fPos1Y, fX, fY, fD, iObs))
			{
				if (IsPointDirect(fX, fY, iSrcSize, fPosX, fPosY, iDstSize, 0, iObs))
				{
					iCount++;
					if (iCount > 1)
					{
						if (m_iPathPointCount < PATH_RESULT)
						{
							m_piPathPoint[m_iPathPointCount++] = TePos2(fX, fY);
						}
						return;
					}
				}
			}
		}

	}

	int iIndex = m_piPath[iPos3];
	int iX = iIndex % m_iWidth;
	int iY = iIndex / m_iWidth;
	float fX = Grid2Map(iX);
	float fY = Grid2Map(iY);
	if (iPos3 == 0 && eRet != TFR_NOT_ARRIVE && iDistance == 0)
	{
		fX = fEndX;
		fY = fEndY;
	}

	if (m_iPathPointCount < PATH_RESULT)
	{
		m_piPathPoint[m_iPathPointCount++] = TePos2(fX, fY);
	}
}

void TePathFinder::MarkLine(int iSrcGridX, int iSrcGridY, int iSize, int iDstGridX, int iDstGridY, int iDistance, int iObs)
{
	int iDX = abs(iSrcGridX - iDstGridX);
	int iDY = abs(iSrcGridY - iDstGridY);

	int iIncX = (iDstGridX - iSrcGridX) > 0 ? 1 : -1;
	int iIncY = (iDstGridY - iSrcGridY) > 0 ? 1 : -1;
	int iX = iSrcGridX;
	int iY = iSrcGridY;
	int iP = iDX - iDY;

	{
		int iIndex = GridIndex(iX, iY);
		TeMapGrid& kGrid = m_akGrid[iIndex];
		ChangeListPush(iIndex);

		if (CanStayGrid(iX, iY, iSize, iObs))
		{
			kGrid.iState = TGS_ONLINE;
		}
		else
		{
			kGrid.iState = TGS_OTS;
		}
	}
	{
		int iX = iDstGridX;
		int iY = iDstGridY;
		{
			int iIndex = GridIndex(iX, iY);
			TeMapGrid& kGrid = m_akGrid[iIndex];
			ChangeListPush(iIndex);

			if (CanStayGrid(iX, iY, iSize, iObs))
			{
				kGrid.iState = TGS_ONLINE;
			}
			else
			{
				kGrid.iState = TGS_OTS;
			}
		}
	}

	while (((iDX > iDY) && (iX != iDstGridX))
		|| ((iDX <= iDY) && (iY != iDstGridY)))
	{
		if (iP > 0)
		{
			iX += iIncX;
			iP -= iDY;
		}
		else
		{
			iY += iIncY;
			iP += iDX;
		}

		int iIndex = GridIndex(iX, iY);
		TeMapGrid& kGrid = m_akGrid[iIndex];
		ChangeListPush(iIndex);
		if (CanStayGrid(iX, iY, iSize, iObs))
		{
			kGrid.iState = TGS_ONLINE;
		}
		else
		{
			kGrid.iState = TGS_OTS;
		}
	}
}

TeFindResult TePathFinder::FindPathGrid_Edge(int iSrcGridX, int iSrcGridY, int iSize, int iDstGridX, int iDstGridY, int iDistance, int iObs)
{
	m_iPathCount = 0;
	if (!m_akGrid)
	{
		return TFR_NONE;
	}

	if (iSrcGridX < 0 || iSrcGridY < 0 || iSrcGridX >= m_iWidth || iSrcGridY >= m_iHeight)
	{
		return TFR_NONE;
	}

	if (iDstGridX < 0 || iDstGridY < 0 || iDstGridX >= m_iWidth || iDstGridY >= m_iHeight)
	{
		return TFR_NONE;
	}

	if (iSrcGridX == iDstGridX && iSrcGridY == iDstGridY)
	{
		return TFR_ARRIVED;
	}

	TeFindResult eRet = TFR_NONE;

	MarkLine(iSrcGridX, iSrcGridY, iSize, iDstGridX, iDstGridY, iDistance, iObs);

	int iStart = GridIndex(iSrcGridX, iSrcGridY);
	TeMapGrid& kStartGrid = m_akGrid[iStart];
	kStartGrid.iState = TGS_OPEN;
	OpenListPushTack(iStart);
	ChangeListPush(iStart);

	const int aiOff[][3] =
	{
		{2,-m_iWidth,6},{4,1,0},{6,m_iWidth,2},{8,-1,4},
	};

	const int aiParent[5][2] =
	{
		{0,4},
		{3,6},
		{0,3},
		{1,4},
		{2,5},
	};

	const int aiOut[] =
	{
		-m_iWidth - 1, -m_iWidth,-m_iWidth + 1,1,
		m_iWidth + 1,m_iWidth,m_iWidth - 1,-1,
	};

	int iDistance2 = GetDistance2Path(iSrcGridX, iSrcGridY, iDstGridX, iDstGridY);
	int iDistanceLimit2 = iDistance * iDistance / (GRID_COORD_SIZE * GRID_COORD_SIZE);
	int iNearestIndex = iStart;

	while (OpenListGetCount() > 0 && !ChangeListFull())
	{
		int iIndex = OpenListPopHead();
		if (iIndex < 0 || iIndex >= m_iMaxIdx)
		{
			continue;
		}
		int iGridX = iIndex % m_iWidth;
		int iGridY = iIndex / m_iWidth;
		TeMapGrid& kGrid = m_akGrid[iIndex];
		kGrid.iState = TGS_CLOSE;

		int iC1 = aiParent[kGrid.iParent / 2][0];
		int iC2 = aiParent[kGrid.iParent / 2][1];
		for (int iChild = iC1; iChild < iC2; ++iChild)
		{
			int i = iChild & 3; // % 4
			int iIndex1 = iIndex + aiOff[i][1];
			if (iIndex1 < 0 || iIndex1 >= m_iMaxIdx)
			{
				continue;
			}

			TeMapGrid& kGrid1 = m_akGrid[iIndex1];

			switch (kGrid1.iState)
			{
			case TGS_NONE:
				ChangeListPush(iIndex1);
				if (!CanStayGrid(iIndex1, iSize, iObs))
				{
					kGrid1.iState = TGS_OTS;
					break;
				}
			case TGS_NOOTS:
			{
				bool bOK = false;
				int iStart = 0;
				int iEnd = 8;

				for (int iOut = iStart; iOut < iEnd; ++iOut)
				{
					int iIndexOut = iIndex1 + aiOut[iOut % 8];
					if (iIndexOut < 0 || iIndexOut >= m_iMaxIdx)
					{
						continue;
					}
					TeMapGrid& kGridOut = m_akGrid[iIndexOut];

					if (kGridOut.iState == TGS_NONE)
					{
						ChangeListPush(iIndexOut);
						if (!CanStayGrid(iIndexOut, iSize, iObs))
						{
							kGridOut.iState = TGS_OTS;
							bOK = true;
							break;
						}
						else
						{
							kGridOut.iState = TGS_NOOTS;
						}
					}
					else if (kGridOut.iState == TGS_OTS)
					{
						bOK = true;
						break;
					}
					else
					{
					}
				}

				if (!bOK)
				{
					kGrid1.iState = TGS_CLOSE;
					break;
				}
				else
				{
					int iGridX1 = iIndex1 % m_iWidth;
					int iGridY1 = iIndex1 / m_iWidth;
					if (abs(iGridX - iGridX1) > 1 || abs(iGridY - iGridY1) > 1)
					{
						kGrid1.iState = TGS_CLOSE;
						break;
					}
				}
			}
			case TGS_ONLINE:
			{
				kGrid1.iState = TGS_OPEN;
				kGrid1.iParent = aiOff[i][0];
				OpenListPushTack(iIndex1);
				int iGridX1 = iIndex1 % m_iWidth;
				int iGridY1 = iIndex1 / m_iWidth;

				int iD2 = GetDistance2Path(iGridX1, iGridY1, iDstGridX, iDstGridY);
				if (iDistance2 > iD2)
				{
					if (eRet == TFR_NONE)
					{
						eRet = TFR_NOT_ARRIVE;
					}
					iDistance2 = iD2;
					iNearestIndex = iIndex1;
					if (iD2 <= iDistanceLimit2)
					{
						eRet = TFR_ARRIVE;
						OpenListClear();
						break;
					}
				}
				break;
			}
			}
		}
	}

	CreatePath(iNearestIndex, iStart);

	return eRet;
}

bool TePathFinder::UnitCanReach(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize, int iDistance, int iObs)
{
	bool bRet = false;
	m_iChangeListLimit = 500;

	CheckInput(fSrcX, fSrcY, fDstX, fDstY);
	TeFindResult eRet = TFR_NONE;
	if (fSrcX == fDstX && fSrcY == fDstY)
	{
		eRet = TFR_ARRIVED;
		bRet = true;
	}
	else
	{
		if (IsPointDirect(fSrcX, fSrcY, iSrcSize, fDstX, fDstY, iDstSize, (float)iDistance, iObs))
		{
			m_iPathPointCount = 0;
			m_piPathPoint[m_iPathPointCount++] = TePos2(fDstX, fDstY);
			eRet = TFR_DIRECT;
			bRet = true;
		}
	}

	if (eRet == TFR_NONE)
	{
		CreateList();
		eRet = FindPathGrid_Edge(Map2Grid(fSrcX), Map2Grid(fSrcY), iSrcSize, Map2Grid(fDstX), Map2Grid(fDstY), iDistance, iObs);

		if (TFR_ARRIVE == eRet)
		{
			bRet = true;
		}
		ReleaseList();

	}
	return bRet;
}

TeFindResult TePathFinder::FindPathGrid_Edge_Safe(int iSrcGridX, int iSrcGridY, int iSize, int iDstGridX, int iDstGridY, int iDistance, int iObs, int iSrcParent)
{
	if (iSrcGridX == iDstGridX && iSrcGridY == iDstGridY)
	{
		return TFR_ARRIVED;
	}

	m_iPathCount = 0;
	if (!m_akGrid)
	{
		return TFR_NONE;
	}

	if (iSrcGridX < 0 || iSrcGridY < 0 || iSrcGridX >= m_iWidth || iSrcGridY >= m_iHeight)
	{
		return TFR_NONE;
	}

	if (iDstGridX < 0 || iDstGridY < 0 || iDstGridX >= m_iWidth || iDstGridY >= m_iHeight)
	{
		return TFR_NONE;
	}

	TeFindResult eRet = TFR_NONE;

	MarkLine(iSrcGridX, iSrcGridY, iSize, iDstGridX, iDstGridY, iDistance, iObs);

	int iStart = GridIndex(iSrcGridX, iSrcGridY);
	TeMapGrid& kStartGrid = m_akGrid[iStart];
	kStartGrid.iState = TGS_OPEN;
	OpenListPushTack(iStart);
	ChangeListPush(iStart);

	int iDistance2 = GetDistance2Path(iSrcGridX, iSrcGridY, iDstGridX, iDstGridY);
	int iDistanceLimit2 = iDistance * iDistance / (GRID_COORD_SIZE * GRID_COORD_SIZE);
	int iNearestIndex = iStart;
	int aiChild[4][2] = { 0 };
	int aiParent[4] = { 0 };
	while (OpenListGetCount() > 0 && !ChangeListFull())
	{
		int iIndex = OpenListPopHead();
		int iGridX = iIndex % m_iWidth;
		int iGridY = iIndex / m_iWidth;
		TeMapGrid& kGrid = m_akGrid[iIndex];
		kGrid.iState = TGS_CLOSE;


		switch (kGrid.iParent)
		{
		case 2:
		{
			aiChild[0][0] = iGridX;
			aiChild[0][1] = iGridY - 1;
			aiParent[0] = 2;
			if (aiChild[0][1] < 0)
			{
				aiChild[0][1] = 0;
				aiParent[0] = 0;
			}

			aiChild[1][0] = iGridX > 0 ? iGridX - 1 : iGridX;
			aiChild[1][1] = iGridY;
			aiParent[1] = iGridX > 0 ? 8 : 0;

			aiChild[2][0] = iGridX < m_iWidth - 1 ? iGridX + 1 : iGridX;
			aiChild[2][1] = iGridY;
			aiParent[2] = iGridX < m_iWidth - 1 ? 4 : 0;

			aiChild[3][0] = iGridX;
			aiChild[3][1] = iGridY < m_iHeight - 1 ? iGridY + 1 : iGridY;
			aiParent[3] = iGridY < m_iHeight - 1 ? 6 : 0;
			break;
		}
		case 4:
		{
			aiChild[0][0] = iGridX < m_iWidth - 1 ? iGridX + 1 : iGridX;
			aiChild[0][1] = iGridY;
			aiParent[0] = iGridX < m_iWidth - 1 ? 4 : 0;

			aiChild[1][0] = iGridX;
			aiChild[1][1] = iGridY > 0 ? iGridY - 1 : iGridY;
			aiParent[1] = iGridY > 0 ? 2 : 0;

			aiChild[2][0] = iGridX;
			aiChild[2][1] = iGridY < m_iHeight - 1 ? iGridY + 1 : iGridY;
			aiParent[2] = iGridY < m_iHeight - 1 ? 6 : 0;

			aiChild[3][0] = iGridX > 0 ? iGridX - 1 : iGridX;
			aiChild[3][1] = iGridY;
			aiParent[3] = iGridX > 0 ? 8 : 0;
			break;
		}
		case 6:
		{
			aiChild[0][0] = iGridX;
			aiChild[0][1] = iGridY < m_iHeight - 1 ? iGridY + 1 : iGridY;
			aiParent[0] = iGridY < m_iHeight - 1 ? 6 : 0;

			aiChild[1][0] = iGridX > 0 ? iGridX - 1 : iGridX;
			aiChild[1][1] = iGridY;
			aiParent[1] = iGridX > 0 ? 8 : 0;

			aiChild[2][0] = iGridX < m_iWidth - 1 ? iGridX + 1 : iGridX;
			aiChild[2][1] = iGridY;
			aiParent[2] = iGridX < m_iWidth - 1 ? 4 : 0;

			aiChild[3][0] = iGridX;
			aiChild[3][1] = iGridY > 0 ? iGridY - 1 : iGridY;
			aiParent[3] = iGridY > 0 ? 2 : 0;
			break;
		}
		case 8:
		{
			aiChild[0][0] = iGridX > 0 ? iGridX - 1 : iGridX;
			aiChild[0][1] = iGridY;
			aiParent[0] = iGridX > 0 ? 8 : 0;

			aiChild[1][0] = iGridX;
			aiChild[1][1] = iGridY > 0 ? iGridY - 1 : iGridY;
			aiParent[1] = iGridY > 0 ? 2 : 0;

			aiChild[2][0] = iGridX;
			aiChild[2][1] = iGridY < m_iHeight - 1 ? iGridY + 1 : iGridY;
			aiParent[2] = iGridY < m_iHeight - 1 ? 6 : 0;

			aiChild[3][0] = iGridX < m_iWidth - 1 ? iGridX + 1 : iGridX;
			aiChild[3][1] = iGridY;
			aiParent[3] = iGridX < m_iWidth - 1 ? 4 : 0;
			break;
		}
		default:
		{
			aiChild[0][0] = iGridX > 0 ? iGridX - 1 : iGridX;
			aiChild[0][1] = iGridY;
			aiParent[0] = iGridX > 0 ? 8 : 0;

			aiChild[1][0] = iGridX;
			aiChild[1][1] = iGridY > 0 ? iGridY - 1 : iGridY;
			aiParent[1] = iGridY > 0 ? 2 : 0;

			aiChild[2][0] = iGridX;
			aiChild[2][1] = iGridY < m_iHeight - 1 ? iGridY + 1 : iGridY;
			aiParent[2] = iGridY < m_iHeight - 1 ? 6 : 0;

			aiChild[3][0] = iGridX < m_iWidth - 1 ? iGridX + 1 : iGridX;
			aiChild[3][1] = iGridY;
			aiParent[3] = iGridX < m_iWidth - 1 ? 4 : 0;
			break;
		}
		}

		for (int iChild = 0; iChild < 4; ++iChild)
		{
			int iGridX1 = aiChild[iChild][0];
			int iGridY1 = aiChild[iChild][1];

			int iIndex1 = iGridX1 + iGridY1 * m_iWidth;
			if (iIndex1 == iIndex)
			{
				continue;
			}

			TeMapGrid& kGrid1 = m_akGrid[iIndex1];

			switch (kGrid1.iState)
			{
			case TGS_NONE:
				ChangeListPush(iIndex1);
				if (!CanStayGrid(iIndex1, iSize, iObs))
				{
					kGrid1.iState = TGS_OTS;
					break;
				}
			case TGS_NOOTS:
			{
				bool bOK = false;

				int iTX = iGridX1 > 0 ? iGridX1 - 1 : iGridX1;
				int iEX = iGridX1 < m_iWidth - 1 ? iGridX1 + 1 : iGridX1;
				int iTY = iGridY1 > 0 ? iGridY1 - 1 : iGridY1;
				int iEY = iGridY1 < m_iHeight - 1 ? iGridY1 + 1 : iGridY1;

				for (int iNY = iTY; iNY <= iEY; ++iNY)
				{
					for (int iNX = iTX; iNX <= iEX; ++iNX)
					{
						int iIndexOut = iNX + iNY * m_iWidth;
						TeMapGrid& kGridOut = m_akGrid[iIndexOut];
						switch (kGridOut.iState)
						{
						case TGS_NONE:
						{
							ChangeListPush(iIndexOut);
							if (!CanStayGrid(iIndexOut, iSize, iObs))
							{
								kGridOut.iState = TGS_OTS;
								bOK = true;
								iNY = iEY + 1;
								iNX = iEX + 1;
							}
							else
							{
								kGridOut.iState = TGS_NOOTS;
							}
							break;
						}
						case TGS_OTS:
						{
							bOK = true;
							iNY = iEY + 1;
							iNX = iEX + 1;
							break;
						}
						};
					}
				}

				if (!bOK)
				{
					kGrid1.iState = TGS_CLOSE;
					break;
				}
			}
			case TGS_ONLINE:
			{
				kGrid1.iState = TGS_OPEN;
				kGrid1.iParent = aiParent[iChild];
				OpenListPushTack(iIndex1);

				int iD2 = GetDistance2Path(iGridX1, iGridY1, iDstGridX, iDstGridY);
				if (iDistance2 > iD2)
				{
					if (eRet == TFR_NONE)
					{
						eRet = TFR_NOT_ARRIVE;
					}
					iDistance2 = iD2;
					iNearestIndex = iIndex1;
					if (iD2 <= iDistanceLimit2)
					{
						eRet = TFR_ARRIVE;
						OpenListClear();
						break;
					}
				}
				break;
			}
			}
		}
	}

	CreatePath(iNearestIndex, iStart);

	return eRet;
}

TagAround  g_akTagAround[MAX_MAP_DIRECTION] =
{
	{0, 1},
	{-1, 0},
	{0, -1},
	{1, 0},
};

int g_aiTranchAround[TMT_MAX][MAX_MAP_DIRECTION] =
{
	{0, 1, 2, 3,},
	{0, 3, 2, 1,},
	{0, 1, 2, 3,},
};

TeFindResult TePathFinder::FindPathTStarGrid(int iSrcGridX, int iSrcGridY, int iSize, int iDstGridX, int iDstGridY)
{
	if (iSrcGridX == iDstGridX && iSrcGridY == iDstGridY)
	{
		return TFR_ARRIVED;
	}

	m_iPathCount = 0;
	if (!m_akTGrid)
	{
		return TFR_NONE;
	}

	if (iSrcGridX < 0 || iSrcGridY < 0 || iSrcGridX >= m_iWidth || iSrcGridY >= m_iHeight)
	{
		return TFR_NONE;
	}

	if (iDstGridX < 0 || iDstGridY < 0 || iDstGridX >= m_iWidth || iDstGridY >= m_iHeight)
	{
		return TFR_NONE;
	}

	VecTMapGrid akOpenList;
	VecTMapGrid akTackList;

	int iSrcGridIndex = GridIndex(iSrcGridX, iSrcGridY);
	int iDstGridIndex = GridIndex(iDstGridX, iDstGridY);
	TeTMapGrid* pkOriginGrid = &m_akTGrid[iSrcGridIndex];
	TeTMapGrid* pkTargetGrid = &m_akTGrid[iDstGridIndex];

	pkOriginGrid = CorrectOriginGrid(pkOriginGrid);

	if (!pkOriginGrid || pkOriginGrid->iState == TMPS_TLOCK || !pkTargetGrid || pkTargetGrid->iState == TMPS_TLOCK)
	{
		return TFR_NONE;
	}

	pkOriginGrid->iState = TMPS_ORIGIN;
	pkTargetGrid->iState = TMPS_TARGET;
	pkTargetGrid->bChange = true;

	ChangeListPush(iSrcGridIndex);
	ChangeListPush(iDstGridIndex);

	akOpenList.push_back(pkOriginGrid);

	VecTMapGrid::iterator itrGrid;
	TeTMapGrid* pkCurGrid = NULL;
	TeTMapGrid* pkNextGrid = NULL;
	TeTMapGrid* pkLeftGrid = NULL;
	TeTMapGrid* pkRightGrid = NULL;
	bool bCancelTranch = false;
	int iDir = 0, iNextX = 0, iNextY = 0, iNextIndex = 0, iCount = 0;

	while (!akOpenList.empty())
	{
		akTackList.clear();

		for (itrGrid = akOpenList.begin(); itrGrid != akOpenList.end(); ++itrGrid)
		{
			pkNextGrid = NULL;
			pkCurGrid = *itrGrid;
			if (!pkCurGrid)
			{
				continue;
			}

			ChangeListPush(pkCurGrid->iIndex);

			if (pkCurGrid->iState != TMPS_ORIGIN)
			{
				pkCurGrid->iState = TMPS_CHECK;
			}

			if (pkCurGrid->iTranch == TMT_NONE)
			{
				for (iDir = 0; iDir < MAX_MAP_DIRECTION; ++iDir)
				{
					iNextX = pkCurGrid->x + g_akTagAround[iDir].x;
					iNextY = pkCurGrid->y + g_akTagAround[iDir].y;

					if (iNextY < 0 || iNextY >= m_iHeight || iNextX < 0 || iNextX >= m_iWidth)
					{
						pkCurGrid->iState = TMPS_CLOSE;
						continue;
					}

					int iDirDiff = BSF_GetDirDiff(pkCurGrid->x, pkCurGrid->y, pkTargetGrid->x, pkTargetGrid->y, iNextX, iNextY);
					if (iDirDiff <= 8)
					{
						pkNextGrid = &m_akTGrid[iNextY * m_iWidth + iNextX];
						break;
					}
				}

				if (!pkNextGrid)
				{
					continue;
				}

				ChangeListPush(pkNextGrid->iIndex);

				switch (pkNextGrid->iState)
				{
				case TMPS_TARGET:
				{
					pkNextGrid->pkPreGrid = pkCurGrid;
					pkNextGrid->iStep = pkCurGrid->iStep + 1;

					CreateTStarPath(pkOriginGrid, pkTargetGrid);

					return TFR_ARRIVE;
				}
				break;
				case TMPS_NONE:
				{
					pkNextGrid->pkPreGrid = pkCurGrid;
					pkNextGrid->iStep = pkCurGrid->iStep + 1;
					pkNextGrid->iDir = iDir;
					pkNextGrid->iState = TMPS_OPEN;

					if (pkNextGrid->iDir != pkCurGrid->iDir && (pkCurGrid->pkPreGrid && pkNextGrid->iDir != pkCurGrid->pkPreGrid->iDir))
					{
						pkNextGrid->bChange = true;
						pkCurGrid->bChange = true;
					}

					akTackList.push_back(pkNextGrid);
				}
				break;
				case TMPS_OPEN:
				{
					pkCurGrid->iState = TMPS_CLOSE;
					continue;
				}
				break;
				case TMPS_CLOSE:
				{
					pkCurGrid->iState = TMPS_CLOSE;
					continue;
				}
				break;
				case TMPS_TLOCK:
				{
					pkLeftGrid = NULL;
					pkRightGrid = NULL;

					bCancelTranch = false;
					int iTestDir = 0;
					for (iTestDir = 0; iTestDir < MAX_MAP_DIRECTION; ++iTestDir)
					{
						if (g_aiTranchAround[TMT_LEFT][iTestDir] == iDir)
						{
							break;
						}
					}

					for (iTestDir = iTestDir + 1, iCount = 0; iCount < MAX_MAP_DIRECTION - 1; ++iTestDir, ++iCount)
					{
						iTestDir = (iTestDir == MAX_MAP_DIRECTION ? 0 : iTestDir);

						iNextX = pkCurGrid->x + g_akTagAround[g_aiTranchAround[TMT_LEFT][iTestDir]].x;
						iNextY = pkCurGrid->y + g_akTagAround[g_aiTranchAround[TMT_LEFT][iTestDir]].y;

						if (iNextY < 0 || iNextY >= m_iHeight || iNextX < 0 || iNextX >= m_iWidth)
						{
							bCancelTranch = true;
							break;
						}

						pkLeftGrid = &m_akTGrid[iNextY * m_iWidth + iNextX];
						if (!pkLeftGrid || pkLeftGrid->iState != TMPS_TLOCK)
						{
							break;
						}
					}

					if (!bCancelTranch && pkLeftGrid)
					{
						ChangeListPush(pkLeftGrid->iIndex);

						pkLeftGrid->iTranch = TMT_LEFT;
						pkLeftGrid->iDir = g_aiTranchAround[TMT_LEFT][iTestDir];

						switch (pkLeftGrid->iState)
						{
						case TMPS_NONE:
						{
							pkLeftGrid->pkPreGrid = pkCurGrid;
							pkLeftGrid->iStep = pkCurGrid->iStep + 1;
						}
						break;
						case TMPS_CLOSE:
						{
							if (pkLeftGrid->iTranch == TMT_RIGHT && (pkCurGrid->pkPreGrid && pkLeftGrid->x == pkCurGrid->pkPreGrid->x && pkLeftGrid->y == pkCurGrid->pkPreGrid->y))
							{
								bCancelTranch = true;
							}
							else if (pkLeftGrid->iStep > pkCurGrid->iStep + 1)
							{
								pkLeftGrid->pkPreGrid = pkCurGrid;
								pkLeftGrid->iStep = pkCurGrid->iStep + 1;
							}
						}
						break;
						case TMPS_OPEN:
						{
							if (pkLeftGrid->iStep > pkCurGrid->iStep + 1)
							{
								pkLeftGrid->pkPreGrid = pkCurGrid;
								pkLeftGrid->iStep = pkCurGrid->iStep + 1;
							}
						}
						break;
						default:
							break;
						}

						if (!bCancelTranch)
						{
							pkLeftGrid->iAngle = 0;
							pkLeftGrid->iReel = iCount + 1;
							pkLeftGrid->iState = TMPS_OPEN;

							int iAngle = BSF_GetDirAngle(pkOriginGrid->x, pkOriginGrid->y, pkLeftGrid->x, pkLeftGrid->y, pkNextGrid->x, pkNextGrid->y);
							iAngle = (iAngle > 32 ? (iAngle - 64) : -iAngle);
							pkLeftGrid->iAngle = iAngle;

							pkCurGrid->bChange = true;

							akTackList.push_back(pkLeftGrid);
						}
					}

					bCancelTranch = false;
					for (iTestDir = 0; iTestDir < MAX_MAP_DIRECTION; ++iTestDir)
					{
						if (g_aiTranchAround[TMT_RIGHT][iTestDir] == iDir)
						{
							break;
						}
					}

					for (iTestDir = iTestDir + 1, iCount = 0; iCount < MAX_MAP_DIRECTION - 1; ++iTestDir, ++iCount)
					{
						iTestDir = (iTestDir == MAX_MAP_DIRECTION ? 0 : iTestDir);

						iNextX = pkCurGrid->x + g_akTagAround[g_aiTranchAround[TMT_RIGHT][iTestDir]].x;
						iNextY = pkCurGrid->y + g_akTagAround[g_aiTranchAround[TMT_RIGHT][iTestDir]].y;

						if (iNextY < 0 || iNextY >= m_iHeight || iNextX < 0 || iNextX >= m_iWidth)
						{
							bCancelTranch = true;
							break;
						}

						pkRightGrid = &m_akTGrid[iNextY * m_iWidth + iNextX];
						if (!pkRightGrid || pkRightGrid->iState != TMPS_TLOCK)
						{
							break;
						}
					}

					if (!bCancelTranch && pkRightGrid)
					{
						ChangeListPush(pkRightGrid->iIndex);

						pkRightGrid->iTranch = TMT_RIGHT;
						pkRightGrid->iDir = g_aiTranchAround[TMT_RIGHT][iTestDir];

						switch (pkRightGrid->iState)
						{
						case TMPS_NONE:
						{
							pkRightGrid->pkPreGrid = pkCurGrid;
							pkRightGrid->iStep = pkCurGrid->iStep + 1;
						}
						break;
						case TMPS_CLOSE:
						{
							if (pkRightGrid->iTranch == TMT_LEFT && (pkCurGrid->pkPreGrid && pkRightGrid->x == pkCurGrid->pkPreGrid->x && pkRightGrid->y == pkCurGrid->pkPreGrid->y))
							{
								bCancelTranch = true;
							}
							else if (pkRightGrid->iStep > pkCurGrid->iStep + 1)
							{
								pkRightGrid->pkPreGrid = pkCurGrid;
								pkRightGrid->iStep = pkCurGrid->iStep + 1;
							}
						}
						break;
						case TMPS_OPEN:
						{
							if (pkRightGrid->iStep > pkCurGrid->iStep + 1)
							{
								pkRightGrid->pkPreGrid = pkCurGrid;
								pkRightGrid->iStep = pkCurGrid->iStep + 1;
							}
						}
						break;
						default:
							break;
						}

						if (!bCancelTranch)
						{
							pkRightGrid->iReel = iCount + 1;
							pkRightGrid->iState = TMPS_OPEN;

							int iAngle = BSF_GetDirAngle(pkOriginGrid->x, pkOriginGrid->y, pkRightGrid->x, pkRightGrid->y, pkNextGrid->x, pkNextGrid->y);
							iAngle = (iAngle > 32 ? (iAngle - 64) : iAngle);
							pkRightGrid->iAngle = iAngle;

							pkCurGrid->bChange = true;

							akTackList.push_back(pkRightGrid);
						}
					}
				}
				default:
					break;
				}
			}
			else
			{
				switch (pkCurGrid->iTranch)
				{
				case TMT_LEFT:
				{
					for (iDir = 0; iDir < MAX_MAP_DIRECTION; ++iDir)
					{
						if (g_aiTranchAround[TMT_LEFT][iDir] == pkCurGrid->iDir)
						{
							break;
						}
					}

					bool bTreak = false;

					iDir += 2 + 1;
					iDir = (iDir >= MAX_MAP_DIRECTION ? iDir - MAX_MAP_DIRECTION : iDir);
					for (iCount = 0; iCount < MAX_MAP_DIRECTION; ++iCount, ++iDir)
					{
						iDir = (iDir >= MAX_MAP_DIRECTION ? iDir - MAX_MAP_DIRECTION : iDir);
						int iNextDir = g_aiTranchAround[TMT_LEFT][iDir];

						iNextX = pkCurGrid->x + g_akTagAround[iNextDir].x;
						iNextY = pkCurGrid->y + g_akTagAround[iNextDir].y;

						if (iNextY < 0 || iNextY >= m_iHeight || iNextX < 0 || iNextX >= m_iWidth)
						{
							break;
						}

						pkNextGrid = &m_akTGrid[iNextY * m_iWidth + iNextX];
						if (!pkNextGrid)
						{
							continue;
						}

						ChangeListPush(pkNextGrid->iIndex);

						if (pkNextGrid->iState != TMPS_TLOCK)
						{
							if (pkNextGrid->iState == TMPS_TARGET)
							{
								pkNextGrid->pkPreGrid = pkCurGrid;
								pkNextGrid->iStep = pkCurGrid->iStep + 1;

								CreateTStarPath(pkOriginGrid, pkTargetGrid);

								return TFR_ARRIVE;
							}

							switch (pkNextGrid->iState)
							{
							case TMPS_NONE:
							{
								pkNextGrid->pkPreGrid = pkCurGrid;
								pkNextGrid->iStep = pkCurGrid->iStep + 1;
							}
							break;
							case TMPS_CLOSE:
							{
								if (pkNextGrid->iTranch == TMT_RIGHT && pkNextGrid == pkCurGrid->pkPreGrid && pkNextGrid->iDir != iNextDir)
								{
									bTreak = true;
									break;
								}
								else if (pkNextGrid->iTranch == TMT_LEFT && pkNextGrid->iDir == iNextDir)
								{
									bTreak = true;
									break;
								}
								else if (pkNextGrid->iTranch == TMT_NONE && pkNextGrid->iStep > pkCurGrid->iStep)
								{
									bTreak = true;
									break;
								}
								else if (pkNextGrid->iStep > pkCurGrid->iStep + 1)
								{
									pkNextGrid->pkPreGrid = pkCurGrid;
									pkNextGrid->iStep = pkCurGrid->iStep + 1;
								}
							}
							break;
							case TMPS_OPEN:
							{
								if (pkNextGrid->iTranch == TMT_LEFT && pkNextGrid->iDir == iNextDir)
								{
									bTreak = true;
									break;
								}
								else if (pkNextGrid->iStep > pkCurGrid->iStep + 1)
								{
									pkNextGrid->pkPreGrid = pkCurGrid;
									pkNextGrid->iStep = pkCurGrid->iStep + 1;
								}
							}
							break;
							default:
								break;
							}

							if (bTreak)
							{
								break;
							}

							int iAddReel = iCount - 1;
							pkNextGrid->iReel = pkCurGrid->iReel + iAddReel;
							pkNextGrid->iReel = (pkNextGrid->iReel < 0 ? 0 : pkNextGrid->iReel);

							pkNextGrid->iState = TMPS_OPEN;

							int iAngle = BSF_GetDirAngle(pkOriginGrid->x, pkOriginGrid->y, pkNextGrid->x, pkNextGrid->y, pkCurGrid->x, pkCurGrid->y);
							iAngle = (iAngle > 32 ? (64 - iAngle) : -iAngle);
							pkNextGrid->iAngle = pkCurGrid->iAngle + iAngle;

							if (pkNextGrid->iAngle >= 64)
							{
								pkNextGrid->iAngle -= 64;
								pkNextGrid->iReel -= 4;
							}

							if (pkNextGrid->iReel > 0)
							{
								pkNextGrid->iTranch = TMT_LEFT;
								pkNextGrid->iDir = iNextDir;
							}
							else
							{
								pkNextGrid->iTranch = TMT_NONE;
								pkNextGrid->iDir = -1;
								pkNextGrid->iAngle = 0;
								pkCurGrid->bChange = true;
							}

							if (pkNextGrid->iDir != pkCurGrid->iDir)
							{
								pkCurGrid->bChange = true;
							}

							akTackList.push_back(pkNextGrid);
							break;
						}
					}
				}
				break;
				case TMT_RIGHT:
				{
					for (iDir = 0; iDir < MAX_MAP_DIRECTION; ++iDir)
					{
						if (g_aiTranchAround[TMT_RIGHT][iDir] == pkCurGrid->iDir)
						{
							break;
						}
					}

					bool bTreak = false;

					iDir += 2 + 1;
					iDir = (iDir >= MAX_MAP_DIRECTION ? iDir - MAX_MAP_DIRECTION : iDir);
					for (iCount = 0; iCount < MAX_MAP_DIRECTION; ++iCount, ++iDir)
					{
						iDir = (iDir >= MAX_MAP_DIRECTION ? iDir - MAX_MAP_DIRECTION : iDir);
						int iNextDir = g_aiTranchAround[TMT_RIGHT][iDir];

						iNextX = pkCurGrid->x + g_akTagAround[iNextDir].x;
						iNextY = pkCurGrid->y + g_akTagAround[iNextDir].y;

						if (iNextY < 0 || iNextY >= m_iHeight || iNextX < 0 || iNextX >= m_iWidth)
						{
							break;
						}

						pkNextGrid = &m_akTGrid[iNextY * m_iWidth + iNextX];
						if (!pkNextGrid)
						{
							continue;
						}

						ChangeListPush(pkNextGrid->iIndex);

						if (pkNextGrid->iState != TMPS_TLOCK)
						{
							if (pkNextGrid->iState == TMPS_TARGET)
							{
								pkNextGrid->pkPreGrid = pkCurGrid;
								pkNextGrid->iStep = pkCurGrid->iStep + 1;

								CreateTStarPath(pkOriginGrid, pkTargetGrid);

								return TFR_ARRIVE;
							}

							switch (pkNextGrid->iState)
							{
							case TMPS_NONE:
							{
								pkNextGrid->pkPreGrid = pkCurGrid;
								pkNextGrid->iStep = pkCurGrid->iStep + 1;
							}
							break;
							case TMPS_CLOSE:
							{
								if (pkNextGrid->iTranch == TMT_LEFT && pkNextGrid == pkCurGrid->pkPreGrid && pkNextGrid->iDir != iNextDir)
								{
									bTreak = true;
									break;
								}
								else if (pkNextGrid->iTranch == TMT_RIGHT && pkNextGrid->iDir == iNextDir)
								{
									bTreak = true;
									break;
								}
								else if (pkNextGrid->iTranch == TMT_NONE && pkNextGrid->iStep > pkCurGrid->iStep)
								{
									bTreak = true;
									break;
								}
								else if (pkNextGrid->iStep > pkCurGrid->iStep + 1)
								{
									pkNextGrid->pkPreGrid = pkCurGrid;
									pkNextGrid->iStep = pkCurGrid->iStep + 1;
								}
							}
							break;
							case TMPS_OPEN:
							{
								if (pkNextGrid->iTranch == TMT_RIGHT && pkNextGrid->iDir == iNextDir)
								{
									bTreak = true;
									break;
								}
								else if (pkNextGrid->iStep > pkCurGrid->iStep + 1)
								{
									pkNextGrid->pkPreGrid = pkCurGrid;
									pkNextGrid->iStep = pkCurGrid->iStep + 1;
								}
							}
							break;
							default:
								break;
							}

							if (bTreak)
							{
								break;
							}

							int iAddReel = iCount - 1;
							pkNextGrid->iReel = pkCurGrid->iReel + iAddReel;
							pkNextGrid->iReel = (pkNextGrid->iReel < 0 ? 0 : pkNextGrid->iReel);

							pkNextGrid->iState = TMPS_OPEN;

							int iAngle = BSF_GetDirAngle(pkOriginGrid->x, pkOriginGrid->y, pkNextGrid->x, pkNextGrid->y, pkCurGrid->x, pkCurGrid->y);
							iAngle = (iAngle > 32 ? (64 - iAngle) : iAngle);
							pkNextGrid->iAngle = pkCurGrid->iAngle + iAngle;

							if (pkNextGrid->iAngle >= 64)
							{
								pkNextGrid->iAngle -= 64;
								pkNextGrid->iReel -= 4;
							}

							if (pkNextGrid->iReel > 0)
							{
								pkNextGrid->iTranch = TMT_RIGHT;
								pkNextGrid->iDir = iNextDir;
							}
							else
							{
								pkNextGrid->iTranch = TMT_NONE;
								pkNextGrid->iDir = -1;
								pkNextGrid->iAngle = 0;
								pkCurGrid->bChange = true;
							}

							if (pkNextGrid->iDir != pkCurGrid->iDir)
							{
								pkCurGrid->bChange = true;
							}

							akTackList.push_back(pkNextGrid);
							break;
						}
					}
				}
				break;
				default:
					break;
				}
			}

			if (pkCurGrid->iState != TMPS_ORIGIN)
			{
				pkCurGrid->iState = TMPS_CLOSE;
			}
		}

		akOpenList.clear();
		akOpenList.splice(akOpenList.end(), akTackList);
	}

	ResetTStarChangeList();

	m_iPathPointCount = 0;

	return TFR_NONE;
}

void TePathFinder::CreateTStarPath(TeTMapGrid* pkOriginGrid, TeTMapGrid* pkTargetGrid)
{
	if (!pkOriginGrid || !pkTargetGrid)
	{
		return;
	}

	if (!pkTargetGrid)
	{
		ResetTStarChangeList();

		return;
	}

	m_iPathCount = 0;
	TeTMapGrid* pkGrid = pkTargetGrid;
	while (true)
	{
		if (!pkGrid || m_iPathCount >= PATH_CHANGE_LIST)
		{
			break;
		}

		if (pkGrid->iState == TMPS_ORIGIN)
		{
			m_piPath[m_iPathCount++] = pkGrid->iIndex;
			if (m_iPathCount > 1)
			{
				m_akTGrid[m_piPath[m_iPathCount - 2]].pkPreGrid = pkGrid;
			}

			break;
		}

		if (!pkGrid->bChange)
		{
			pkGrid = pkGrid->pkPreGrid;
			continue;
		}

		if (m_iPathCount >= 2)
		{
			TeTMapGrid& rkCurGrid = m_akTGrid[m_piPath[m_iPathCount - 1]];
			TeTMapGrid& rkPreGrid = m_akTGrid[m_piPath[m_iPathCount - 2]];

			if ((pkGrid->x - rkCurGrid.x) * (rkCurGrid.y - rkPreGrid.y) == (rkCurGrid.x - rkPreGrid.x) * (pkGrid->y - rkCurGrid.y))
			{
				m_piPath[m_iPathCount - 1] = pkGrid->iIndex;
				m_akTGrid[m_piPath[m_iPathCount - 2]].pkPreGrid = pkGrid;
				pkGrid = pkGrid->pkPreGrid;

				continue;
			}
		}

		m_piPath[m_iPathCount++] = pkGrid->iIndex;
		if (m_iPathCount > 1)
		{
			m_akTGrid[m_piPath[m_iPathCount - 2]].pkPreGrid = pkGrid;
		}
		pkGrid = pkGrid->pkPreGrid;
	}

	m_iPathCount = 0;
	pkGrid = pkTargetGrid;
	while (true)
	{
		if (!pkGrid || m_iPathCount >= PATH_CHANGE_LIST)
		{
			break;
		}

		if (pkGrid->iState == TMPS_ORIGIN)
		{
			m_piPath[m_iPathCount++] = pkGrid->iIndex;
			break;
		}

		if (m_iPathCount >= 2)
		{
			TeTMapGrid& rkCurGrid = m_akTGrid[m_piPath[m_iPathCount - 1]];
			TeTMapGrid& rkPreGrid = m_akTGrid[m_piPath[m_iPathCount - 2]];

			int iDiff = (pkGrid->x - rkCurGrid.x) * (rkCurGrid.y - rkPreGrid.y) - (rkCurGrid.x - rkPreGrid.x) * (pkGrid->y - rkCurGrid.y);
			if (iDiff <= 10 && iDiff >= -10)
			{
				m_piPath[m_iPathCount - 1] = pkGrid->iIndex;
				m_akTGrid[m_piPath[m_iPathCount - 2]].pkPreGrid = pkGrid;
				pkGrid = pkGrid->pkPreGrid;

				continue;
			}
		}

		m_piPath[m_iPathCount++] = pkGrid->iIndex;
		if (m_iPathCount > 1)
		{
			m_akTGrid[m_piPath[m_iPathCount - 2]].pkPreGrid = pkGrid;
		}
		pkGrid = pkGrid->pkPreGrid;
	}

	m_iPathCount = 0;
	FilterTStarDirectPoint(pkOriginGrid, pkTargetGrid);

	m_iPathPointCount = m_iPathCount - 1;
	m_iPathPointCount = (m_iPathPointCount > PATH_RESULT ? PATH_RESULT : m_iPathPointCount);

	for (int i = 0; i < m_iPathPointCount; ++i)
	{
		TeTMapGrid& rkGrid = m_akTGrid[m_piPath[i + 1]];
		m_piPathPoint[i] = TePos2(Grid2Map(rkGrid.x), Grid2Map(rkGrid.y));
	}

	ResetTStarChangeList();
}

void TePathFinder::FilterTStarDirectPoint(TeTMapGrid* pkStart, TeTMapGrid* pkEnd)
{
	if (!pkStart || !pkEnd)
	{
		return;
	}

	m_piPath[m_iPathCount++] = pkStart->iIndex;

	if (pkStart == pkEnd)
	{
		return;
	}

	bool bDirect = false;
	TeTMapGrid* pkLast = pkEnd;
	TeTMapGrid* pkNextStart = NULL;
	while (pkLast && pkLast != pkStart)
	{
		bDirect = IsPointDirect(Grid2Map(pkStart->x), Grid2Map(pkStart->y), 1, Grid2Map(pkLast->x), Grid2Map(pkLast->y), 0, 0, m_iTStarObs);
		if (bDirect)
		{
			return FilterTStarDirectPoint(pkLast, pkEnd);
		}

		pkNextStart = pkLast;
		pkLast = pkLast->pkPreGrid;
	}

	return FilterTStarDirectPoint(pkNextStart, pkEnd);
}

void TePathFinder::ResetTStarChangeList(void)
{
	for (int i = 0; i < m_iChangeCount; ++i)
	{
		int iIdx = m_piChange[i];
		if (iIdx < 0 || iIdx >= m_iMaxIdx)
		{
			continue;
		}
		TeTMapGrid& rkGrid = m_akTGrid[iIdx];

		rkGrid.iState = (rkGrid.iState != TMPS_TLOCK ? TMPS_NONE : TMPS_TLOCK);
		rkGrid.iTranch = 0;
		rkGrid.iDir = -1;
		rkGrid.iReel = 0;
		rkGrid.iStep = 0;
		rkGrid.iAngle = 0;
		rkGrid.bChange = false;
		rkGrid.pkPreGrid = NULL;
	}
	m_iChangeCount = 0;
}

int g_aiGridOff[4][2] = { {0,1},{1,0},{0,-1},{-1,0}, };

TeTMapGrid* TePathFinder::CorrectOriginGrid(TeTMapGrid* pkOrigin)
{
	if (!pkOrigin || pkOrigin->iState != TMPS_TLOCK)
	{
		return pkOrigin;
	}

	int iGridIndex = 0;
	for (int iDis = 1; iDis <= 5; iDis++)
	{
		for (int iDir = 0; iDir < 4; iDir++)
		{
			iGridIndex = GridIndex(pkOrigin->x + g_aiGridOff[iDir][0] * iDis, pkOrigin->y + g_aiGridOff[iDir][1] * iDis);
			TeTMapGrid* pkGrid = &m_akTGrid[iGridIndex];

			if (pkGrid && pkGrid->iState != TMPS_TLOCK)
			{
				pkGrid->pkPreGrid = pkOrigin;
				pkGrid->bChange = true;
				return pkGrid;
			}
		}
	}

	return pkOrigin;
}