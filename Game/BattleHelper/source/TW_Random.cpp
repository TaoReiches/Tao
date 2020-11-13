/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Random.h"

#define TW_DEFAULT_SEED		6869

TwRandNum::TwRandNum(void)
{
	m_iSeed = TW_DEFAULT_SEED;
	m_uiCount = 0;
	m_uiPoolNum = 0;
}

TwRandNum::~TwRandNum(void)
{
}

ITwRandom* NewRandom()
{
	return new TwRandNum;
}

void TwRandNum::Initialize(int iSeed)
{
	if (iSeed == 0)
	{
		m_iSeed = TW_DEFAULT_SEED;
	}
	else
	{
		m_iSeed = iSeed;
	}

	m_uiPoolNum = m_iSeed;
	m_uiCount = 0;
}

int TwRandNum::GetSeed(void)
{
	return m_iSeed;
}

int TwRandNum::GetCount(void)
{
	return m_uiCount;
}

int TwRandNum::GetPoolNum(void)
{
	return m_uiPoolNum;
}

int TwRandNum::RandInt(void)
{
	unsigned int uiHigh = m_uiPoolNum / RAND_Q;
	unsigned int uiLow = m_uiPoolNum - (uiHigh * RAND_Q);

	m_uiPoolNum = (RAND_CONST * uiLow) - (RAND_R * uiHigh);
	if (m_uiPoolNum == 0)
	{
		m_uiPoolNum = TW_DEFAULT_SEED;
	}

	int iRand = int(m_uiPoolNum);
	if (iRand < 0)
	{
		iRand = -iRand;
	}

	m_uiCount++;
	return iRand;
}

int TwRandNum::RandInt(int iMin, int iMax)
{
	if (iMax <= iMin)
	{
		return iMin;
	}

	return iMin + RandInt() % (iMax - iMin);
}

float TwRandNum::RandFloat(float fMin, float fMax)
{
	if (fMax <= fMin)
	{
		return fMin;
	}

	return fMin + (fMax - fMin) * ((float)(RandInt()) / (float)(RAND_int_MAX + 1));
}
