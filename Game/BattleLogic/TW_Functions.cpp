/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Functions.h"

typedef struct POINT
{
	float x;
	float y;
	float z;
} POINT;

float GetBuyDistance(void)
{
	return (1500.0f * 1500.0f);
}

float GetDistance(float fx1, float fy1, float fx2, float fy2)
{
	float fDeltaX = fx1 - fx2;
	float fDeltaY = fy1 - fy2;
	return sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY);
}

float GetDistance2(float fx1, float fy1, float fx2, float fy2)
{
	float fDeltaX = fx1 - fx2;
	float fDeltaY = fy1 - fy2;
	return (fDeltaX * fDeltaX + fDeltaY * fDeltaY);
}

float GetDistance3D(float fx1, float fy1, float fz1, float fx2, float fy2, float fz2)
{
	float fDeltaX = fx1 - fx2;
	float fDeltaY = fy1 - fy2;
	float fDeltaZ = fz1 - fz2;
	return sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY + fDeltaZ * fDeltaZ);
}

bool IsInTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int x, int y)
{
	POINT akDir[3];

	akDir[0].x = x0 - x;
	akDir[0].y = y0 - y;
	akDir[1].x = x1 - x;
	akDir[1].y = y1 - y;
	akDir[2].x = x2 - x;
	akDir[2].y = y2 - y;

	for (int i = 0; i < 3; i++)
	{
		int j = (i + 1) % 3;
		if ((akDir[i].x * akDir[j].y - akDir[i].y * akDir[j].x) == 0)
		{
			if (akDir[i].x * akDir[j].x > 0 || akDir[i].y * akDir[j].y > 0)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}

	int iT1, iT2;

	iT1 = akDir[0].y * akDir[2].x - akDir[2].y * akDir[0].x;
	iT2 = akDir[0].y * akDir[1].x - akDir[1].y * akDir[0].x;
	if ((iT1 > 0) + (iT2 > 0) != 1)
	{
		return false;
	}

	iT1 = akDir[1].y * akDir[0].x - akDir[0].y * akDir[1].x;
	iT2 = akDir[1].y * akDir[2].x - akDir[2].y * akDir[1].x;
	if ((iT1 > 0) + (iT2 > 0) != 1)
	{
		return false;
	}

	return true;
}

bool IsInQuadrangle(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)
{
	if (IsInTriangle(x0, y0, x1, y1, x3, y3, x, y))
	{
		return true;
	}
	else if (IsInTriangle(x1, y1, x2, y2, x3, y3, x, y))
	{
		return true;
	}
	return false;
}

float AntiClockWiseDelta(float fRadian0, float fRadian1)
{
	float fDeltaRadian = fRadian1 - fRadian0;

	AdjustRadian(fDeltaRadian);

	return fDeltaRadian;
}

void AdjustRadian(float& fRadian)
{
	int iCount = 0;
	while (fRadian < -D3DX_PI)
	{
		fRadian += D3DX_2PI;
		iCount++;
		if (iCount > 10000)
		{
			fRadian = 0.0f;
			return;
		}
	}

	iCount = 0;
	while (fRadian >= D3DX_PI)
	{
		fRadian -= D3DX_2PI;
		iCount++;
		if (iCount > 10000)
		{
			fRadian = 0.0f;
			return;
		}
	}
}

bool IsAngleBetween(float fStart, float fEnd, float fAngle)
{
	while (fEnd < fStart)
	{
		fEnd += D3DX_2PI;
	}

	while (fEnd > fStart + D3DX_2PI)
	{
		fEnd -= D3DX_2PI;
	}

	while (fAngle < fStart)
	{
		fAngle += D3DX_2PI;
	}

	while (fAngle > fStart + D3DX_2PI)
	{
		fAngle -= D3DX_2PI;
	}

	return (fAngle < fEnd);
}

unsigned int ReverseDWORD(unsigned int dwOrgin)
{
	unsigned int dwNewDWord = 0;
	char* pcSource;
	char* pcTarget;

	pcSource = (char*)(&dwOrgin);
	pcTarget = (char*)(&dwNewDWord);

	pcTarget[0] = pcSource[3];
	pcTarget[1] = pcSource[2];
	pcTarget[2] = pcSource[1];
	pcTarget[3] = pcSource[0];

	return dwNewDWord;
}

void GetPosByDistance(float fDistance, float fPosX, float fPosY, float fTargetPosX, float fTargetPosY, float& fGetX, float& fGetY)
{
	float fDX = fTargetPosX - fPosX;
	float fDY = fTargetPosY - fPosY;
	float fDistance0 = sqrtf(fDX * fDX + fDY * fDY);
	if (fDistance0 <= 0.0f)
	{
		fGetX = fPosX;
		fGetY = fPosY;
		return;
	}

	float fScale = fDistance / fDistance0;
	fGetX = fPosX + fScale * fDX;
	fGetY = fPosY + fScale * fDY;
}

float GetPointToLineDistance2(float fPosX, float fPosY, float fA, float fB, float fC)
{
	float fAbs = abs(fA * fPosX + fB * fPosY + fC);
	float fDistance2 = fAbs * fAbs / (fA * fA + fB * fB);
	return fDistance2;
}

float GetPointToLineDistance(float fPosX, float fPosY, float fA, float fB, float fC)
{
	float fAbs = abs(fA * fPosX + fB * fPosY + fC);
	float fDistance = fAbs / (sqrtf(fA * fA + fB * fB));
	return fDistance;
}

const char* g_apcActionName[BUAN_NUM] = {
	"stand",
	"stand-1",
	"stand-2",
	"stand-3",
	"stand-4",
	"stand-5",
	"gstand",
	"gstand-1",
	"gstand-2",
	"gstand-3",
	"gtstand",

	"brokenstand",
	"brokenstand-1",
	"brokenstand-2",

	"walk",
	"gwalk",
	"gwalk-1",

	"attack",
	"attack-1",
	"attack-2",
	"attack-3",
	"attack-4",
	"attack-5",
	"attack-slam",
	"attack-slam-1",
	"gattack",
	"gattack-1",
	"gattack-2",
	"gattack-slam",
	"attack-appear",

	"spell",
	"spell-1",
	"spell-2",
	"spell-3",
	"spell-4",
	"spell-5",
	"spell-6",
	"spell-7",
	"spell-8",
	"spell-9",
	"gspell",

	"tdance",
	"tdance-1",

	"death",
	"gdeath",
	"gdeath-1",

	"dissipate",
	"gdissipate",

	"decay",
	"gdecay",

	"birth",

	"gtfly",

	"idle",
};

int GetIndexByActionName(const char* pcName)
{
	char acName[128] = { 0 };
	strcpy(acName, pcName);
#ifdef _WIN32
	strlwr(acName);
#else
	for (int i = 0; i < strlen(pcName); i++)
	{
		acName[i] = tolower(acName[i]);
	}
#endif
	if (acName == NULL)
	{
		return -1;
	}

	char acName1[128] = { 0 };
	int iLen = strlen(acName);
	int iStrLen = 0;
	for (int i = 0; i < iLen; i++)
	{
		if (acName[i] != ' ')
		{
			acName1[iStrLen++] = acName[i];
		}
	}
	acName1[iStrLen] = '\0';
	for (int i = 0; i < BUAN_NUM; i++)
	{
		if (strcmp(g_apcActionName[i], acName1) == 0)
		{
			return i;
		}
	}
	return -1;
}

int TruncToInt(float fValue)
{
	int iValue;
	if (fValue >= -.01f && fValue <= 0.1f)
	{
		return 0;
	}

	iValue = *(int*)&fValue;
	int mantissa = (iValue & 0x07fffff) | 0x800000;
	int exponent = 150 - ((iValue >> 23) & 0xff);
	if (exponent < 0)
	{
		iValue = (mantissa << -exponent);
	}
	else
	{
		iValue = (mantissa >> exponent);
	}

	if ((*(int*)&fValue) & 0x80000000)
	{
		iValue = -iValue;
	}

	return iValue;
}
