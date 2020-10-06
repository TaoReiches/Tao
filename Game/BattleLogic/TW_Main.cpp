/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Main.h"

BeMain::BeMain(void)
{
	pkAttachMain = this;
	m_iMapID = 0;
	memset(m_acRaceID, 0, sizeof(m_acRaceID));
	m_iOwner = -1;
	m_kMapName = "";
	m_bIsMovieOn = false;
	m_bIsCameraRotate = false;
	m_bIsCameraChange = false;
	m_bNeedCameraReset = false;

	m_iFirstBloodPlayer = -1;
	m_iFirstDestoryTower = -1;
	m_pkRandNum = nullptr;
	m_pkMap = nullptr;
	m_pkDataMgr = nullptr;
	m_pkUnitMgr = nullptr;
	m_pkEffectMgr = nullptr;
	m_pkMapItemMgr = nullptr;
	m_pkTriggerMgr = nullptr;
	m_pkFormulaInfo = nullptr;
	m_fProgress = 0;
	m_pkRenJiMgr = nullptr;
	m_iGameBeginTime = 0;

	m_bOpenZuoBi = false;
	m_iSelfZuoBi = -1;
	m_bAllZuoBi = false;
	m_bNoon = false;
	m_bCheatNight = false;
	m_bAutoMatch = false;
	m_iGuoZhanCityFlag = 0;
	m_bCurMapFirstGame = false;

	m_pkTaskTriggerHandle = nullptr;

	for (int i = 0; i < SERVERMODE_MAXPLAYER; i++)
	{
		m_akPureDataCmdRecord[i].Clear();
	}

	m_iHackRushDiff = 0;
	m_iRSBossCount = 0;
	m_iDropSkillTime = 0;
	ResetData();

	m_e8V8State = BMS_8V8_NONE;

	m_iCurMapID = 0;
	m_pkCurMapInfo = nullptr;

	for (int i = 0; i < MAX_GROUPS; i++)
	{
		m_iCampKills[i] = 0;
	}

	for (int i = 0; i < MAX_GROUPS; i++)
	{
		m_iCampTeamScore[i] = 0;
	}
	m_iCampLastKiller[0] = 0;
	m_iCampLastKiller[1] = 0;
	m_iRedGuardFlag = 0;
	m_iBlueGuardFlag = 0;
	m_bSecretArea = false;
}

BeMain::~BeMain(void)
{
	Finialize();
	ReleaseModule();
}

int BeMain::GetSoldierPlayerByGroup(SeRoomPlayerCamp eCamp) const
{
	if (eCamp >= 0 && eCamp < SRPC_NUMS)
	{
		return m_aiGroupSoldier[eCamp];
	}
	return MAX_PLAYERS;
}

void BeMain::ResetData(void)
{
	memset(m_aiGroupSoldier, 0, sizeof(m_aiGroupSoldier));
	m_eState = BMS_NONE;
	m_uiGameTime = 0;
	m_iPreGiveCommandTimes = 0;

	m_uiFrameCount = 0;
	m_iGameTimeLeft = 0;
	m_uiRealTimeNow = 0;

	for (int i = 0; i < MAX_PLAYERS; ++i)
	{
		m_akPlayer[i].Clear();
	}
	m_bShowAllDamage = false;
	memset(m_aiGenID, 0, sizeof(m_aiGenID));
	memset(m_aiGroupComboKill, 0, sizeof(m_aiGroupComboKill));
	memset(m_abAllTowerDeadCamp, 0, sizeof(m_abAllTowerDeadCamp));
	memset(m_aiOtherCampAllDeadCount, 0, sizeof(m_aiOtherCampAllDeadCount));
	memset(m_iVersion, 0, sizeof(m_iVersion));
	memset(m_fZhanGongSoliderAdd, 0.0f, sizeof(m_fZhanGongSoliderAdd));

	m_iOverTime = 90;
	m_bGameValid = true;
	m_iDifficultyGrade = 0;

	m_bBaseHasDestroy = false;
	m_iMaxSidePlayer = 0;

	m_bZMap = false;
	m_bPVEMap = false;
	m_b8v8Map = false;

	m_akMapShareUnit.clear();

	m_kMChongWuKongZhiSwitch.clear();
	m_kMShareSwitch.clear();
	m_kTownItemStack.clear();

	m_kShareLeave.clear();

	BeLevelMain* pkTriggerMain = (BeLevelMain*)m_pkTaskTriggerHandle;
	if (pkTriggerMain)
	{
		pkTriggerMain->Reset();
	}

	m_bOver = false;

	memset(m_aiLockUnit, -1, sizeof(m_aiLockUnit));
	memset(m_aiLastLockUnit, -1, sizeof(m_aiLastLockUnit));
	memset(m_aiHeroDataChangeFlag, 0, sizeof(m_aiHeroDataChangeFlag));
	m_bRacePause = false;
	m_iChangeFlag = 0;
	m_iMaxPerLoadID = 0;
	m_bReLink = false;
	FinializeServerMode();
	m_aiVecHeroTypeID.clear();
	memset(m_aiRandomSkillTimes, 0, sizeof(m_aiRandomSkillTimes));
	memset(m_aiRandomSkillGroupScore, 0, sizeof(m_aiRandomSkillGroupScore));
	m_iRSBossCount = 0;
	m_iDropSkillTime = 0;
	m_aiVecExistRSGroupIndex.clear();

	m_bNewMapData = false;
	for (int i = 0; i < SERVERMODE_MAXPLAYER; i++)
	{
		m_akPureDataCmdRecord[i].Clear();
	}
	m_kViewData.Clear();
	memset(m_aiFocusHeroID, -1, sizeof(m_aiFocusHeroID));

	memset(m_akEntityPointer, 0, sizeof(m_akEntityPointer));

	for (int i = 0; i < MAX_ACTIVEPLAYERS; i++)
	{
		m_bSendPing[i] = false;
	}

	m_iCampMaxScore = 0;
}

