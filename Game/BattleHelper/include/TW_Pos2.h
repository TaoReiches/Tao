#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

struct TePos2
{
	TePos2(float x = 0.0f, float y = 0.0f) : fX(x), fY(y)
	{
	}

	bool operator == (const TePos2& kPos) const
	{
		return (kPos.fX == fX && kPos.fY == fY);
	}

	TePos2& operator += (const TePos2& kPos)
	{
		fX += kPos.fX;
		fY += kPos.fY;
		return *this;
	}

	TePos2 operator + (const TePos2& kPos) const
	{
		TePos2 kTemp = *this;
		kTemp += kPos;
		return kTemp;
	}

	TePos2& operator -= (const TePos2& kPos)
	{
		fX -= kPos.fX;
		fY -= kPos.fY;
		return *this;
	}

	TePos2 operator - (const TePos2& kPos) const
	{
		TePos2 kTemp = *this;
		kTemp -= kPos;
		return kTemp;
	}

	TePos2& Rotate(const TePos2& kPos, float fRadian)
	{
		float fSinA = sinf(fRadian);
		float fCosA = cosf(fRadian);
		TePos2 kTemp = *this - kPos;
		TePos2 kTempNew;
		kTempNew.fX = kTemp.fX * fCosA + kTemp.fY * fSinA;
		kTempNew.fY = -kTemp.fX * fSinA + kTemp.fY * fCosA;
		*this = kPos + kTempNew;
		return *this;
	}

	float	fX;
	float	fY;
};
