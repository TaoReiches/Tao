#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

enum BePathFinderDef
{
	GRID_COORD_SIZE = 32,
	PATH_OPEN_LIST = 512 * 512,
	PATH_CHANGE_LIST = 5000,
	PATH_RESULT = 128,

	PATH_POOL_ELE_SIZE = PATH_CHANGE_LIST + PATH_OPEN_LIST + PATH_OPEN_LIST + 1024,
};

struct BeMapGrid
{
	unsigned short	iObstacle : 8;
	unsigned short	iState : 4;
	unsigned short	iParent : 4;
	inline bool IsObs(int iObs, bool bHasVision) const
	{
		iObs &= iObstacle;
		if (!bHasVision)
		{
			iObs &= ~(BGF_UNIT | BGF_TEMP | BGF_SKILL);
		}
		return iObs;
	}

	BeMapGrid()
	{
		iObstacle = 0;
		iState = 0;
		iParent = 0;
	}
};

struct BeBMapGrid
{
	unsigned short	iObstacle : 8;
	unsigned short	iState : 4;
	unsigned short	iBranch : 4;
	unsigned short	iDir : 4;
	unsigned short	iReel : 4;
	int		iIndex;
	int		x;
	int		y;
	int		iStep;
	int		iAngle;
	bool	bChange;
	BeBMapGrid* pkPreGrid;

	inline bool IsObs(int iObs, bool bHasVision) const
	{
		iObs &= iObstacle;
		if (!bHasVision)
		{
			iObs &= ~(BGF_UNIT | BGF_TEMP | BGF_SKILL);
		}
		return iObs;
	}

	BeBMapGrid()
	{
		iObstacle = 0;
		iState = 0;
		iBranch = 0;
		iDir = -1;
		iReel = 0;
		iIndex = 0;
		x = 0;
		y = 0;
		iStep = 0;
		iAngle = 0;
		bChange = false;
		pkPreGrid = NULL;
	}
};
typedef std::list<BeBMapGrid*> VecBMapGrid;

enum BeGridState
{
	BGS_NONE = 0,
	BGS_OBS,
	BGS_OPEN,
	BGS_CLOSE,
	BGS_ONLINE,
	BGS_NOOBS,
};

#define MAX_MAP_DIRECTION		4

enum BMapGridState
{
	BMPS_NONE = 0,
	BMPS_BLOCK,
	BMPS_ORIGIN,
	BMPS_TARGET,
	BMPS_CLOSE,
	BMPS_OPEN,
	BMPS_CHECK,
	BMPS_MAX,
};

enum BMapBranch
{
	BMB_NONE = 0,
	BMB_LEFT,
	BMB_RIGHT,
	BMB_MAX,
};

struct TagAround
{
	int x;
	int y;
};

struct BePathMem
{
	DECLARE_POOL(BePathMem);
	int m_aiData[PATH_POOL_ELE_SIZE];
};

class BePathFinder : public IPathFinder
{
	friend void InitServerPathGrids(int iW, int iH, unsigned short* akGrids);
public:
	BePathFinder();
	virtual ~BePathFinder();

	virtual	const BePos2* GetPathPoint() const;
	virtual	int				GetPathPointNum() const;

	virtual	void			InitGrids(int iW, int iH, unsigned short* akGrids, bool bUseBStar = false, int iBStarObs = BGF_TERRAIN | BGF_DOODAD);

	virtual	void			ClrObstacle(float fX, float fY, int iObstacle = BGF_UNIT, int iSize = 2);
	virtual	void			SetObstacle(float fX, float fY, int iObstacle = BGF_UNIT, int iSize = 2);

	virtual	BeFindResult	FindPathUnit(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize = 0, int iDistance = 0, int iObs = BGF_FIXED_OBS | BGF_UNIT | BGF_SKILL);
	virtual	int				GetFirstCanStay(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, float& fX, float& fY, float fDistance, int iObs = BGF_FIXED_OBS | BGF_UNIT | BGF_SKILL) const;
	virtual bool			GetNearestCanStay(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, float& fX, float& fY, float fDistance, int iObs = BGF_FIXED_OBS | BGF_UNIT | BGF_SKILL) const;

	virtual bool			ExecuteCommand(const char* strCmd);
	virtual bool			IsObstacle(float fX, float fY, int iObstacle = BGF_UNIT, int iSize = 2);
	virtual bool			UnitCanReach(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize = 0, int iDistance = 0, int iObs = BGF_FIXED_OBS | BGF_UNIT | BGF_SKILL);
	virtual	void			InitVision(int iW, int iH, unsigned short* pVision);