void BeMain::ReleaseModule(void)
{
	SAFE_DELETE(m_pkMapItemMgr);
	SAFE_DELETE(m_pkEffectMgr);
	SAFE_DELETE(m_pkUnitMgr);
	SAFE_DELETE(m_pkMap);
	SAFE_DELETE(m_pkTriggerMgr);
	SAFE_DELETE(m_pkRandNum);
	SAFE_DELETE(m_pkFormulaInfo);
	SAFE_DELETE(m_pkDataMgr);
	SAFE_DELETE(m_pkRenJiMgr);

	if (m_pkTaskTriggerHandle)
	{
		BeLevelMain* pkTriggerMain = (BeLevelMain*)m_pkTaskTriggerHandle;
		delete pkTriggerMain;
		m_pkTaskTriggerHandle = nullptr;
	}
}

void BeMain::InitPlayerGroup()
{
	bool bMoreCamp = false;

	m_aiGroupSoldier[SRPC_INVALID] = MAX_PLAYERS;
	{
		m_aiGroupSoldier[SRPC_CAMPA] = MAX_ACTIVEPLAYERS;
		m_aiGroupSoldier[SRPC_CAMPB] = MAX_ACTIVEPLAYERS + 1;
		m_aiGroupSoldier[SRPC_CAMPC] = m_aiGroupSoldier[SRPC_INVALID];
		m_aiGroupSoldier[SRPC_CAMPD] = m_aiGroupSoldier[SRPC_INVALID];
		m_aiGroupSoldier[SRPC_NEUTRAL_PASSIVE] = MAX_ACTIVEPLAYERS + 2;
		m_aiGroupSoldier[SRPC_NEUTRAL_ENEMY] = MAX_ACTIVEPLAYERS + 3;
		m_aiGroupSoldier[SRPC_NEUTRAL_ALLY] = MAX_ACTIVEPLAYERS + 4;
		m_aiGroupSoldier[SRPC_OTHER_SIDE] = MAX_ACTIVEPLAYERS + 5;
	}
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (i < MAX_ACTIVEPLAYERS)
		{
		}
		else
			if (i == SIDEA_SOILDER)
			{
				SetPlayerGroup(i, SRPC_CAMPA);
			}
			else if (i == SIDEB_SOILDER)
			{
				SetPlayerGroup(i, SRPC_CAMPB);
			}
			else if (i == SIDEC_SOILDER)
			{
				SetPlayerGroup(i, SRPC_CAMPC);
			}
			else if (i == NEUTRAL_PASSIVE)
			{
				SetPlayerGroup(i, SRPC_NEUTRAL_PASSIVE);
			}
			else if (i == NEUTRAL_ENEMY)
			{
				SetPlayerGroup(i, SRPC_NEUTRAL_ENEMY);
			}
			else if (i == NEUTRAL_ALLY)
			{
				SetPlayerGroup(i, SRPC_NEUTRAL_ALLY);
			}
			else
			{
				SetPlayerGroup(i, SRPC_OTHER_SIDE);
			}
	}

}

bool BeMain::InitializeGame(void)
{
	InitCommonEventTrigger();
	InitUnitEventTrigger();

	switch (m_iMapID)
	{
	case 'M200':
	case 'M201':
	case 'M202':
	{
		SetPveMap(true);
		InitZhanChangEventTrigger();
		break;
	}
	case 'M300':
	case 'M301':
	case 'M302':
	case 'M303':
	case 'M304':
	{
		if (m_iMapID == 'M301') {
			gData.SetSecretAreaLayer(21);
		}
		else if (m_iMapID == 'M302') {
			gData.SetSecretAreaLayer(41);
		}
		else if (m_iMapID == 'M303') {
			gData.SetSecretAreaLayer(61);
		}
		else if (m_iMapID == 'M304') {
			gData.SetSecretAreaLayer(81);
		}
		InitSecretAreaEventTrigger();
		InitZhanChangEventTrigger();
		SetPveMap(true);
		SetSecretArea(true);
		break;
	}
	default:
		break;
	}

	if (!m_pkTriggerMgr || !m_pkDataMgr)
	{
		return false;
	}

	LoadLevelMainMapFile();
	return true;
}

void BeMain::LoadLevelMainMapFile(void)
{
	BeLevelMain* pkTriggerMain = (BeLevelMain*)m_pkTaskTriggerHandle;
	EsAssert(pkTriggerMain);
	if (pkTriggerMain)
	{
		const MapInfoTable* pkMapInfoRes = gMain.GetMapInfoRes(GetMapID());
		if (!pkMapInfoRes)
		{
			return;
		}

		std::string kLevelFile = pkMapInfoRes->kConfigName;
		pkTriggerMain->LoadTask(kLevelFile.c_str());

		BeTDEventArgs kArgs = BeTDEventArgs();
		kArgs.iEventType = BeTDEventArgs::BeTDEVENT_GAMESTART;
		pkTriggerMain->GetEventMgr()->FireEvent(kArgs);
	}
}

