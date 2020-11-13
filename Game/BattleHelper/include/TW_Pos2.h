#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <math.h>

class TwPos2
{
public:
	TwPos2(float x = 0.0f, float y = 0.0f) : fX(x), fY(y)
	{
	}

	bool operator == (const TwPos2& kPos) const
	{
		return (kPos.fX == fX && kPos.fY == fY);
	}

	TwPos2& operator += (const TwPos2& kPos)
	{
		fX += kPos.fX;
		fY += kPos.fY;
		return *this;
	}

	TwPos2 operator + (const TwPos2& kPos) const
	{
		TwPos2 kTemp = *this;
		kTemp += kPos;
		return kTemp;
	}

	TwPos2& operator -= (const TwPos2& kPos)
	{
		fX -= kPos.fX;
		fY -= kPos.fY;
		return *this;
	}

	TwPos2 operator - (const TwPos2& kPos) const
	{
		TwPos2 kTemp = *this;
		kTemp -= kPos;
		return kTemp;
	}

	TwPos2& Rotate(const TwPos2& kPos, float fRadian)
	{
		float fSinA = sinf(fRadian);
		float fCosA = cosf(fRadian);
		TwPos2 kTemp = *this - kPos;
		TwPos2 kTempNew;
		kTempNew.fX = kTemp.fX * fCosA + kTemp.fY * fSinA;
		kTempNew.fY = -kTemp.fX * fSinA + kTemp.fY * fCosA;
		*this = kPos + kTempNew;
		return *this;
	}

	float	fX;
	float	fY;
};
