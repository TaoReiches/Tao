/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Main.h"

int BeMain::GetHeroTypeID(int iIdx)
{
	if (iIdx < 0 || iIdx >= MAX_PLAYERS)
	{
		return 0;
	}

	BeUnit* pkHero = gUnitMgr.GetUnitByID(m_akPlayer[iIdx].iHeroID);// m_akPlayer[iIdx].skHeroPtr.Get();
	if (pkHero)
	{
		return pkHero->GetTypeID();
	}
	return 0;
}

const char* BeMain::GetHeroName(int iIdx)
{
	if (iIdx < 0 || iIdx >= MAX_PLAYERS)
	{
		return NULL;
	}

	BeUnit* pkHero = gUnitMgr.GetUnitByID(m_akPlayer[iIdx].iHeroID); //m_akPlayer[iIdx].skHeroPtr.Get();
	if (pkHero)
	{
		//		return pkHero->GetName();
	}
	return NULL;
}

int BeMain::GetHeroLevel(int iIdx)
{
	if (iIdx < 0 || iIdx >= MAX_PLAYERS)
	{
		return 0;
	}

	BeUnit* pkHero = gUnitMgr.GetUnitByID(m_akPlayer[iIdx].iHeroID); //m_akPlayer[iIdx].skHeroPtr.Get();
	if (pkHero)
	{
		return pkHero->GetLevel();
	}
	return 0;
}

int BeMain::TimeGetHour(void) const
{
	if (m_bNoon)
	{
		return 12;
	}
	else if (m_bCheatNight)
	{
		return -1;
	}
	{
		return (m_uiGameTime / 37500 + 6) % 24;
	}
}

int BeMain::TimeGetMinute(void) const
{
	if (m_uiGameTime <= 90000)
	{
		return -1;
	}

	return ((m_uiGameTime - 90000) / 625) % 60;
}

int BeMain::TimeGetSecond(void) const
{
	if (m_uiGameTime <= 90000)
	{
		return -1;
	}

	return ((m_uiGameTime - 90000) / 10) % 60;
}

void BeMain::SetLoadingProgress(float fRate)
{
	m_fProgress = fRate;
}

float BeMain::GetLoadingProgress(void) const
{
	return m_fProgress;
}

void BeMain::SetMapID(int iMapID)
{
	m_iMapID = iMapID;
}

void BeMain::SetRaceID(const char* acRaceID)
{
	memcpy(m_acRaceID, acRaceID, sizeof(m_acRaceID));
}

void BeMain::SetRSBossCount(int iCount)
{
	m_iRSBossCount = iCount;
}

int BeMain::GetRSBossCount()
{
	return m_iRSBossCount;
}

void BeMain::PushExistRSGroup(int iIndex)
{
	m_aiVecExistRSGroupIndex.push_back(iIndex);
}

bool BeMain::IsExistRSGroup(int iIndex)
{
	bool bExist = false;
	for (int i = 0; i < (int)m_aiVecExistRSGroupIndex.size(); ++i)
	{
		if (iIndex == m_aiVecExistRSGroupIndex[i])
		{
			bExist = true;
			break;
		}
	}
	return bExist;
}

void BeMain::SetDropSkillTime(int iTime)
{
	m_iDropSkillTime = iTime;
}

int BeMain::GetDropSkillTime()
{
	return m_iDropSkillTime;
}