bool BeMain::Initialize(void)
{
	TeVision::FirstInitialize();

	m_eState = BMS_NONE;
	m_uiGameTime = 0;

	m_uiFrameCount = 0;
	m_iGameTimeLeft = 0;
	m_uiRealTimeNow = 0;
	m_bIsMovieOn = false;
	m_bIsCameraRotate = false;
	m_bIsCameraChange = false;
	m_bNeedCameraReset = false;

	memset(m_aiGroupComboKill, 0, sizeof(m_aiGroupComboKill));
	memset(m_abAllTowerDeadCamp, 0, sizeof(m_abAllTowerDeadCamp));
	memset(m_aiOtherCampAllDeadCount, 0, sizeof(m_aiOtherCampAllDeadCount));
	memset(m_aiRandomSkillTimes, 0, sizeof(m_aiRandomSkillTimes));
	memset(m_aiRandomSkillGroupScore, 0, sizeof(m_aiRandomSkillGroupScore));
	m_iRSBossCount = 0;
	m_iDropSkillTime = 0;
	m_aiVecExistRSGroupIndex.clear();

	if (!m_pkDataMgr || !m_pkDataMgr->Initialize())
	{
		return false;
	}

	if (!m_pkUnitMgr || !m_pkUnitMgr->Initialize())
	{
		return false;
	}

	if (!m_pkEffectMgr || !m_pkEffectMgr->Initialize())
	{
		return false;
	}

	if (!m_pkMapItemMgr || !m_pkMapItemMgr->Initialize())
	{
		return false;
	}

	if (!m_pkTriggerMgr->Initialize(BTE_MAX_NUM))
	{
		return false;
	}

	InitPlayerGroup();

	if (!m_pkRenJiMgr || !m_pkRenJiMgr->Initialize())
	{
		return false;
	}

	m_eState = BMS_INITED;
	m_bOpenZuoBi = false;
	m_iSelfZuoBi = -1;
	m_bAllZuoBi = false;
	m_bNoon = false;
	m_bCheatNight = false;
	m_eRaceGameModel = BRGM_NORMAL;

	m_aiVecHeroTypeID.clear();

	m_iLeftSurrenderBeginTime = 0;
	m_iRightSurrenderBeginTime = 0;

	return true;
}

void BeMain::Finialize(void)
{
	if (m_pkRandNum)
	{
	}

	if (m_pkTriggerMgr)
	{
		m_pkTriggerMgr->Finialize();
	}

	if (m_pkUnitMgr)
	{
		m_pkUnitMgr->Finialize();
	}

	if (m_pkEffectMgr)
	{
		m_pkEffectMgr->Finialize();
	}

	if (m_pkMapItemMgr)
	{
		m_pkMapItemMgr->Finialize();
	}

	if (m_pkDataMgr)
	{
		m_pkDataMgr->Finialize();
	}

	if (m_pkMap)
	{
		m_pkMap->Finialize();
	}

	if (m_pkRenJiMgr)
	{
		m_pkRenJiMgr->Finialize();
	}

	BeLevelMain* pkTriggerMain = (BeLevelMain*)m_pkTaskTriggerHandle;
	if (pkTriggerMain)
	{
		pkTriggerMain->Finialize();
	}

	ResetData();

	m_akMapShareUnit.clear();
}