	virtual void			CopyGridsFromServerGrids();
public:
	inline bool CanStayPos(float fX, float fY, int iSize, int iObs) const;
	inline bool CanStayPos(int iX, int iY, int iSize, int iObs) const;
	inline bool CanStayGrid(int iGridX, int iGridY, int iSize, int iObs) const;
	inline bool CanStayGrid(int iIndex, int iSize, int iObs) const;

	inline bool CheckInput(float& fSrcX, float& fSrcY, float& fDstX, float& fDstY) const;

	BeFindResult	FindPathBStarGrid(int iSrcGridX, int iSrcGridY, int iSize, int iDstGridX, int iDstGridY);
	void			CreateBStarPath(BeBMapGrid* pkOriginPoint, BeBMapGrid* pkTargetPoint);
	void			ResetBStarChangeList(void);
	BeBMapGrid* CorrectOriginGrid(BeBMapGrid* pkOrigin);
	void			FilterBStarDirectPoint(BeBMapGrid* pkStart, BeBMapGrid* pkEnd);

	BeFindResult	FindPathGrid_Edge(int iSrcGridX, int iSrcGridY, int iSize, int iDstGridX, int iDstGridY, int iDistance, int iObs);

	BeFindResult	FindPathGrid_Edge_Safe(int iSrcGridX, int iSrcGridY, int iSize, int iDstGridX, int iDstGridY, int iDistance, int iObs, int iSrcParent = 0);

	void			MarkLine(int iSrcGridX, int iSrcGridY, int iSize, int iDstGridX, int iDstGridY, int iDistance, int iObs);

	void Release();

	void CreatePath(int iNearestIndex, int iStart);

	void SmoothPath(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize, int iDistance, int iObs, BeFindResult eRet);
	bool IsPointDirect(float fSrcX, float fSrcY, int iSrcSize, float fDstX, float fDstY, int iDstSize, float fDistance, int iObs);

	inline int Map2Grid(int iMap) const
	{
		return iMap / GRID_COORD_SIZE;
	}
	inline float Grid2Map(int iGrid) const
	{
		return (float)(iGrid * GRID_COORD_SIZE + GRID_COORD_SIZE / 2);
	}
	inline int Map2Grid(float f) const
	{
		return Map2Grid((int)f);
	}
	inline int GridIndex(int iGridX, int iGridY) const
	{
		if (iGridX < 0)
		{
			iGridX = 0;
		}
		else if (iGridX >= m_iWidth)
		{
			iGridX = m_iWidth - 1;
		}

		if (iGridY < 0)
		{
			iGridY = 0;
		}
		else if (iGridY >= m_iHeight)
		{
			iGridY = m_iHeight - 1;
		}

		return iGridY * m_iWidth + iGridX;
	}

	inline bool HasVision(int iX, int iY) const
	{
		if (!m_akVision)
		{
			return true;
		}

		iX = iX >> 1;
		iY = iY >> 1;

		int iIndex = iY * m_iVisionWidth + iX;
		if (m_akVision[iIndex] > 0)
		{
			return true;
		}

		return false;
	}
protected:
	int			m_iWidth;
	int			m_iHeight;
	int			m_iMaxIdx;
	BeMapGrid* m_akGrid;
	BeBMapGrid* m_akBGrid;

	int* m_piOpen;
	int m_iB;
	int m_iE;

	int* m_piChange;
	int m_iChangeCount;
	int m_iChangeListLimit;

	int* m_piPath;
	int m_iPathCount;

	BePos2 m_piPathPoint[PATH_RESULT];
	int m_iPathPointCount;

	mutable BePos2 m_kSrcPos;
	mutable BePos2 m_kDstPos;
	mutable int	m_iBlockType;
	mutable int m_iBlockX;
	mutable int m_iBlockY;

	int			m_iVisionWidth;
	int			m_iVisionHeight;
	unsigned short* m_akVision;

	bool		m_bUseBStar;
	int			m_iBStarObs;
protected:
	inline void OpenListPushBack(int iIndex);
	inline int OpenListPopHead(void);
	inline int OpenListGetCount(void);
	inline void OpenListClear(void);
	inline void ChangeListPush(int iIndex);
	inline bool ChangeListFull(void);

	void CreateList(void);
	void ReleaseList(void);
};

inline int GetDistance2Path(int iSrcX, int iSrcY, int iDstX, int iDstY);
inline int GetDistancePath(int iSrcX, int iSrcY, int iDstX, int iDstY);
inline float GetDistance2Path(float fSrcX, float fSrcY, float fDstX, float fDstY);
inline float GetDistancePath(float fSrcX, float fSrcY, float fDstX, float fDstY);

inline float fnextafter(float f1, float f2)
{
	float fTemp = f2;
	while (f1 != f2)
	{
		fTemp = f2;
		f2 -= f1;
		f2 *= 0.5f;
		f2 += f1;
	}
	return fTemp;
}
