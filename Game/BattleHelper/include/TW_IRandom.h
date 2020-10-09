#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

class ITeRandom
{
public:
	virtual		void		Initialize(int iSeed = 0) = 0;
	virtual		int			GetSeed(void) = 0;
	virtual		int			GetCount(void) = 0;
	virtual		int			GetPoolNum(void) = 0;
	virtual		int			RandInt(void) = 0;
	virtual		int			RandInt(int iMin, int iMax) = 0;
	virtual		float		RandFloat(float fMin, float fMax) = 0;
};

ITeRandom* NewRandom();