void BeMain::OnRaceOver(int iWinnerGroup, int iLastAttackPlayerIdx)
{
	const MapInfoTable* pkMapRes = gMain.GetMapInfoRes(m_iMapID);
	if (!pkMapRes)
	{
		return;
	}

	SetState(BMS_OVER);

	for (int i = 0; i < SSD_MAX_ROOM_PLAYER; ++i)
	{
		int iResultFlag = GetGameOver(i);
		if (GetPlayerType(i) == BPT_nullptr)
		{
			iResultFlag |= BGI_INVILAD;
		}
		else if (iWinnerGroup == GetPlayerCamp(i))
		{
			if (GetPlayerType(i) == BPT_DIS_PLAYER)
			{
				iResultFlag |= (BGI_FINISHED_SUCCES | BGI_GIVEUP);
			}
			else
			{
				iResultFlag |= (BGI_FINISHED_SUCCES);
			}
		}
		else
		{
			if (GetPlayerType(i) == BPT_DIS_PLAYER)
			{
				iResultFlag |= (BGI_FINISHED_FAILED | BGI_GIVEUP);
			}
			else
			{
				iResultFlag |= (BGI_FINISHED_FAILED);
			}
		}

		if (GetNoPlayerCheckCount(i) == 4)
		{
			iResultFlag |= BGI_NOPLAY_BIG;
		}
		else if (GetNoPlayerCheckCount(i) == 2)
		{
			iResultFlag |= BGI_NOPLAY_LITTLE;
		}
		SetGameOver(i, iResultFlag);
	}

	float			fKDA[SSD_MAX_ROOM_PLAYER] = { 0.0f };
	for (int iIndex = 0; iIndex < SSD_MAX_ROOM_PLAYER; iIndex++)
	{
		if (GetPlayerType(iIndex) == BPT_nullptr)
		{
			fKDA[iIndex] = 0.0f;
		}
		else
		{
			fKDA[iIndex] = GetPlayerKillCount(iIndex) * 5.0f + GetHelpCount(iIndex) * 4.0f + GetTotalHeroDamage(iIndex) * 0.001
				+ GetZhiLiao(iIndex) * 0.0005 + GetTotalBuildingDamage(iIndex) * 0.0015
				+ (GetTotalBeDamaged(iIndex) - GetPlayerDeathMaxHp(iIndex)) * 0.001
				+ GetEyeNum(iIndex) * 0.0f + GetKillSoildiers(iIndex) * 0.01f + GetCalculateGold(iIndex) * 0.001
				- GetPlayerBeKilledCount(iIndex) * 2.0f;
		}
		if (fKDA[iIndex] < 0.0f)
		{
			fKDA[iIndex] = 0.0f;
		}
		SetMVPValue(iIndex, fKDA[iIndex]);
	}
	int		iWinMVP = -1;
	float	fMaxWinKDA = 0.0f;
	int		iLoseMVP = -1;
	float	fMaxLoseKDA = -1;
	for (int iIndex = 0; iIndex < SSD_MAX_ROOM_PLAYER; iIndex++)
	{
		if (iWinnerGroup == GetPlayerCamp(iIndex))
		{
			if (fMaxWinKDA < fKDA[iIndex])
			{
				iWinMVP = iIndex;
				fMaxWinKDA = fKDA[iIndex];
			}
		}
		else
		{
			if (fMaxLoseKDA < fKDA[iIndex])
			{
				iLoseMVP = iIndex;
				fMaxLoseKDA = fKDA[iIndex];
			}
		}
	}
	if (iWinMVP != -1)
	{
		SetMVPTitleFlag(iWinMVP, BGI_MVP);
	}
	if (iLoseMVP != -1)
	{
		SetMVPTitleFlag(iLoseMVP, BGI_FAIL_MVP);
	}

	for (int iIndex = 0; iIndex < SSD_MAX_ROOM_PLAYER; iIndex++)
	{
		int		iKills = GetEndComboMaxKills(iIndex);
		if (iKills >= 5)
		{
			SetMVPTitleFlag(iIndex, BGI_FIVE_KILL);
		}
		else if (iKills == 4)
		{
			SetMVPTitleFlag(iIndex, BGI_FOURE_KILL);
		}
		else if (iKills == 3)
		{
			SetMVPTitleFlag(iIndex, BGI_THREE_KILL);
		}
	}

	int		iMaxKills = 0;
	for (int iIndex = 0; iIndex < SSD_MAX_ROOM_PLAYER; iIndex++)
	{
		int		iKills = GetPlayerKillCount(iIndex);
		if (iKills > iMaxKills)
		{
			iMaxKills = iKills;
		}
	}
	if (iMaxKills != 0)
	{
		for (int iIndex = 0; iIndex < SSD_MAX_ROOM_PLAYER; iIndex++)
		{
			int		iKills = GetPlayerKillCount(iIndex);
			if (iKills == iMaxKills)
			{
				SetMVPTitleFlag(iIndex, BGI_MAX_KILL);
			}
		}
	}

	int		iMaxGold = 0;
	int		iMaxGoldPlayer = -1;
	for (int iIndex = 0; iIndex < SSD_MAX_ROOM_PLAYER; iIndex++)
	{
		int		iGold = GetCalculateGold(iIndex);
		if (iGold > iMaxGold)
		{
			iMaxGold = iGold;
			iMaxGoldPlayer = iIndex;
		}
	}
	if (iMaxGoldPlayer != -1)
	{
		SetMVPTitleFlag(iMaxGoldPlayer, BGI_MAX_GOLD);
	}

	int		iMaxBuilding = 0;
	for (int iIndex = 0; iIndex < SSD_MAX_ROOM_PLAYER; iIndex++)
	{
		int	iBuilding = GetTotalBuildingDamage(iIndex);
		if (iBuilding > iMaxBuilding)
		{
			iMaxBuilding = iBuilding;
		}
	}
	if (iMaxBuilding != 0)
	{
		for (int iIndex = 0; iIndex < SSD_MAX_ROOM_PLAYER; iIndex++)
		{
			int	iBuilding = GetTotalBuildingDamage(iIndex);
			if (iBuilding == iMaxBuilding)
			{
				SetMVPTitleFlag(iIndex, BGI_MAX_BUILDING);
			}
		}
	}

	int		iMaxDamage = 0;
	int		iMaxDamagePlayer = -1;
	for (int iIndex = 0; iIndex < SSD_MAX_ROOM_PLAYER; iIndex++)
	{
		int		iDamage = GetTotalHeroDamage(iIndex);
		if (iDamage > iMaxDamage)
		{
			iMaxDamage = iDamage;
			iMaxDamagePlayer = iIndex;
		}
	}
	if (iMaxDamagePlayer != -1)
	{
		SetMVPTitleFlag(iMaxDamagePlayer, BGI_MAX_DAMAGE);
	}

	int		iMaxBeDamage = 0;
	int		iMaxBeDamagePlayer = -1;
	for (int iIndex = 0; iIndex < SSD_MAX_ROOM_PLAYER; iIndex++)
	{
		int		iBeDamage = GetTotalBeDamaged(iIndex);
		if (iBeDamage > iMaxBeDamage)
		{
			iMaxBeDamage = iBeDamage;
			iMaxBeDamagePlayer = iIndex;
		}
	}
	if (iMaxBeDamagePlayer != -1)
	{
		SetMVPTitleFlag(iMaxBeDamagePlayer, BGI_MAX_BEDAMAGE);
	}

	int		iMaxHelp = 0;
	for (int iIndex = 0; iIndex < SSD_MAX_ROOM_PLAYER; iIndex++)
	{
		int		iHelp = GetHelpCount(iIndex);
		if (iHelp > iMaxHelp)
		{
			iMaxHelp = iHelp;
		}
	}
	if (iMaxHelp != 0)
	{
		for (int iIndex = 0; iIndex < SSD_MAX_ROOM_PLAYER; iIndex++)
		{
			int		iHelp = GetHelpCount(iIndex);
			if (iHelp == iMaxHelp)
			{
				SetMVPTitleFlag(iIndex, BGI_MAX_HELP);
			}
		}
	}

	for (int iIndex = 0; iIndex < SSD_MAX_ROOM_PLAYER; iIndex++)
	{
		int		iKills = GetEndComboKills(iIndex);
		if (iKills >= 8)
		{
			SetMVPTitleFlag(iIndex, BGI_GOD);
		}
	}
	gMain.GetMapID();
	if (gMain.IsZhanChangMap())
	{
		for (int iIndex = 0; iIndex < SSD_MAX_ROOM_PLAYER; iIndex++)
		{
			SetCalculateGold(iIndex, GetPlayerKillEternalGuardCount(iIndex));
			SetKillSoildiers(iIndex, GetPlayerKillMercenariesCount(iIndex));
		}
	}

	SetKillBase(iLastAttackPlayerIdx);
}

