/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Main.h"

void BeMain::AddPlayerGold(int iIdx, int iGold, bool bIsCalculate, bool bIsSellMoney)
{
	if (iIdx < 0 || iIdx >= MAX_ACTIVEPLAYERS)
	{
		return;
	}
	if (iGold > 0)
	{
		if (iGold > 100000000)
		{
			iGold = 100000000;
		}
		if (bIsCalculate)
		{
			m_akPlayer[iIdx].iCalCulateGold += iGold;
		}
	}
	m_akPlayer[iIdx].iGold += iGold;
	if (m_akPlayer[iIdx].iGold <= 0)
	{
		m_akPlayer[iIdx].iGold = 0;
	}
}

void BeMain::SetPlayerKillCount(int iIdx, int iKillCount)
{
	CHECK_IDX(iIdx);
	m_akPlayer[iIdx].iKillCount = iKillCount;
}

void BeMain::AddPlayerKillCount(int iIdx, int iKillCount)
{
	m_akPlayer[iIdx].iKillCount += iKillCount;

	if (m_akPlayer[iIdx].iLastKillHeroTime != 0 && m_uiGameTime - m_akPlayer[iIdx].iLastKillHeroTime < 15000)
	{
		if (++m_akPlayer[iIdx].iContinueKill > m_akPlayer[iIdx].iMaxContinueKill)
		{
			m_akPlayer[iIdx].iMaxContinueKill = m_akPlayer[iIdx].iContinueKill;
		}
	}
	else
	{
		m_akPlayer[iIdx].iContinueKill = 0;
	}
	m_akPlayer[iIdx].iLastKillHeroTime = m_uiGameTime;
}

const UnitTable* BeMain::GetResUnit(int iUnitID) const
{
	if (iUnitID == 0)
	{
		return NULL;
	}

	return UnitTableMgr::Get()->GetUnitTable(iUnitID);
}

SeRaceEndPlayerInfo BeMain::GetRaceEndInfo(int iIndex, int iPlayerNums)
{
	SeRaceEndPlayerInfo kInfo;
	kInfo.iSeatIndex = iIndex;
	kInfo.dwHeroTypeID = GetHeroTypeID(iIndex);
	kInfo.iKillHeroNums = GetPlayerKillCount(iIndex);
	kInfo.iPlayerID = GetPlayerID(iIndex);
	kInfo.iDeathNums = GetPlayerBeKilledCount(iIndex);
	kInfo.iAssistanceNums = GetHelpCount(iIndex);
	kInfo.iMoney = GetCalculateGold(iIndex);
	kInfo.iTownNum = GetTownNum(iIndex) + GetTownAssistNum(iIndex);
	kInfo.iImTownNum = GetImTownNum(iIndex);
	kInfo.iCampNum = GetCampNum(iIndex) + GetCampAssistNum(iIndex);
	kInfo.iImCampNum = GetImCampNum(iIndex);
	kInfo.iBuildingNum = GetBuildingNum(iIndex);
	kInfo.iImBuildingNum = GetImBuildingNum(iIndex);
	kInfo.iKillSoildiers = GetKillSoildiers(iIndex);
	kInfo.iImKillSoildiers = GetImKillSoildiers(iIndex);
	kInfo.iKillMonsterNum = GetKillMonsterNum(iIndex);
	kInfo.iKillBigBossNum = GetKillBigBossNum(iIndex);
	kInfo.iKillSmallBossNum = GetKillSmallBossNum(iIndex);
	kInfo.iComboMaxKills = GetEndComboMaxKills(iIndex);
	kInfo.iComboKills = GetEndComboKills(iIndex);
	if (kInfo.iComboMaxKills == 0)
	{
		if (kInfo.iComboKills > 0)
		{
			kInfo.iComboMaxKills = 1;
		}
	}

	kInfo.TotalBuildingDamage = GetTotalBuildingDamage(iIndex);
	kInfo.iSpyNums = GetSpyNums(iIndex);
	kInfo.bKillBase = (GetKillBase(iIndex) != 0);
	kInfo.dwZhiliao = (unsigned int)GetZhiLiao(iIndex);
	kInfo.dwDamageEndure = (unsigned int)GetTotalBeDamaged(iIndex);
	kInfo.iTotalDamage = GetTotalDamage(iIndex);
	kInfo.iTotalHeroDamage = GetTotalHeroDamage(iIndex);
	kInfo.iRaceEndFlag = (GetGameOver(iIndex)) | (GetMVPTitleFlag(iIndex));
	kInfo.iMVPValue = GetMVPValue(iIndex);
	kInfo.iFeatsValue = GetPlayerFeatsValue(iIndex);
	kInfo.iRaceTime = gTime / 1000;
	kInfo.dwWishID = m_pkDataMgr->GetSelectedFunAward(iIndex);
	BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iIndex), true);
	if (pkHero)
	{
		kInfo.iHeroLevel = pkHero->GetLevel();
		for (int iItemIndex = 0; iItemIndex < 6; iItemIndex++)
		{
			BeItem* pkItem = pkHero->GetItemByPos(iItemIndex);
			if (pkItem)
			{
				kInfo.iItemTypeID[iItemIndex] = pkItem->GetTypeID();
			}
		}
	}
	strcpy(kInfo.acName, GetPlayerName(iIndex));
	if (GetPlayerType(iIndex) == BPT_NULL)
	{
		kInfo.iSeatIndex = -1;
	}
	int iFightScore = 0;

	kInfo.iFightScore = iFightScore;

	return kInfo;
}

