#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Unit.h"
#include "TW_Buff.h"
#include "TW_UserDataDefine.h"

struct UnitEnmityPoint_MoreThan
{
	bool operator()(const std::shared_ptr<TwUnit> pkLeft, const std::shared_ptr<TwUnit> pkRight)
	{
		int iLeft = pkLeft->GetUD_Int(UserDataKey::UDK_MonsterAI);
		int iRight = pkRight->GetUD_Int(UserDataKey::UDK_MonsterAI);
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
	std::shared_ptr<TwUnit> m_pkUnit;
	Unit2UnitDistance_LessThan(std::shared_ptr<TwUnit> pkUnit)
	{
		m_pkUnit = pkUnit;
	}

	bool operator()(const std::shared_ptr<TwUnit> pkLeft, const std::shared_ptr<TwUnit> pkRight)
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
	int operator()(const std::shared_ptr<TwUnit> pkUnit, const std::shared_ptr<TwUnit> pkTarget, int iBufferID)
	{
		int iScore = 0;
		if (pkUnit == pkTarget)
		{
			iScore = 10000;
		}
		else
		{
			iScore = 2000;
		}

		//const BeBuffer* pkBuffer = pkUnit->GetBuffer(iBufferID);
		//if (pkBuffer)
		//{
		//	void* pkAttachMain = pkUnit->pkAttachMain;
		//	int iRemainTime = pkBuffer->GetRemoveTime() - gTime;
		//	iScore += iRemainTime / (-20);
		//}
		//else
		//{
		//	iScore += 2000;
		//}
		return iScore;
	}
};

struct UnitBufferScore_MoreThan
{
	bool operator()(const std::shared_ptr<TwUnit> pkLeft, const std::shared_ptr<TwUnit> pkRight)
	{
		int iLeft = UnitBufferScore()(pkLeft, pkTarget, iBufferID);
		int iRight = UnitBufferScore()(pkRight, pkTarget, iBufferID);
		return (iLeft > iRight);
	}

	UnitBufferScore_MoreThan(std::shared_ptr<TwUnit> pkTar, int iID) : pkTarget(pkTar), iBufferID(iID)
	{
	}
	std::shared_ptr<TwUnit> pkTarget;
	int iBufferID;
};

struct UnitHPPer_LessThan
{
	bool operator()(const std::shared_ptr<TwUnit> pkLeft, const std::shared_ptr<TwUnit> pkRight)
	{
		float fHPPercent1 = pkLeft->GetHP() / pkLeft->GetMaxHP();
		float fHPPercent2 = pkRight->GetHP() / pkRight->GetMaxHP();
		return (fHPPercent1 < fHPPercent2);
	}
};

struct UnitHPAbs_LessThan
{
	bool operator()(const std::shared_ptr<TwUnit> pkLeft, const std::shared_ptr<TwUnit> pkRight)
	{
		return pkLeft->GetHP() < pkRight->GetHP();
	}
};