void	BeMain::UpdateSurrender()
{
	if (m_iLeftSurrenderBeginTime > 0)
	{
		if (gTime % 100 != 0)
		{
			return;
		}
		if (m_iLeftSurrenderBeginTime + 60 * 1000 > gTime)
		{
			int		iNeedPlayer = 0;
			int		iAgreePlayer = 0;
			int		iGamePlayer = 0;
			for (int iPlayerIndex = 0; iPlayerIndex < MAX_ACTIVEPLAYERS / 2; iPlayerIndex++)
			{
				if (GetPlayerType(iPlayerIndex) == BPT_PLAYER)
				{
					iGamePlayer++;
					int		iSurrenderTime = GetSurrenderTime(iPlayerIndex);
					if (iSurrenderTime != 0 && gTime - iSurrenderTime < 60 * 1000)
					{
						iAgreePlayer++;
					}
				}
			}
			iNeedPlayer = ceil(iGamePlayer * 0.7);
			if (iAgreePlayer >= iNeedPlayer)
			{
				OnRaceOver(SRPC_CAMPB, SIDEB_SOILDER);

				for (int iResultIndex = 0; iResultIndex < MAX_ACTIVEPLAYERS; iResultIndex++)
				{
					if (iResultIndex < MAX_ACTIVEPLAYERS / 2)
					{
						SetGameOver(iResultIndex, BGI_SURRENDER_FAILED);
					}
					else
					{
						SetGameOver(iResultIndex, BGI_SURRENDER_SUCCES);
					}
				}
			}
		}
		else
		{
			m_iLeftSurrenderBeginTime = 0;
			for (int iPlayerIndex = 0; iPlayerIndex < MAX_ACTIVEPLAYERS / 2; iPlayerIndex++)
			{
				BeUnit* pkHero = GetHeroBySeat(iPlayerIndex);
				if (pkHero)
				{
					int		iSurrenderTime = GetSurrenderTime(iPlayerIndex);
					if (iSurrenderTime < 0)
					{
						SetSurrenderTime(iPlayerIndex, 0);
						pkHero->SetTabInfoFlag(BTCF_SURRENDER);
					}
				}
			}
		}
	}
	if (m_iRightSurrenderBeginTime > 0)
	{
		if (gTime % 100 != 0)
		{
			return;
		}
		if (m_iRightSurrenderBeginTime + 60 * 1000 > gTime)
		{
			int		iNeedPlayer = 0;
			int		iAgreePlayer = 0;
			int		iGamePlayer = 0;
			for (int iPlayerIndex = MAX_ACTIVEPLAYERS / 2; iPlayerIndex < MAX_ACTIVEPLAYERS; iPlayerIndex++)
			{
				if (GetPlayerType(iPlayerIndex) == BPT_PLAYER)
				{
					iGamePlayer++;
					int		iSurrenderTime = GetSurrenderTime(iPlayerIndex);
					if (iSurrenderTime != 0 && gTime - iSurrenderTime < 60 * 1000)
					{
						iAgreePlayer++;
					}
				}
			}

			iNeedPlayer = ceil(iGamePlayer * 0.7);
			if (iAgreePlayer >= iNeedPlayer)
			{
				OnRaceOver(SRPC_CAMPA, SIDEA_SOILDER);

				for (int iResultIndex = 0; iResultIndex < MAX_ACTIVEPLAYERS; iResultIndex++)
				{
					if (iResultIndex < MAX_ACTIVEPLAYERS / 2)
					{
						SetGameOver(iResultIndex, BGI_SURRENDER_SUCCES);
					}
					else
					{
						SetGameOver(iResultIndex, BGI_SURRENDER_FAILED);
					}
				}
			}
		}
		else
		{
			m_iRightSurrenderBeginTime = 0;
			for (int iPlayerIndex = MAX_ACTIVEPLAYERS / 2; iPlayerIndex < MAX_ACTIVEPLAYERS; iPlayerIndex++)
			{
				BeUnit* pkHero = GetHeroBySeat(iPlayerIndex);
				if (pkHero)
				{
					int		iSurrenderTime = GetSurrenderTime(iPlayerIndex);
					if (iSurrenderTime < 0)
					{
						SetSurrenderTime(iPlayerIndex, 0);
						pkHero->SetTabInfoFlag(BTCF_SURRENDER);
					}
				}
			}
		}
	}
}

