#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <string>

#define D3DX_PI 3.1415926f
#define	D3DX_2PI					(D3DX_PI * 2.0f)		// 2PI

float	GetBuyDistance(void);
float	GetDistance(float fx1, float fy1, float fx2, float fy2);
float	GetDistance2(float fx1, float fy1, float fx2, float fy2);
float	GetDistance3D(float fx1, float fy1, float fz1, float fx2, float fy2, float fz2);
bool	IsInTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int x, int y);
bool	IsInQuadrangle(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float x, float y);

float	AntiClockWiseDelta(float fRadian0, float fRadian1);
void	AdjustRadian(float& fRadian);
bool	IsAngleBetween(float fStart, float fEnd, float fAngle);
unsigned int	ReverseDWORD(unsigned int dwOrgin);
void	GetPosByDistance(float fDistance, float fPosX, float fPosY, float fTargetPosX, float fTargetPosY, float& fGetX, float& fGetY);

float   GetPointToLineDistance2(float fPosX, float fPosY, float fA, float fB, float fC);
float   GetPointToLineDistance(float fPosX, float fPosY, float fA, float fB, float fC);

enum BeUnitActionName
{
	BUAN_STAND = 0,		// stand
	BUAN_STAND1,		// stand-1
	BUAN_STAND2,		// stand-2
	BUAN_STAND3,		// stand-3
	BUAN_STAND4,		// stand-4
	BUAN_STAND5,		// stand-5
	BUAN_GSTAND,		// gstand
	BUAN_GSTAND1,		// gstand-1
	BUAN_GSTAND2,		// gstand-2
	BUAN_GSTAND3,		// gstand-3
	BUAN_GTSTAND,		// gtstand

	// brokenstand
	BUAN_BROKENSTAND,	// BrokenStand
	BUAN_BROKENSTAND1,	// BrokenStand-1
	BUAN_BROKENSTAND2,	// BrokenStand-2

	BUAN_WALK,			// walk
	BUAN_GWALK,			// gwalk
	BUAN_GWALK1,		// gwalk-1

	BUAN_ATTACK,		// attack
	BUAN_ATTACK1,		// attack-1
	BUAN_ATTACK2,		// attack-2
	BUAN_ATTACK3,		// attack-3
	BUAN_ATTACK4,		// attack-4
	BUAN_ATTACK5,		// attack-5
	BUAN_ATTACKSLAM,	// attack-slam
	BUAN_ATTACKSLAM1,	// attack-slam-1
	BUAN_GATTACK,		// gattack
	BUAN_GATTACK1,		// gattack-1
	BUAN_GATTACK2,		// gattack-2
	BUAN_GATTACKSLAM,	// gattack-slam
	BUAN_ATTACKAPPEAR,	// attack-appear

	BUAN_SPELL,			// spell
	BUAN_SPELL1,		// spell-1
	BUAN_SPELL2,		// spell-2
	BUAN_SPELL3,		// spell-3
	BUAN_SPELL4,		// spell-4
	BUAN_SPELL5,		// spell-5
	BUAN_SPELL6,		// spell-6
	BUAN_SPELL7,		// spell-7
	BUAN_SPELL8,		// spell-8
	BUAN_SPELL9,		// spell-9
	BUAN_GSPELL,		// gspell

	BUAN_TDANCE,		// tdance
	BUAN_TDANCE1,		// tdance-1

	BUAN_DEATH,			// death
	BUAN_GDEATH,		// gdeath
	BUAN_GDEATH1,		// gdeath-1

	BUAN_DISSIPATE,		// dissipate
	BUAN_GDISSIPATE,	// gdissipate

	BUAN_DECAY,			// decay
	BUAN_GDECAY,		// gdecay

	BUAN_BIRTH,			// birth

	BUAN_GTFLY,			// gtfly

	BUAN_IDLE,			// idle

	BUAN_NUM,
};

int					GetIndexByActionName(const char* pcName);
const char* GetActionNameByIndex(int iIndex);

int					GetIndexByAttachPosName(const char* pcName);
const char* GetAttachPosNameByIndex(int iIndex);

int TruncToInt(float fValue);

void LogRaceUpdateTime(const char* pcName, int iUpdateTime);

int		GetTypeID2Index(int iTypeID);
int		GetIndex2TypeID(int iIndex);
