#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

extern int	g_aiBStarSin[64];
extern int	g_aiBStarCos[64];

int	BSF_InternalDirSinCos(int pSinCosTable[], int nDir, int nMaxDir);
int BSF_DirSin(int nDir, int nMaxDir);
int BSF_DirCos(int nDir, int nMaxDir);
int	BSF_GetDistance(int nX1, int nY1, int nX2, int nY2);
int	BSF_GetDirIndex(int nX1, int nY1, int nX2, int nY2);
int BSF_GetDirDiff(int nOriginX, int nOriginY, int nTargetX, int nTargetY, int nX, int nY);
int BSF_GetDirAngle(int nOriginX, int nOriginY, int nTargetX, int nTargetY, int nX, int nY);
int BSF_Dir64To8(int nDir);
int BSF_Dir8To64(int nDir);