void	BeMain::UpdatePlayerLeave()
{
	if (IsZhanChangMap() && gTime > 3 * 60 * 1000)
	{
		bool bDelZMap = true;
		for (int i = 0; i < MAX_ACTIVEPLAYERS; ++i)
		{
			if (GetPlayerType(i) == BPT_PLAYER)
			{
				bDelZMap = false;
				break;
			}
		}
		if (bDelZMap)
		{
			OnRaceOver(SRPC_OTHER_SIDE, SIDEA_SOILDER);
		}
	}
	else if (IsSecretArea())
	{
		for (int i = 0; i < 3; ++i)
		{
			if (GetPlayerType(i) == BPT_PLAYER)
			{
				return;
			}
		}
		OnRaceOver(SRPC_CAMPA, SIDEA_SOILDER);
	}

	if (gTime < 40 * 60 * 1000)
	{
		return;
	}

	if (gTime % 5000 == 0)
	{
		int		iMaxActivePlayers = MAX_ACTIVEPLAYERS;
		const	MapInfoTable* pkMapInfo = GetMapInfoRes(m_iMapID);
		if (!pkMapInfo)
		{
			return;
		}
		iMaxActivePlayers = pkMapInfo->iMaxPlayer;

		int		iCampAPlayerNum = 0;
		int		iCampBPlayerNum = 0;
		for (int i = 0; i < MAX_ACTIVEPLAYERS; ++i)
		{
			if (GetPlayerType(i) == BPT_PLAYER)
			{
				if (GetPlayerCamp(i) == SRPC_CAMPA)
				{
					iCampAPlayerNum++;
				}
				else
				{
					iCampBPlayerNum++;
				}
			}
		}

		if (iCampAPlayerNum <= 0)
		{
			OnRaceOver(SRPC_CAMPB, SIDEB_SOILDER);
		}
		else if (iCampBPlayerNum <= 0)
		{
			if (pkMapInfo->uiType != MAPINFO_TYPE_MMWAR)
			{
				OnRaceOver(SRPC_CAMPA, SIDEA_SOILDER);
			}
		}
	}
}

bool	BeMain::UpdateFrame(unsigned int dwFrame)
{
	BeMain* pkAttachMain = this;

	if (IsRacePause())
	{
		return true;
	}

	while (m_uiFrameCount < dwFrame)
	{
		m_pkTriggerMgr->Update(GAME_FRAME_TIME);
		m_pkEffectMgr->Update(GAME_FRAME_TIME);
		m_pkUnitMgr->Update(GAME_FRAME_TIME);
		m_pkMapItemMgr->Update(GAME_FRAME_TIME);
		m_pkMap->Update(GAME_FRAME_TIME);
		m_pkRenJiMgr->Update(GAME_FRAME_TIME);

		BeLevelMain* pkTriggerMain = (BeLevelMain*)m_pkTaskTriggerHandle;
		if (pkTriggerMain)
		{
			pkTriggerMain->Update(GAME_FRAME_TIME);
		}

		UpdatePlayerLeave();
		UpdateSurrender();

		m_uiGameTime += GAME_FRAME_TIME;
		m_uiFrameCount++;

		if (!gMain.IsSecretArea()) {
			if ((int)m_uiGameTime >= 60 * 60 * 1000)
			{
				OnRaceOver(SRPC_CAMPB, 0);
			}
		}
	}
	return true;
}

bool BeMain::LoadRes(int iSeed)
{
	ReleaseModule();

	m_pkRandNum = new SeRandNum;
	m_pkRandNum->Initialize(iSeed);

	const MapInfoTable* pkMapRes = gMain.GetResMapInfo(GetMapID());
	if (!pkMapRes)
	{
		return false;
	}
	std::string kMapConfig = pkMapRes->kMapInfoPath;
	gMapInfo.PreloadMap(kMapConfig.c_str());

	m_pkDataMgr = new BeDataMgr;
	m_pkDataMgr->AttachMain(this);

	m_pkRenJiMgr = new BeNewRenjiMgr;
	m_pkRenJiMgr->AttachMain(this);

	m_pkTriggerMgr = new TePtTriggerMgr(this);
	m_pkEffectMgr = new BeEffectMgr;
	m_pkEffectMgr->AttachMain(this);

	m_pkMapItemMgr = new BeMapItemMgr;
	m_pkMapItemMgr->AttachMain(this);

	m_pkUnitMgr = new BeUnitMgr;
	m_pkUnitMgr->AttachMain(this);

	m_pkFormulaInfo = new BeFormulaInfo;
	m_pkFormulaInfo->AttachMain(this);

	if (m_pkTaskTriggerHandle == nullptr)
	{
		m_pkTaskTriggerHandle = new BeLevelMain;
	}
	BeLevelMain* pkTriggerMain = (BeLevelMain*)m_pkTaskTriggerHandle;
	if (pkTriggerMain)
	{
		pkTriggerMain->AttachMain(this);
		pkTriggerMain->Initialize();
	}
	m_pkMap = new BeMap;
	m_pkMap->AttachMain(this);
	if (!m_pkMap->Initialize(kMapConfig))
	{
		return false;
	}
	if (!Initialize())
	{
		return false;
	}
	if (!InitializeGame())
	{
		return false;
	}
	if (!CreateHero())
	{
		return false;
	}

	return true;
}

