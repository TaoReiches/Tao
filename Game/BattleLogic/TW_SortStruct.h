#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

struct UnitEnmityPoint_MoreThan
{
	bool operator()(const BeUnit* pkLeft, const BeUnit* pkRight)
	{
		int iLeft = pkLeft->GetUD_Int(UDK_UintTempEnmity);
		int iRight = pkRight->GetUD_Int(UDK_UintTempEnmity);
		return (iLeft > iRight);
	}
};

struct MapItem2Pos_LessThan
{
	float fPosX;
	float fPosY;
	MapItem2Pos_LessThan(float fX, float fY) : fPosX(fX), fPosY(fY)
	{
	}

	bool operator()(const BeMapItem* pkLeft, const BeMapItem* pkRight)
	{
		float fDist = GetDistance2(fPosX, fPosY, pkLeft->GetPosX(), pkLeft->GetPosY());
		float fDist1 = GetDistance2(fPosX, fPosY, pkRight->GetPosX(), pkRight->GetPosY());
		return (fDist < fDist1);
	}
};

struct Unit2UnitDistance_LessThan
{
	BeUnit* m_pkUnit;
	Unit2UnitDistance_LessThan(BeUnit* pkUnit)
	{
		m_pkUnit = pkUnit;
	}

	bool operator()(const BeUnit* pkLeft, const BeUnit* pkRight)
	{
		if (!pkLeft || !pkRight || !m_pkUnit)
		{
			return false;
		}
		float fDist1 = GetDistance2(pkLeft->GetPosX(), pkLeft->GetPosY(), m_pkUnit->GetPosX(), m_pkUnit->GetPosY());
		float fDist2 = GetDistance2(pkRight->GetPosX(), pkRight->GetPosY(), m_pkUnit->GetPosX(), m_pkUnit->GetPosY());
		return (fDist1 < fDist2);
	}
};

struct UnitBufferScore
{
	int operator()(const BeUnit* pkUnit, const BeUnit* pkTarget, int iBufferID)
	{
		int iScore = 0;
		if (pkUnit == pkTarget)
		{
			iScore = 10000;
		}
		else if (pkUnit->IsHero())
		{
			iScore = 6000;
		}
		else
		{
			iScore = 2000;
		}

		const BeBuffer* pkBuffer = pkUnit->GetBuffer(iBufferID);
		if (pkBuffer)
		{
			void* pkAttachMain = pkUnit->pkAttachMain;
			int iRemainTime = pkBuffer->GetRemoveTime() - gTime;
			iScore += iRemainTime / (-20);
		}
		else
		{
			iScore += 2000;
		}
		return iScore;
	}
};

struct UnitBufferScore_MoreThan
{
	bool operator()(const BeUnit* pkLeft, const BeUnit* pkRight)
	{
		int iLeft = UnitBufferScore()(pkLeft, pkTarget, iBufferID);
		int iRight = UnitBufferScore()(pkRight, pkTarget, iBufferID);
		return (iLeft > iRight);
	}

	UnitBufferScore_MoreThan(BeUnit* pkTar, int iID) : pkTarget(pkTar), iBufferID(iID)
	{
	}
	BeUnit* pkTarget;
	int iBufferID;
};

struct UnitHPPer_LessThan
{
	bool operator()(const BeUnit* pkLeft, const BeUnit* pkRight)
	{
		float fHPPercent1 = pkLeft->GetHP() / pkLeft->GetMaxHP();
		float fHPPercent2 = pkRight->GetHP() / pkRight->GetMaxHP();
		return (fHPPercent1 < fHPPercent2);
	}
};

struct UnitHPAbs_LessThan
{
	bool operator()(const BeUnit* pkLeft, const BeUnit* pkRight)
	{
		return pkLeft->GetHP() < pkRight->GetHP();
	}
};
