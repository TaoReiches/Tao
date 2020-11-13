#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_IRandom.h"

class TwRandNum : public ITwRandom
{
public:
	TwRandNum(void);
	~TwRandNum(void);

	void    Initialize(int iSeed = 0) override;
	int     GetSeed(void) override;
	int     GetCount(void) override;
	int     GetPoolNum(void) override;
	int     RandInt(void) override;
	int     RandInt(int iMin, int iMax) override;
	float   RandFloat(float fMin, float fMax) override;

protected:
	static const unsigned int RAND_int_MAX = 0x7FFFFFFF;
	static const unsigned int RAND_CONST = 0x000041A7;
	static const unsigned int RAND_Q = RAND_int_MAX / RAND_CONST;
	static const unsigned int RAND_R = RAND_int_MAX % RAND_CONST;

	int             m_iSeed;
	unsigned int    m_uiCount;
	unsigned int    m_uiPoolNum;
};