BeUnit* BeMain::GetUnitControlBySeat(unsigned short bySeat, int iUnitID)
{
	BeUnit* pkUnit = m_pkUnitMgr->GetUnitByID(iUnitID, true);
	if (pkUnit && pkUnit->UnitCanBeControl(bySeat))
	{
		return pkUnit;
	}
	return nullptr;
}

void BeMain::SetGameFrame(unsigned int dwFrame)
{
	m_uiFrameCount = dwFrame;
	m_uiGameTime = GAME_FRAME_TIME * m_uiFrameCount;
}

const MapInfoTable* BeMain::GetMapInfoRes(unsigned int iMapID)
{
	if (iMapID != m_iCurMapID)
	{
		m_iCurMapID = iMapID;
		m_pkCurMapInfo = MapInfoTableMgr::Get()->GetMapInfoTable(iMapID);
	}

	return m_pkCurMapInfo;
}

int BeMain::GetSkillOrgTypeID(int iSkillTypeID)
{
	const SkillTable* pkSkillRes = GetResSkill(iSkillTypeID);
	if (pkSkillRes && pkSkillRes->uiOriginTypeID == (UInt)-1)
	{
		return iSkillTypeID;
	}

	return pkSkillRes->uiOriginTypeID;
}

void BeMain::SetCameraFocusPos(int iPlayer, float fPosX, float fPosY, BeUnit* pkUnit, bool bSetFocusByRelive)
{
	BeShareUIWindowData	kData;
	kData.iUnitID = iPlayer;
	kData.eWindowType = BWT_CAMARA_SET;
	kData.fValue1 = fPosX;
	kData.fValue2 = fPosY;

	AddWindowData(kData);
}

const std::vector<BeShareDamageData>& BeMain::GetVisionDamageData(int iHeroSeat, UnitGroupID& rkVisionUnitID)
{
	m_kVisionDamageData.clear();

	for (int i = 0; i < (int)m_kAllShareDamageData.size(); i++)
	{
		BeShareDamageData	pkData = m_kAllShareDamageData[i];
		if (pkData.eType == BSDT_GOLDEN) {

			m_kVisionDamageData.push_back(m_kAllShareDamageData[i]);

		}
		else {

			for (int j = 0; j < (int)rkVisionUnitID.Size(); j++)
			{
				if (rkVisionUnitID[j] == m_kAllShareDamageData[i].iUnitID)
				{
					m_kVisionDamageData.push_back(m_kAllShareDamageData[i]);
					break;
				}
			}
		}
	}

	return m_kVisionDamageData;
}

void BeMain::AddDamageData(BeShareDamageData& kData)
{
	if (kData.eType == BSDT_ADD_LIFE ||
		kData.eType == BSDT_DAMAGE ||
		kData.eType == BSDT_CRITILE)
	{
		BeUnit* pkAttacker = gUnitMgr.GetUnitByID(kData.iAttackerID, true);
		if (pkAttacker)
		{
			if (!IsShowAllDamage())
			{
				if (pkAttacker->GetClass() != UNIT_CLASSTYPE_HERO && pkAttacker->GetClass() != UNIT_CLASSTYPE_PET)
				{
					return;
				}
			}
		}
	}

	for (int i = 0; i < m_kAllShareDamageData.size(); i++)
	{
		BeShareDamageData& kFindData = m_kAllShareDamageData[i];
		if (kFindData.iUnitID == kData.iUnitID && kData.eType != BSDT_GOLDEN && kData.eType != BSDT_EXP && kData.eType != BSDT_ZHANCHANG_FEATS && kData.eType != BSDT_ZHANCHANG_SCORE)
		{
			kFindData.fValue += kData.fValue;
			if (kData.eType == BSDT_CRITILE)
			{
				kFindData.eType = BSDT_CRITILE;
			}
			return;
		}
	}

	m_kAllShareDamageData.push_back(kData);
}

void BeMain::AddEntityPointer(BeGenIDType eType, int iID, void* pkVoid)
{
	if (eType > GIT_EFFECT || iID < 0 || iID >= 65535)
	{
		return;
	}
	m_akEntityPointer[eType][iID] = pkVoid;
}

void BeMain::DelEntityPointer(BeGenIDType eType, int iID)
{
	if (eType > GIT_EFFECT || iID < 0 || iID >= 65535)
	{
		return;
	}
	m_akEntityPointer[eType][iID] = nullptr;
}

void* BeMain::GetEntityPointer(BeGenIDType eType, int iID)
{
	if (eType > GIT_EFFECT || iID < 0 || iID >= 65535)
	{
		return nullptr;
	}

	return m_akEntityPointer[eType][iID];
}

void	BeMain::AddSmallMapMessage(float fX, float fY, bool bAttack, int iResID, int iTime, int iPlayer, int iTarID, bool bItem)
{
	BeShareUIWindowData	kData;
	kData.eWindowType = BWT_SMALLMAP_MESSAGE;
	kData.fValue1 = fX;
	kData.fValue2 = fY;
	kData.iRemoveTime = iTime;
	kData.iResID = iResID;

	AddWindowData(kData);
}