bool BeMain::RandFloatInRate(float fRate, int iRandID)
{
	if (fRate <= 0)
	{
		return false;
	}

	bool bResult = (m_pkRandNum->RandFloat(RANDINFO, 0.0f, 1.0f) <= fRate) ? true : false;
	return bResult;
}

BeLabelObj* BeMain::GetLabelObject(const EsHandle& rkHandle)
{
	switch (rkHandle.GetType())
	{
	case BTOT_UNIT:
		return (BeLabelObj*)(m_pkUnitMgr->GetUnitByID(rkHandle.GetID(), true));
	case 5:
		return (BeLabelObj*)(m_pkMapItemMgr->GetMapItemByID(rkHandle.GetID()));
	case BTOT_EFFECT:
		return (BeLabelObj*)(m_pkEffectMgr->GetEffectByID(rkHandle.GetID()));
	default:
		break;
	}

	return NULL;
}

void BeMain::SetPlayerInfo(int iIdx, int iID, int iHeroID, const char* acName)
{
	if (iIdx < 0 || iIdx >= MAX_PLAYERS)
	{
		return;
	}

	SetPlayerType(iIdx, BPT_PLAYER);
	SetPlayerID(iIdx, iID);
	m_akPlayer[iIdx].iHeroUnitID = iHeroID;
	if (acName)
	{
		SetPlayerName(iIdx, acName);
	}
}

void	BeMain::SetPlayerGroup(int iIdx, int eGroup, bool bSetPlayer)
{
	CHECK_IDX(iIdx);
	m_akPlayer[iIdx].iCamp = eGroup;
	if (bSetPlayer)
	{
		UnitGroup kGroup;
		m_pkUnitMgr->GetUnitGroupByControl(kGroup, iIdx, true, false);
		for (int i = 0; i < (int)kGroup.Size(); i++)
		{
			BeUnit* pkUnit = kGroup[i];
			if (pkUnit)
			{
				pkUnit->SetPlayer(iIdx);
			}
		}
	}
}

void BeMain::SetPlayerGold(int iIdx, int iGold)
{
	if (iGold <= 0)
	{
		iGold = 0;
	}
	else if (iGold > 100000000)
	{
		iGold = 100000000;
	}
	CHECK_IDX(iIdx);
	m_akPlayer[iIdx].iGold = iGold;
}

void BeMain::AddRevengePlayer(int iIdx, int iPlayer)
{
	CHECK_IDX(iIdx);

	for (int i = 0; i < MAX_PLAYERS; ++i)
	{
		if (m_akPlayer[iIdx].akRevengeInfo[i].iKillPlayer == iPlayer)
		{
			m_akPlayer[iIdx].akRevengeInfo[i].iKillNums += 1;
			return;
		}
	}
	for (int i = 0; i < MAX_PLAYERS; ++i)
	{
		if (m_akPlayer[iIdx].akRevengeInfo[i].iKillPlayer == -1)
		{
			m_akPlayer[iIdx].akRevengeInfo[i].iKillPlayer = iPlayer;
			m_akPlayer[iIdx].akRevengeInfo[i].iKillNums = 1;
			return;
		}
	}
}

void BeMain::DelRevengePlayer(int iIdx, int iPlayer)
{
	CHECK_IDX(iIdx);
	for (int i = 0; i < MAX_PLAYERS; ++i)
	{
		if (m_akPlayer[iIdx].akRevengeInfo[i].iKillPlayer == iPlayer)
		{
			m_akPlayer[iIdx].akRevengeInfo[i].Clear();
			return;
		}
	}
}

bool BeMain::SetMissionID(int iIdx, int iMissionID)
{
	return false;
}

int BeMain::GetTotalMissionCompleteNums(int iIdx)
{
	CHECK_IDX(iIdx);
	int iTotalNums = 0;

	return iTotalNums;
}