bool BeMain::IsPlayerCharacterMeet(int iIdx, int iCharacter)
{
	bool bResult = false;
	BePlayerCharacter eCharacter = GetPlayerCharacter(iIdx);
	switch (eCharacter)
	{
	case BPC_Radical:
		bResult = (iCharacter & RENJI_CHARACTER_RADICAL) == RENJI_CHARACTER_RADICAL ? true : false;
		break;
	case BPC_BRAVE:
		bResult = (iCharacter & RENJI_CHARACTER_BRAVE) == RENJI_CHARACTER_BRAVE ? true : false;
		break;
	case BPC_POSITIVE:
		bResult = (iCharacter & RENJI_CHARACTER_POSITIVE) == RENJI_CHARACTER_POSITIVE ? true : false;
		break;
	case BPC_NEGATIVE:
		bResult = (iCharacter & RENJI_CHARACTER_NEGATIVE) == RENJI_CHARACTER_NEGATIVE ? true : false;
		break;
	case BPC_CONSERVATION:
		bResult = (iCharacter & RENJI_CHARACTER_CONSERVATIVE) == RENJI_CHARACTER_CONSERVATIVE ? true : false;
		break;
	case BPC_WEAK:
		bResult = (iCharacter & RENJI_CHARACTER_WEAK) == RENJI_CHARACTER_WEAK ? true : false;
		break;
	default:
		break;
	}

	return bResult;
}

void	BeMain::AddUIMessage(int iUnitID, int iType, int iStrIndex, int iTime)
{
	BeShareUIWindowData	kData;

	kData.eWindowType = (BeUiWindowType)iType;
	kData.iUnitID = iUnitID;
	kData.iResID = iStrIndex;
	kData.fValue1 = iTime;
	kData.fValue4 = 502;

	AddWindowData(kData);
}

void	BeMain::AddFontEffect(int iUnitID, int iType, int iStrIndex, int iTime, int iColor, int iLable, int iFontSize)
{
	BeShareUIWindowData	kData;

	kData.eWindowType = (BeUiWindowType)iType;
	kData.iUnitID = iUnitID;
	kData.iResID = iStrIndex;
	kData.fValue1 = iTime;
	kData.fValue2 = iColor;
	kData.fValue3 = iFontSize;
	kData.fValue4 = iLable;

	AddWindowData(kData);
}

void BeMain::SetHeroDataChangeFlag(int iSeat, int iFlag)
{
	if (iSeat < 0 || iSeat >= MAX_ACTIVEPLAYERS)
	{
		return;
	}
	m_aiHeroDataChangeFlag[iSeat] |= iFlag;
}

int BeMain::GetHeroDataChangeFlag(int iSeat)
{
	if (iSeat < 0 || iSeat >= MAX_ACTIVEPLAYERS)
	{
		return 0;
	}
	return m_aiHeroDataChangeFlag[iSeat];
}

void BeMain::ClrHeroDataChangeFlag(int iSeat, int iFlag)
{
	if (iSeat < 0 || iSeat >= MAX_ACTIVEPLAYERS)
	{
		return;
	}
	m_aiHeroDataChangeFlag[iSeat] &= ~iFlag;
}

bool BeMain::HasHeroDataChangeFlag(int iSeat, int iFlag)
{
	if (iSeat < 0 || iSeat >= MAX_ACTIVEPLAYERS)
	{
		return false;
	}

	int iHeroDataChangeFlag = m_aiHeroDataChangeFlag[iSeat];
	return ((iHeroDataChangeFlag & iFlag) == iFlag ? true : false);
}

void BeMain::AddPlayerDeathData(int iSeat, int iBeginTime, int iReliveTime)
{
	if (iSeat < 0 || iSeat >= MAX_ACTIVEPLAYERS)
	{
		return;
	}
	m_akPlayer[iSeat].iDeathBeginTime = iBeginTime;
	m_akPlayer[iSeat].iReLiveTime = iReliveTime;
}

bool	BeMain::HasGrassVisionForCamp(int iGrassIndex, int iSrcCamp, int iDstCamp)
{
	auto	Iter = m_kGrassUnit.begin();
	while (Iter != m_kGrassUnit.end())
	{
		if (Iter->second == iGrassIndex)
		{
			BeUnit* pkUnit = gUnitMgr.GetUnitByID(Iter->first);
			if (pkUnit)
			{
				if (pkUnit->GetCamp() == iDstCamp)
				{
					return true;
				}
			}
		}
		++Iter;
	}
	return false;
}

bool	BeMain::HasGrassVision(int iSrcUnitID, int iTarUnitID)
{
	std::map<int, int>::iterator iter = m_kGrassUnit.find(iTarUnitID);
	if (iter == m_kGrassUnit.end())
	{
		return true;
	}
	int iTarGrassIndex = iter->second;
	std::map<int, int>::iterator iterSrc = m_kGrassUnit.begin();

	int		iSrcCamp = -1;
	BeUnit* pkSrcUnit = gUnitMgr.GetUnitByID(iSrcUnitID);
	if (pkSrcUnit)
	{
		iSrcCamp = pkSrcUnit->GetCamp();
	}
	int		iTarCamp = -1;
	BeUnit* pkTarUnit = gUnitMgr.GetUnitByID(iTarUnitID);
	if (pkTarUnit)
	{
		iTarCamp = pkTarUnit->GetCamp();
	}
	if (iTarCamp == iSrcCamp)
	{
		return true;
	}

	for (; iterSrc != m_kGrassUnit.end(); iterSrc++)
	{
		int iGrassUnitID = iterSrc->first;
		BeUnit* pkGrassUnit = gUnitMgr.GetUnitByID(iGrassUnitID);
		if (pkGrassUnit)
		{
			if (pkGrassUnit->GetCamp() == iSrcCamp)
			{
				if (iterSrc->second == iTarGrassIndex)
				{
					return true;
				}
			}
		}
	}

	return false;
}
