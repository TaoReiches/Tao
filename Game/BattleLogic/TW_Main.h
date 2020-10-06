#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

class	BeRandLogProcedure;
class	SeThread;
struct	SeCR_RaceEndRace;

typedef		void* BeTaskTriggerHandle;
typedef		void	(*FrameRandLogCallBack)(unsigned int, void*);
typedef		void	(*HpRandLogCallBack)(int, int, void*);
typedef		void	(*MpRandLogCallBack)(int, int, void*);
typedef		void	(*DamageRandLogCallBack)(int, int, void*);

class   BeMain
{
public:
	BeMain(void);
	~BeMain(void);
private:
	BeMain* pkAttachMain;
public:
	int GetSoldierPlayerByGroup(SeRoomPlayerCamp eCamp) const;
protected:
	int m_aiGroupSoldier[SRPC_NUMS];

public:
	void SetPlayerInfo(int iIdx, int iID, int iHeroID, const char* acName);

public:
	bool LoadRes(int iSeed);
	BeUnit* GetUnitControlBySeat(unsigned short bySeat, int iUnitID);

	inline UInt GetFrameCount(void)
	{
		return m_uiFrameCount;
	}

	inline void BeginRace(void)
	{
		m_eState = BMS_RACING;
	}

	inline int GenerateID(BeGenIDType eType)
	{
		if (eType < GIT_ENTITY || eType >= GIT_MAX)
		{
			eType = GIT_ENTITY;
		}

		return ++m_aiGenID[eType];
	}

	inline int SetGenerateID(BeGenIDType eType, int iID)
	{
		if (eType < GIT_ENTITY || eType >= GIT_MAX)
		{
			return 0;
		}

		return m_aiGenID[eType] = iID;
	}

	inline void SetState(BeMainState eState)
	{
		if (eState == BMS_OVER)
		{
		}
		m_eState = eState;
	}

	inline BeMainState GetState(void) const
	{
		return m_eState;
	}

	inline const bool PlayerHasZItem(int iIdx, int iUniqueID) const
	{
		CHECK_IDX(iIdx);

		for (int i = 0; i < m_akPlayer[iIdx].iZItenCount; i++)
		{
			if (m_akPlayer[iIdx].akZItemsInfo[i].iUniqueID == iUniqueID)
			{
				return true;
			}
		}
		return false;
	}

	inline const bool PlayerHasQualityZItem(int iIdx, unsigned int uiQuality)
	{
		CHECK_IDX(iIdx);
		for (int i = 0; i < m_akPlayer[iIdx].iZItenCount; i++)
		{
			const ZhanChangItemTable* pkInfo = GetResZhanChangItem(m_akPlayer[iIdx].akZItemsInfo[i].iItemType);
			if (pkInfo && pkInfo->uiItemQuality == uiQuality)
			{
				return true;
			}
		}
		return false;
	}

	inline const BeZItemInfo* GetPlayerZItemInfoByID(int iIdx, int iUniqueID) const
	{
		CHECK_IDX(iIdx);

		for (int i = 0; i < m_akPlayer[iIdx].iZItenCount; i++)
		{
			if (m_akPlayer[iIdx].akZItemsInfo[i].iUniqueID == iUniqueID)
			{
				return &m_akPlayer[iIdx].akZItemsInfo[i];
			}
		}
		return NULL;
	}

	inline const BePlayerInfo& GetPlayerInfo(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx];
	}

	inline void DelPlayer(int iIdx)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].eType = BPT_DIS_PLAYER;
		m_akPlayer[iIdx].iID = 0;
	}

	inline void ClearPlayer(int iIdx)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].Clear();
	}

	inline bool IsPlayerActive(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return (m_akPlayer[iIdx].eType >= BPT_ROBOT);
	}

	inline void SetPlayerType(int iIdx, BePlayerType eType)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].eType = eType;
	}
	inline void SetPlayerCamp(int iIdx, int iValue)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iCamp = iValue;
	}

	inline BePlayerType GetPlayerType(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].eType;
	}

	inline void SetPlayerCharacter(int iIdx, BePlayerCharacter eType)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].eCharacter = eType;
	}

	inline BePlayerCharacter GetPlayerCharacter(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].eCharacter;
	}

	inline void SetPlayerID(int iIdx, int iPlayerID)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iID = iPlayerID;
	}

	inline int GetPlayerID(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iID;
	}

	inline int GetPlayerCamp(int iIdx) const
	{
		if (iIdx >= 0 && iIdx < MAX_PLAYERS)
		{
			return m_akPlayer[iIdx].iCamp;
		}
		return 0;
	}

	inline void SetPlayerName(int iIdx, const char* pcName)
	{
		CHECK_IDX(iIdx);
		strcpy(m_akPlayer[iIdx].acName, pcName);
	}

	inline const char* GetPlayerName(int iIdx) const
	{
		if (iIdx < 0 || iIdx >= MAX_PLAYERS)
		{
			return NULL;
		}
		return m_akPlayer[iIdx].acName;
	}

	inline int GetPlayerDeathBeginTime(int iIdx)
	{
		if (iIdx < 0 || iIdx >= MAX_PLAYERS)
		{
			return 0;
		}
		return m_akPlayer[iIdx].iDeathBeginTime;
	}

	inline int GetPlayerDeathEndTime(int iIdx)
	{
		if (iIdx < 0 || iIdx >= MAX_PLAYERS)
		{
			return 0;
		}
		return m_akPlayer[iIdx].iReLiveTime;
	}
	inline int GetPlayerGold(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iGold;
	}

	int GetPlayerBuyItemCount(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iBuyItemCount;
	}

	void AddPlayerBuyItemCount(int iIdx)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iBuyItemCount++;
	}

	inline void SetPlayerAllGold(int iIdx, int iGold)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iAllGold = iGold;
	}

	inline int GetAllPlayerGold(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iAllGold;
	}

	inline int GetCalculateGold(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iCalCulateGold;
	}

	inline void SetCalculateGold(int iIdx, int iValue)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iCalCulateGold = iValue;
	}

	inline void SetLocalPlayReliveTime(int iIdx, int iTime)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iReLiveTime = iTime;
	}

	inline int GetLocalPlayReliveTime(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iReLiveTime;
	}

	inline int GetPlayerContinueKill(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iContinueKill;
	}

	inline int GetPlayerMaxContinueKill(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iMaxContinueKill;
	}

	inline int GetPlayerKillCount(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iKillCount;
	}

	inline void AddPlayerBeKilledCount(int iIdx, int iBeKilledCount)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iBeKilledCount += iBeKilledCount;
	}

	inline int GetPlayerBeKilledCount(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iBeKilledCount;
	}

	inline void SetPlayerBeKilledCount(int iIdx, int iCount)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iBeKilledCount = iCount;
	}

	inline void AddPlayerDeathMaxHp(int iIdx, int iDeathMaxHp)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iDeathMaxHp += iDeathMaxHp;
	}

	inline int GetPlayerDeathMaxHp(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iDeathMaxHp;
	}

	inline void SetPlayerDeathMaxHp(int iIdx, int iCount)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iDeathMaxHp = iCount;
	}

	inline void AddTownNum(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iTownNum += iAddNum;
	}

	inline void SetTownNum(int iIdx, int iNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iTownNum = iNum;
	}

	inline int GetTownNum(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iTownNum;
	}

	inline void SetTownAssistNum(int iIdx, int iNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iTownAssistNum = iNum;
	}

	inline void AddTownAssistNum(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iTownAssistNum += iAddNum;
	}

	inline int GetTownAssistNum(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iTownAssistNum;
	}

	inline	int		GetSurrenderTime(int iIdx)	const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iSurrenderTime;
	}

	inline void SetSurrenderTime(int iIdx, int iTime)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iSurrenderTime = iTime;
	}

	inline void SetNoPlayerCheckTime(int iIdx, int iTime)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iNoPlayCheckTime = iTime;
	}

	inline void SetNoPlayerCheckCount(int iIdx, int iCount)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iCheckCount = iCount;
	}

	inline int GetNoPlayerCheckCount(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iCheckCount;
	}

	inline int GetNoPlayerCheckTime(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iNoPlayCheckTime;
	}

	inline void SetCampAssistNum(int iIdx, int iNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iCampAssistNum = iNum;
	}

	inline void AddCampAssistNum(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iCampAssistNum += iAddNum;
	}

	inline int GetCampAssistNum(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iCampAssistNum;
	}

	inline void SetImTownNum(int iIdx, int iNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iImTownNum = iNum;
	}

	inline void AddImTownNum(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iImTownNum += iAddNum;
	}

	inline int GetImTownNum(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iImTownNum;
	}

	inline void AddCampNum(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iCampNum += iAddNum;
	}

	inline void SetCampNum(int iIdx, int iNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iCampNum = iNum;
	}

	inline int GetCampNum(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iCampNum;
	}

	inline void SetImCampNum(int iIdx, int iNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iImCampNum = iNum;
	}

	inline void AddImCampNum(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iImCampNum += iAddNum;
	}

	inline int GetImCampNum(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iImCampNum;
	}

	inline int GetBuildingNum(int iIdx)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iBuildingNum = m_akPlayer[iIdx].iCampNum + m_akPlayer[iIdx].iTownNum;
		return m_akPlayer[iIdx].iBuildingNum;
	}


	inline int GetImBuildingNum(int iIdx)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iImBuildingNum = m_akPlayer[iIdx].iImCampNum + m_akPlayer[iIdx].iImTownNum;
		return m_akPlayer[iIdx].iImBuildingNum;
	}

	inline void SetKillSoildiers(int iIdx, int iCount)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iKillSoildiers = iCount;
	}

	inline void AddKillSoildiers(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iKillSoildiers += iAddNum;
	}

	inline int GetKillSoildiers(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iKillSoildiers;
	}

	inline void SetImKillSoildiers(int iIdx, int iCount)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iImKillSoildiers = iCount;
	}

	inline void AddImKillSoildiers(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		//SetHeroDataChangeFlag(iIdx,SPRDCF_IMKILLSOLIDER);
		m_akPlayer[iIdx].iImKillSoildiers += iAddNum;
	}

	inline int GetImKillSoildiers(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iImKillSoildiers;
	}

	inline void AddKillBigBossNum(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iKillBigBossNum += iAddNum;
	}
	inline void AddKillSmallBossNum(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iKillSmallBossNum += iAddNum;
	}
	inline int GetKillBigBossNum(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iKillBigBossNum;
	}
	inline int GetKillSmallBossNum(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iKillSmallBossNum;
	}

	inline void SetKillMonsterNum(int iIdx, int iNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iKillMonsterNum = iNum;
	}

	inline void AddKillMonsterNum(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iKillMonsterNum += iAddNum;
	}

	inline int GetKillMonsterNum(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iKillMonsterNum;
	}

	inline void AddTotalDamage(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iTotalDamage += iAddNum;
	}

	inline int GetTotalDamage(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iTotalDamage;
	}
	inline void AddTotalHeroDamage(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iTotalHeroDamage += iAddNum;
	}
	inline int GetTotalHeroDamage(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iTotalHeroDamage;
	}

	inline void AddTotalBuildingDamage(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iTotalBuildingDamage += iAddNum;
	}

	inline int GetTotalBuildingDamage(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iTotalBuildingDamage;
	}

	inline void AddTotalBeDamaged(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iTotalBeDamaged += iAddNum;
	}

	inline int GetTotalBeDamaged(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iTotalBeDamaged;
	}
	inline void AddZhiLiao(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iZhiLiao += iAddNum;
	}
	inline int GetZhiLiao(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iZhiLiao;
	}

	inline void AddEyeNum(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iUseEyeTimes += iAddNum;
	}
	inline int GetEyeNum(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iUseEyeTimes;
	}

	inline void AddComboKills(int iIdx, int iAddNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iComboKills += iAddNum;
		if (m_akPlayer[iIdx].iComboKills > m_akPlayer[iIdx].iEndComboKills)
		{
			m_akPlayer[iIdx].iEndComboKills = m_akPlayer[iIdx].iComboKills;
		}
	}
	inline void SetEndComboMaxKills(int iIdx, int iNum)
	{
		CHECK_IDX(iIdx);
		if (m_akPlayer[iIdx].iEndComboMaxKills < iNum)
		{
			m_akPlayer[iIdx].iEndComboMaxKills = iNum;
		}
	}
	inline int GetEndComboMaxKills(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iEndComboMaxKills;
	}
	inline int GetComboKills(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iComboKills;
	}
	inline int GetEndComboKills(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iEndComboKills;
	}
	inline int GetComboKillsAdjust(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return (m_akPlayer[iIdx].iComboKills <= 10) ? m_akPlayer[iIdx].iComboKills : 10;
	}

	inline void SetComboMaxKills(int iIdx, int iNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iComboMaxKills = iNum;
	}

	inline int GetComboMaxKills(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iComboMaxKills;
	}

	inline void ClearComboKills(int iIdx)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iComboKills = 0;
	}

	inline int GetComboDeaths(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iComboDeaths;
	}

	inline void AddComboDeaths(int iIdx)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iComboDeaths++;
	}

	inline void ClearComboDeaths(int iIdx)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iComboDeaths = 0;
	}

	inline void ReduceComboDeaths(int iIdx, int iNum)
	{
		CHECK_IDX(iIdx);
		if (m_akPlayer[iIdx].iComboDeaths >= iNum) {
			m_akPlayer[iIdx].iComboDeaths -= iNum;
		}
		else
		{
			m_akPlayer[iIdx].iComboDeaths = 0;
		}
	}

	inline int GetSoldiersGoldByNum(int iIdx)
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iSoldiersGoldByNum;
	}

	inline void AddSoldiersGoldByNum(int iIdx, int iNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iSoldiersGoldByNum += iNum;
	}

	inline void ClearSoldiersGoldByNum(int iIdx)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iSoldiersGoldByNum = 0;
	}

	inline void SetHeroID(int iIdx, int iID)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iHeroID = iID;
		BeUnit* pkUnit = m_pkUnitMgr->GetUnitByID(iID, true);
		if (pkUnit)
		{
			m_akPlayer[iIdx].iHeroUnitID = pkUnit->GetTypeID();
		}
	}

	inline int GetHeroID(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iHeroID;
	}

	inline int GetHeroUnitID(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iHeroUnitID;
	}

	inline void SetGameOver(int iIdx, int iOverFlag)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iGameOverFlag |= iOverFlag;
	}

	inline void SetGameValid(bool bValid)
	{
		m_bGameValid = bValid;
	}

	inline bool GetGameValid(void)
	{
		return m_bGameValid;
	}

	inline UInt GetPlayerAttackTime(int iIdx, int iPlayerID)
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].auiAttackedTime[iPlayerID];
	}

	inline UInt GetPlayerAddBufferTime(int iIdx, int iPlayerID)
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].auiAddBufferTime[iPlayerID];
	}

	inline UInt GetPlayerHelpTime(int iIdx, int iPlayerID)
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].auiHelpTime[iPlayerID];
	}

	inline void AddPlayerFeatsValue(int iIdx, int iNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iFeatsValue += iNum;
		if (m_akPlayer[iIdx].iFeatsValue > 60)
		{
			m_akPlayer[iIdx].iFeatsValue = 60;
		}
	}

	inline int GetPlayerFeatsValue(int iIdx)
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iFeatsValue;
	}

	inline void AddPlayerMoraleValue(int iIdx, int iNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iMoraleValue += iNum;
	}

	inline int SetPlayerMoraleValue(int iIdx, int iNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iMoraleValue = iNum;
	}

	inline int DoPlayerMoralePunish(int iIdx)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iMoraleValue = m_akPlayer[iIdx].iMoraleValue - (m_akPlayer[iIdx].iMoraleValue / 3);
		return m_akPlayer[iIdx].iMoraleValue;
	}

	inline int GetPlayerMoraleValue(int iIdx)
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iMoraleValue;
	}

	inline void AddPlayerTeamScore(int iIdx, int iNum)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iTeamScore += iNum;
	}

	inline int GetPlayerTeamScore(int iIdx)
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iTeamScore;
	}

	inline void	AddCampTeamScore(int iCamp, int iNum)
	{
		if (iCamp < 0 || iCamp >= MAX_GROUPS)
		{
			return;
		}
		m_iCampTeamScore[iCamp] += iNum;
	}

	inline int	GetCampTeamScore(int iCamp)
	{
		if (iCamp < 0 || iCamp >= MAX_GROUPS)
		{
			return 0;
		}
		return m_iCampTeamScore[iCamp];
	}

	void	SetCampTabInfoFlag(int iFlag)
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			int iHeroID = GetHeroID(i);
			BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(iHeroID, true);
			if (pkHero)
			{
				pkHero->SetTabInfoFlag(iFlag);
			}
		}
	}

	inline	int GetRedGuardFlag()
	{
		return m_iRedGuardFlag;
	}

	inline	void AddRedGuardFlag()
	{
		m_iRedGuardFlag += 1;
	}

	inline	int GetBlueGuardFlag()
	{
		return m_iBlueGuardFlag;
	}

	inline	void AddBlueGuardFlag()
	{
		m_iBlueGuardFlag += 1;
	}

	inline	int		GetPlayerKillEternalGuardCount(int iIdx)
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iKillEternalGuard;
	}
	inline	void	AddPlayerKillEternalGuardCount(int iIdx, int iVlaue)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iKillEternalGuard += iVlaue;
	}

	inline	int		GetPlayerKillMercenariesCount(int iIdx)
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iKillNeutralMercenaries;
	}
	inline	void	AddPlayerKillMercenariesCount(int iIdx, int iVlaue)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iKillNeutralMercenaries += iVlaue;
	}

	inline void SetPlayerAttackTime(int iIdx, int iPlayerID)
	{
		if (iIdx >= 0 && iIdx < MAX_PLAYERS)
		{
			m_akPlayer[iIdx].auiAttackedTime[iPlayerID] = m_uiGameTime;
		}
	}

	inline void SetPlayerAddBufferTime(int iIdx, int iPlayerID)
	{
		if (iIdx >= 0 && iIdx < MAX_PLAYERS)
		{
			m_akPlayer[iIdx].auiAddBufferTime[iPlayerID] = m_uiGameTime;
		}
	}

	inline void SetPlayerHelpTime(int iIdx, int iPlayerID)
	{
		if (iIdx >= 0 && iIdx < MAX_PLAYERS)
		{
			m_akPlayer[iIdx].auiHelpTime[iPlayerID] = m_uiGameTime;
		}
	}

	inline void ClearPlayerHelpTime(int iIdx, int iPlayerID)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].auiHelpTime[iPlayerID] = 0;
	}

	inline int IsRevengePlayer(int iIdx, int iPlayer)
	{
		CHECK_IDX(iIdx);
		for (int i = 0; i < MAX_PLAYERS; ++i)
		{
			if (m_akPlayer[iIdx].akRevengeInfo[i].iKillPlayer == iPlayer)
			{
				return m_akPlayer[iIdx].akRevengeInfo[i].iKillNums;
			}
		}

		return 0;
	}


	inline int GetGameOver(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iGameOverFlag;
	}

	inline int GetGameTime(void) const
	{
		return (int)m_uiGameTime;
	}

	inline void SetGameTime(int iIdx, int iTime)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iGameTime = iTime;
	}

	inline int GetGameTime(int iIdx)
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iGameTime;
	}

	inline void ForceSetGameTime(UInt iTime)
	{
		m_uiGameTime = iTime;
	}

	inline void SetKillBase(int iIdx)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iKillBase = 1;
	}

	inline unsigned short GetKillBase(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return (unsigned short)m_akPlayer[iIdx].iKillBase;
	}

	void SetPlayerHeroTypeID(int iIdx, int iValue)
	{
		CHECK_IDX(iIdx);

		m_akPlayer[iIdx].iTypeID = iValue;
	}

	inline int GetHeroProficiencyEffectID(int iIdx)
	{
		if (iIdx >= 0 && iIdx < MAX_PLAYERS)
		{
			if (m_akPlayer[iIdx].uiHeroProficiency < 2500) {
				return 0;
			}
			if (m_akPlayer[iIdx].uiHeroProficiency < 10000) {
				return 6003;
			}
			else if (m_akPlayer[iIdx].uiHeroProficiency < 30000) {
				return 6004;
			}
			else {
				return 6005;
			}
		}
		return 0;
	}

	void SetHeropProficiency(int iIdx, uint32_t iValue)
	{
		CHECK_IDX(iIdx);

		m_akPlayer[iIdx].uiHeroProficiency = iValue;
	}

	int GetPlayerHeroTypeID(int iIdx)
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iTypeID;
	}

	inline void SetPlayerHeroID(int iIdx, int iValue)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iHeroID = iValue;
	}

	inline void SetGroupComboKill(SeRoomPlayerCamp ePlayGroup, int iKill)
	{
		if (ePlayGroup >= SRPC_OTHER_SIDE)
		{
			return;
		}
		m_aiGroupComboKill[(int)ePlayGroup] = iKill;
	}

	inline int GetGroupComboKill(SeRoomPlayerCamp ePlayGroup) const
	{
		if (ePlayGroup >= SRPC_OTHER_SIDE)
		{
			return 0;
		}
		return m_aiGroupComboKill[(int)ePlayGroup];
	}

	inline int GetBuyLiveCount(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iBuyLiveCount;
	}

	inline void AddBuyLiveCount(int iIdx)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iBuyLiveCount++;
	}

	inline int GetMapID(void) const
	{
		return m_iMapID;
	}

	inline const char* GetRaceID(void) const
	{
		return m_acRaceID;
	}

	inline int GetOwner(void) const
	{
		return m_iOwner;
	}

	inline void SetOwner(int iOwner)
	{
		m_iOwner = iOwner;
	}

	inline bool IsAutoMatch(void)
	{
		return m_bAutoMatch;
	}

	inline void SetAutoMatch(bool bAuto)
	{
		m_bAutoMatch = bAuto;
	}

	inline void SetGuoZhanCityFlag(int iCityFlag)
	{
		m_iGuoZhanCityFlag = iCityFlag;
	}

	inline int GetGuoZhanCityFlag(void)
	{
		return m_iGuoZhanCityFlag;
	}

	inline void SetCurMapFirstGame(bool bFirst)
	{
		m_bCurMapFirstGame = bFirst;
	}

	inline bool GetCurMapFirstGame(void)
	{
		return m_bCurMapFirstGame;
	}

	inline bool IsMovieOn(void)
	{
		return m_bIsMovieOn;
	}

	inline void SetMovieOn(bool bOn)
	{
		m_bIsMovieOn = bOn;
	}

	inline bool IsCameraRotate(void)
	{
		return m_bIsCameraRotate;
	}

	inline void SetCameraRotate(bool bOn)
	{
		m_bIsCameraRotate = bOn;
	}

	inline void SetCameraChanging(bool bChange)
	{
		m_bIsCameraChange = bChange;
	}

	inline void SetCameraNeedReset(bool bReset)
	{
		m_bNeedCameraReset = bReset;
	}

	inline bool IsCameraNeedReset(void)
	{
		return m_bNeedCameraReset;
	}

	inline bool IsCameraChange(void)
	{
		return m_bIsCameraChange;
	}

	inline void AddTotalCommandNums(int iIdx, int iAddNums)
	{
		CHECK_IDX(iIdx);
		if (m_iPreGiveCommandTimes == 0 || m_iPreGiveCommandTimes != (int)m_uiGameTime)
		{
			m_iPreGiveCommandTimes = (int)m_uiGameTime;
			m_akPlayer[iIdx].iTotalCommandNums += iAddNums;
		}
	}

	inline void SetAPM(int iIdx, int iAPM)
	{
		m_akPlayer[iIdx].iTotalCommandNums = iAPM * 3;
		m_akPlayer[iIdx].iGameTime = 3 * 60 * 1000;
	}

	inline int GetAPM(int iIdx)
	{
		CHECK_IDX(iIdx);
		if (m_akPlayer[iIdx].iGameTime < 3 * 60 * 1000)
		{
			return 0;
		}

		return m_akPlayer[iIdx].iTotalCommandNums * 60 / (m_akPlayer[iIdx].iGameTime / 1000 + 1);
	}

	inline void AddHelpCount(int iIdx, int iHelpNums)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iAssistCount += iHelpNums;
	}

	inline int GetHelpCount(int iIdx)
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iAssistCount;
	}

	inline void SetDifficultyGrade(int iDifficultyGrade)
	{
		m_iDifficultyGrade = iDifficultyGrade;
	}

	inline int GetDifficultyGrade()
	{
		return m_iDifficultyGrade;
	}

	inline BeUnit* GetHeroBySeat(unsigned short bySeat)
	{
		CHECK_IDX(bySeat);
		int iHeroID = GetHeroID(bySeat);
		return m_pkUnitMgr->GetUnitByID(iHeroID, true);
	}

	inline std::map<unsigned int, bool>& GetChongWuKongZhiSwitch(void)
	{
		return m_kMChongWuKongZhiSwitch;
	}

	inline std::map<unsigned int, std::map<unsigned int, bool>>& GetShareSwitch(void)
	{
		return m_kMShareSwitch;
	}

	inline const int GetMaxSidePlayers(void) const
	{
		return m_iMaxSidePlayer;
	}

	inline void SetMaxSidePlayers(int iNum)
	{
		m_iMaxSidePlayer = iNum;
	}

	inline 	int GetGameStartTime(void) const
	{
		return 0;
	}

	void AddAttachData(BeShareAttachData& kData)
	{
		m_kAttachData.push_back(kData);
	}

	void ClearAttachData()
	{
		m_kAttachData.clear();
	}
	const std::vector<BeShareAttachData>& GetAttachData()
	{
		return m_kAttachData;
	}

	void ClearMapItem()
	{
		m_kMapItemData.clear();
	}
	const std::vector<BeShareMapItemData>& GetMapItemData()
	{
		return m_kMapItemData;
	}
	void AddMapItemData(BeShareMapItemData& kData)
	{
		m_kMapItemData.push_back(kData);
	}
	const std::vector<BeShareBufferData>& GetBufferShowData()
	{
		return m_kBufferShowData;
	}
	void AddBufferShowData(BeShareBufferData& kData)
	{
		m_kBufferShowData.push_back(kData);
	}
	void ClearBufferShowData()
	{
		m_kBufferShowData.clear();
	}

	inline const int GetAddAssistGold(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iAddAssistGold;
	}

	inline void SetAddAssistGold(int iIdx, int iGold)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iAddAssistGold = iGold;
	}

	inline const float GetAddFarmGoldPer(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].fAddFarmGoldPer;
	}

	inline void SetAddFarmGoldPer(int iIdx, float fGold)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].fAddFarmGoldPer = fGold;
	}

	inline const float GetAddExpPer(int iIdx) const
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].fAddExpPer;
	}

	inline void SetAddExpPer(int iIdx, float fExp)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].fAddExpPer = fExp;
	}

	inline int GetMVPTitleFlag(int iIdx)
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iMVPTitleFlag;
	}

	inline void SetMVPTitleFlag(int iIdx, int iFlag)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iMVPTitleFlag |= iFlag;
	}

	inline int GetMVPValue(int iIdx)
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iMVPValue;
	}

	inline void SetMVPValue(int iIdx, int iFlag)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iMVPValue = iFlag;
	}

	inline int GetSpyNums(int iIdx)
	{
		CHECK_IDX(iIdx);
		return m_akPlayer[iIdx].iSpyNums;
	}

	inline void AddSpyNums(int iIdx, int iNums)
	{
		CHECK_IDX(iIdx);
		m_akPlayer[iIdx].iSpyNums += iNums;
	}

	inline void SetAllTowerDeadCamp(SeRoomPlayerCamp eCamp, bool bDead = true)
	{
		if (eCamp < SRPC_INVALID || eCamp >= SRPC_NUMS)
		{
			return;
		}

		m_abAllTowerDeadCamp[eCamp] = bDead;
	}

	inline bool GetAllTowerDeadCamp(SeRoomPlayerCamp eCamp)
	{
		if (eCamp < SRPC_INVALID || eCamp >= SRPC_NUMS)
		{
			return false;
		}

		return m_abAllTowerDeadCamp[eCamp];
	}

	inline void AddOtherCampAllDeadCount(SeRoomPlayerCamp eCamp, int iCount = 1)
	{
		if (eCamp < SRPC_INVALID || eCamp >= SRPC_NUMS)
		{
			return;
		}

		m_aiOtherCampAllDeadCount[eCamp] += iCount;
	}

	inline bool GetOtherCampAllDeadCount(SeRoomPlayerCamp eCamp)
	{
		if (eCamp < SRPC_INVALID || eCamp >= SRPC_NUMS)
		{
			return false;
		}

		return m_aiOtherCampAllDeadCount[eCamp] != 0;
	}

	inline BeTaskTriggerHandle	GetTaskHandle(void)
	{
		return m_pkTaskTriggerHandle;
	}

	inline bool IsZMap(void) const
	{
		return m_bZMap;
	}
	inline void SetZMap(bool bImba)
	{
		m_bZMap = bImba;
	}

	inline bool IsSecretArea(void) const
	{
		return m_bSecretArea;
	}

	inline void SetSecretArea(bool bImba)
	{
		m_bSecretArea = bImba;
	}

	inline bool IsPveMap(void) const
	{
		return m_bPVEMap;
	}
	inline void SetPveMap(bool bFlag)
	{
		m_bPVEMap = bFlag;
	}

	inline bool IsZhanChangMap() const
	{
		return m_iMapID == BMID_ZHANCHANG_MATCH_1 || m_iMapID == BMID_ZHANCHANG_MATCH_2 || m_iMapID == BMID_ZHANCHANG_MATCH_3;
	}

	inline void SetRandomSkillTimes(int iPlayer, int iCount)
	{
		CHECK_IDX(iPlayer);
		if (iCount < 0 || iCount >= MAX_RANDOMSKILL_TIMES)
		{
			iCount = MAX_RANDOMSKILL_TIMES;
		}
		m_aiRandomSkillTimes[iPlayer] = iCount;
	}

	inline int GetRandomSkillTimes(int iPlayer)
	{
		if (iPlayer < 0 || iPlayer >= MAX_ACTIVEPLAYERS)
		{
			return MAX_RANDOMSKILL_TIMES;
		}

		return m_aiRandomSkillTimes[iPlayer];
	}

	inline int GetRandomSkillGroupScore(int iPlayer)
	{
		if (iPlayer < 0 || iPlayer >= MAX_ACTIVEPLAYERS)
		{
			return 0;
		}

		return m_aiRandomSkillGroupScore[iPlayer];
	}

	inline void SetHackRushDiff(int iHL)
	{
		m_iHackRushDiff = iHL;
	}

	inline int GetHackRushDiff(void)
	{
		return m_iHackRushDiff;
	}

	inline void SetGuardRebirthTime(int iTime)
	{
		m_iNextGuardRebirthTime = iTime;
	}
	inline int GetGuardRebirthTime()
	{
		return m_iNextGuardRebirthTime;
	}

protected:
	bool ChiBi3InitAwardCmd(int iPlayer, int iData);
	bool GuanDuInitAwardCmd(int iPlayer, int iData);
	bool GuanDuImbaInitAwardCmd(int iPlayer, int iData);
	bool PKInitAwardCmd(int iPlayer, int iData);
	bool GuoZhanInitAwardCmd(int iPlayer, int iData, bool bRobot = false);

	bool BossFightInitAwardCmd(int iPlayer, int iData);
	bool SanGuoDaZhanInitAwardCmd(int iPlayer, int iData);
	bool DaLuanDouInitAwardCmd(int iPlayer, int iData);
	bool DZMBInitAwardCmd(int iPlayer, int iData);
	bool GouZiDaZhanInitAwardCmd(int iPlayer, int iData, bool bOtherSel);

	bool PlayerSendCommand(BeUnit* pkUnit, int iSendIdx);

	bool PlayerFuShenCommand(BeUnit* pkPlayer, int iFuShenUnitID);

	bool PlayerReliveStateCommand(BeUnit* pkPlayer, int iData);

public:
	bool YuLeInitAwardCmd(int iPlayer, int iData);
	bool SanGuoDaZhanCmd(int iPlayer, int iData, int iData2, const std::string& rkCmdStr = "");
	bool ZLDLDCmd(int iPlayer, int iData, int iData2, const std::string& rkCmdStr = "");

	bool UnitSelectHero(const BeSelectHero& kCmd, bool bSetSel = true);
	bool CheatCommand(const BeCheatCommand& kCmd);
	bool GuanQiaCommand(const BeGuanQiaCommand& kCmd);

	bool CreateHero(int iPlayer = -1);
	bool CreateOneHero(int iPlayer = -1);
	bool DelOneHero(int iSeat);

	bool GetHeroOrgPos(int iSeat, BePos2& rkPos, float& rfFace) const;
	bool UnitUseTactic(int iCamp, int iPos, int iUsePlayer, bool bUse = false);
	void ExcuPlayerReconBegin(int iSeat, unsigned int dwPlayerID);

public:
	int		GetHeroTypeID(int iIdx);
	const char* GetHeroName(int iIdx);
	int		GetHeroLevel(int iIdx);

	int		TimeGetHour(void) const;
	int		TimeGetMinute(void) const;
	int		TimeGetSecond(void) const;

	void	SetLoadingProgress(float fProgress);
	float	GetLoadingProgress(void) const;

	void SetMapID(int iMapID);
	void SetRaceID(const char* acRaceID);
	void SetRSBossCount(int iCount);
	int  GetRSBossCount();
	void PushExistRSGroup(int iIndex);
	bool IsExistRSGroup(int iIndex);
	void SetDropSkillTime(int iTime);
	int  GetDropSkillTime();
	int  GetCurMapItemDesc(void);

public:
	void SetCameraFocusPos(int iPlayer, float fPosX, float fPosY, BeUnit* pkUnit = NULL, bool bSetFocusByRelive = false);
	void AddPlayerGold(int iIdx, int iGold, bool bIsCalculate = true, bool bIsSellMoney = false);
	void AddPlayerKillCount(int iIdx, int iKillCount);
	void SetPlayerKillCount(int iIdx, int iKillCount);

	const SkillTable* GetResSkill(int iSkillTypeID, const BeUnit* pkUnit = NULL) const
	{
		return SkillTableMgr::Get()->GetSkillTable(iSkillTypeID);
	}
	const UnitTable* GetResUnit(int iUnitID) const;

	SeRaceEndPlayerInfo GetRaceEndInfo(int iIndex, int iPlayerNums);

	bool				RandFloatInRate(float fRate, int iRandID);

	BeLabelObj* GetLabelObject(const EsHandle& rkHandle);
	int		GetPetType(int iIdx) const;
	int		GetPlayerIdx(int iID) const;
	void	SetPlayerGroup(int iIdx, int eGroup, bool bSetPlayer = true);
	void	SetPlayerGold(int iIdx, int iGold);
	void	DelRevengePlayer(int iIdx, int iPlayer);
	void	AddRevengePlayer(int iIdx, int iPlayer);
	bool	SetNormalMissionID(int iIdx, int iMissionID);
	bool	SetMissionID(int iIdx, int iMissionID);
	int		GetTotalNormalMissionNums(int iIdx);
	int		GetTotalMissionCompleteNums(int iIdx);
	bool	SetACBossID(int iIdx, int iBossID);

public:
	void	UpdateRaceOut();
	int		GetUnitRaceOut(int iSeatIndex, int iOffset, char* pcData, int iSize);
	int		GetSvrModelRaceOut(int iOffset, char* pcData, int iSize);
	void	ClearRaceRecord();
	int		GetViewRaceOut(int iOffset, char* pcData, int iSize);

	void	InitServerMode();
	void	FinializeServerMode();

protected:
	void	PushPureDataCmdRecord(SeRoomPlayerCamp eCamp, const void* pkData, int iSize);

private:
	inline SeRoomPlayerCamp GetCamp(int i);

	int		m_aiLockUnit[SSD_MAX_ROOM_PLAYER];
	int		m_aiLastLockUnit[SSD_MAX_ROOM_PLAYER];
public:
	bool HasChangeFlag(int iFlag) const;
	void SetChangeFlag(int iFlag);
	void ClrChangeFlag(int iFlag);
	int GetChangeFlag() const;

	void	GetTabinfoData();

	void	SetHeroDataChangeFlag(int iSeat, int iFlag);
	int		GetHeroDataChangeFlag(int iSeat);
	void	ClrHeroDataChangeFlag(int iSeat, int iFlag);
	bool	HasHeroDataChangeFlag(int iSeat, int iFlag);
	BeShareLockUnitData& GetLockUnitData(int iPlayer);

	void	ClrAllPureData();
protected:
	int								m_aiHeroDataChangeFlag[SSD_MAX_ROOM_PLAYER];
	BeShareLockUnitData				m_kLockUnitData[SSD_MAX_ROOM_PLAYER];
	BeShareLockUnitData				m_kLastLockUnitData[SSD_MAX_ROOM_PLAYER];
	int								m_iChangeFlag;

public:
	void	OnPlayerRelink(int i);

private:
	int		m_iMaxPerLoadID;
	bool	m_bReLink;

public:
	void	AddSmallMapMessage(float fX, float fY, bool bAttack, int iResID, int iTime, int iPlayer = -1, int iTarID = 0, bool bItem = false);
	void	AddUIMessage(int iUnitID, int iType, int iStrIndex, int iTime);
	void	AddFontEffect(int iUnitID, int iType, int iStrIndex, int iTime, int iColor, int iLable, int iFontSize);

	const ItemTable* GetResItem(int iTypeID) const
	{
		if (iTypeID == 0)
		{
			return NULL;
		}

		const ItemTable* pkItemRes = ItemTableMgr::Get()->GetItemTable(iTypeID);

		return pkItemRes;
	}

	const ZhanChangItemTable* GetResZhanChangItem(int iTypeID) const
	{
		if (iTypeID == 0)
		{
			return NULL;
		}

		const ZhanChangItemTable* pkZItemRes = ZhanChangItemTableMgr::Get()->GetZhanChangItemTable(iTypeID);

		return pkZItemRes;
	}

	const ZhanChangSkillTable* GetResZhanChangSkill(int iTypeID) const
	{
		if (iTypeID == 0)
		{
			return NULL;
		}

		const ZhanChangSkillTable* pkZhanChangSkillRes = ZhanChangSkillTableMgr::Get()->GetZhanChangSkillTable(iTypeID);

		return pkZhanChangSkillRes;
	}

	const ItemComposeTable* GetResItemCompose(int iTypeID) const
	{
		if (iTypeID == 0)
		{
			return NULL;
		}

		const ItemComposeTable* pkItemComposeRes = ItemComposeTableMgr::Get()->GetItemComposeTable(iTypeID);

		return pkItemComposeRes;
	}
	const MapInfoTable* GetResMapInfo(int iTypeID) const
	{
		if (iTypeID == 0)
		{
			return NULL;
		}
		const MapInfoTable* pkMapInfoRes = MapInfoTableMgr::Get()->GetMapInfoTable(iTypeID);
		return pkMapInfoRes;
	}

public:
	BeCarry* GetTrgCarry(void);
	int			GetTrgSkillTypeID(void);
	int			GetTrgSkillOrgTypeID(void);
	int			GetTrgSkillLevel(void);
	int			GetTrgSpellTargetID(void);
	int* GetTrgPtrSpellTargetID(void);
	int			GetTrgSpellUsePlayer(void);
	float       GetTrgSpellOrgPosX(void);
	float		GetTrgSpellOrgPosY(void);
	float		GetTrgSpellTargetPosX(void);
	float		GetTrgSpellTargetPosY(void);
	float		GetTrgSpellDirectPosX(void);
	float		GetTrgSpellDirectPosY(void);
	BeUnit* GetTrgUnit(bool bIncSuspend = false);
	BeUnit* GetTrgAttacker(void);
	float		GetTrgAttackedDamge(void);
	int			GetTrgAttackerPlayer(void);
	BeUnit* GetTrgTarget(void);
	int			GetTrgAttackedType(void);
	int			GetTrgAttackedIsFirst(void);
	int			GetTrgAttackedFlag(void);
	int* GetTrgAttackedFlagPtr(void);
	int			GetTrgBufferTypeID(void);
	int			GetTrgBufferLevel(void);
	int			GetTrgBufferInnerID(void);
	int			GetTrgTargetID();

	int			GetTrgIntData(void);
	float		GetTrgFloatData(void);
	bool		GetTrgIsLearnSkill(void);

	BeUnit* TrgGetPtr_Unit(void);
	BeUnit* TrgGetPtr_Attacker(void);
	BeUnit* TrgGetPtr_Target(void);
	BeSkill* TrgGetPtr_Skill(void);
	BeBuffer* TrgGetPtr_Buffer(void);
	BeEffect* TrgGetPtr_Effect(void);
	BeItem* TrgGetPtr_Item(void);
	BeMapItem* TrgGetPtr_MapItem(void);
	int			TrgGetItemPos(void);
	int			TrgGetItemID(void);
	int			TrgGetItemTypeID(void);
	int			TrgGetTriggerItemID(void);
	float* TrgGetPtr_Damage(void);
	float		GetTrgAttackedRawDamge(void);
	float		TrgGetDamageFace(void);

	int			GetTrgMissionID(void);
	int			GetTrgMissionPlayer(void);

	BeAttackingAttr* TrgGetPtr_AttackAttr(void);
	int* TrgGetPtr_Persist(void);
	int* TrgGetPtr_PersistDelta(void);
	int TrgGetTargetType(void);

	TePtCondResult DefaultBuffer_Condition(int iOriginTypeID);
	bool DefaultSkill_ActionParam(BeUnit*& pkTrgUnit, UInt& uiSkillTypeID, int& iSkillLevel, const SkillTable*& pkSkillRes);

	void InitSkillEventTrigger(int iTypeID);
	void InitSkillEventTrigger1(int iTypeID);
	void InitSkillEventTrigger2(int iTypeID);
	void InitSkillEventTrigger3(int iTypeID);
	void InitPetSkillEventTrigger(int iTypeID);
	void InitZhuanShuSkillEventTrigger(int iTypeID);
	void InitItemEventTrigger(int iTypeID);

public:

	bool ProcFrameCMD(const SeRaceCmd* pkRaceCmd);

	bool Proc_FRAME(const SeRaceCmd* pkRaceCmd);
	bool Proc_PLAYER_INFO(const SeRaceCmd* pkRaceCmd);
	bool Proc_LOAD(const SeRaceCmd* pkRaceCmd);
	bool Proc_START(const SeRaceCmd* pkRaceCmd);
	bool Proc_END(const SeRaceCmd* pkRaceCmd);
	bool Proc_CHART_PLAYER(const SeRaceCmd* pkRaceCmd);
	bool Proc_PLAYER_LEAVE(const SeRaceCmd* pkRaceCmd);
	bool Proc_SYSTEM_CHEAT(const SeRaceCmd* pkRaceCmd);
	bool Proc_SYSTEM_SURRENDER(const SeRaceCmd* pkRaceCmd);
	bool Proc_HERO_MOVE(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_MOVE(const SeRaceCmd* pkRaceCmd);
	bool Proc_HERO_STOP(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_STOP(const SeRaceCmd* pkRaceCmd);
	bool Proc_HERO_HOLD(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_HOLD(const SeRaceCmd* pkRaceCmd);
	bool Proc_HERO_ATTACK_POS(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_ATTACK_POS(const SeRaceCmd* pkRaceCmd);
	bool Proc_HERO_ATTACK_UNIT(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_ATTACK_UNIT(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_ATTACK(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_PATROL(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_FOLLOW(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_SPELL(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_SPELLXuLi(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_USE_ITEM(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_DROP_ITEM(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_PICK_ITEM(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_PICK_ITEM_AUTO(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_BUY_ITEM(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_DIRECTBUY_ITEM(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_DIRECTSELL_ITEM(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_SWAP_ITEM(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_LEARN_SKILL(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_ACTIVE_SKILL(const SeRaceCmd* pkRaceCmd);
	bool Proc_PLAYER_RELIVEHERO(const SeRaceCmd* pkRaceCmd);
	bool Proc_PLAYER_MAP_MESSAGE(const SeRaceCmd* pkRaceCmd);
	bool Proc_PLAYER_GUANQIA_CMD(const SeRaceCmd* pkRaceCmd);
	bool Proc_PLAYER_TOWNITEM(const SeRaceCmd* pkRaceCmd);
	bool Proc_PLAYER_RECON_BEGIN(const SeRaceCmd* pkRaceCmd);
	bool Proc_PLAYER_RECON_END(const SeRaceCmd* pkRaceCmd);
	bool Proc_VIEW_SELHERO(const SeRaceCmd* pkRaceCmd);
	bool Proc_PAUSE_BEGIN(const SeRaceCmd* pkRaceCmd);
	bool Proc_PAUSE_END(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_DISPLACE_ITEM(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_GU_DISPLACE_ITEM(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_CHANGE_ITEM(const SeRaceCmd* pkRaceCmd);
	bool Proc_TURN_TABLE_INFO(const SeRaceCmd* pkRaceCmd);
	bool Proc_COST_EXTERNAL_RESOURCE(const SeRaceCmd* pkRaceCmd);
	bool Proc_Show_Action(const SeRaceCmd* pkRaceCmd);
	bool Proc_8V8_DEL_PLAYER(const SeRaceCmd* pkRaceCmd);
	bool Proc_8V8_ADD_PLAYER(const SeRaceCmd* pkRaceCmd);

	bool Proc_Race_LockUnit(const SeRaceCmd* pkRaceCmd);
	bool Proc_Race_AdjustFrame(const SeRaceCmd* pkRaceCmd);

	bool Proc_Race_RelinkBegin(const SeRaceCmd* pkRaceCmd);
	bool Proc_Race_RelinkEnd(const SeRaceCmd* pkRaceCmd);

	bool IsRacePause();

	bool	Proc_HERO_DIRECT_MOVE(const SeRaceCmd* pkRaceCmd);
	bool	Proc_HERO_ATTACK_CANCEL(const SeRaceCmd* pkRaceCmd);
	bool	Proc_HERO_SELECT(const SeRaceCmd* pkRaceCmd);
	bool	Proc_SWITCH_HEROFOCUS(const SeRaceCmd* pkRaceCmd);
	bool	Proc_Ping(const SeRaceCmd* pkRaceCmd);
	bool	Proc_SmallMapMessage(const SeRaceCmd* pkRaceCmd);

	bool Initialize(void);
	//void Update(int iDeltaTime);
	void Finialize(void);
	void OnRaceOver(int iWinnerGroup, int iLastAttackPlayer);
	bool UpdateFrame(unsigned int dwFrame);

	bool	InitializeGame(void);
	void	InitPlayerGroup(void);
	void	ResetData(void);
	void	SetGameFrame(unsigned int dwFrame);

	const MapInfoTable* GetMapInfoRes(unsigned int iMapID);

	TePtCondResult DefaultSkill_Condition(int iOriginTypeID);

	int GetSkillOrgTypeID(int iSkillTypeID);

	void	SetGameBeginTime(int iValue)
	{
		//m_kGlobalData.iChangeFlag |= BGDCF_GAMEBEGINTIME;

		m_iGameBeginTime = iValue;
	}
	int		GetGameBeginTime()
	{
		return m_iGameBeginTime;
	}
	const	std::vector<BeShareChainEffectData>& GetChainData()
	{
		return m_kShareChainData;
	}
	void	AddChainData(BeShareChainEffectData& kData)
	{
		m_kShareChainData.push_back(kData);
	}
	void	ClearChainData()
	{
		m_kShareChainData.clear();
	}

	const	std::vector<BeShareDamageData>& GetDamageData()
	{
		return m_kAllShareDamageData;
	}
	void	AddDamageData(BeShareDamageData& kData);
	void	ClearDamageData()
	{
		m_kAllShareDamageData.clear();
	}

	const	std::vector<BeShareKillHeroData>& GetKillHeroData()
	{
		return m_kKillHeroData;
	}
	void	AddKillHeroData(BeShareKillHeroData& kData)
	{
		m_kKillHeroData.push_back(kData);
	}
	void	ClearKillHeroData()
	{
		m_kKillHeroData.clear();
	}

	void	AddCampKill(int iCamp)
	{
		if (iCamp < 0 || iCamp >= MAX_GROUPS)
		{
			return;
		}
		m_iCampKills[iCamp]++;
	}
	int		GetCampKill(int iCamp)
	{
		if (iCamp < 0 || iCamp >= MAX_GROUPS)
		{
			return 0;
		}
		return m_iCampKills[iCamp];
	}

	const	std::vector<BeShareUIWindowData>& GetWindowData()
	{
		return m_kWindowData;
	}
	void	AddWindowData(BeShareUIWindowData& kData)
	{
		if (kData.iSkillID != 'TA06')
		{
			m_kWindowData.push_back(kData);
		}
	}
	void	ClearWindowData()
	{
		m_kWindowData.clear();
	}

	int		GetFirstBloodPlayer()
	{
		return m_iFirstBloodPlayer;
	}
	void	SetFirstBloodPlayer(int iValue)
	{
		m_iFirstBloodPlayer = iValue;
	}
	int		GetFirstDestoryTowerPlayer()
	{
		return m_iFirstDestoryTower;
	}
	void	SetFirstDestoryTowerPlayer(int iValue)
	{
		m_iFirstDestoryTower = iValue;
	}

	void SetHeroFocusPlayerID(int iPlayer, int iFocusPlayer)
	{
		CHECK_IDX(iPlayer);
		m_aiFocusHeroID[iPlayer] = iFocusPlayer;
	}

	int GetHeroFocusPlayerID(int iPlayer)
	{
		CHECK_IDX(iPlayer);
		return m_aiFocusHeroID[iPlayer];
	}
	const BeShareSelfData& GetSelfShowData(int iSeat, bool bRecon = false);
	const BeShareUnitData& GetUnitShowData(BeUnit* pkUnit, bool bNew = false);
	const BeShareEffectData& GetEffectShowData(BeEffect* pkEffect, bool bRecon = false);
	const std::vector<BeShareEffectData>& GetShareEffectData(bool bRecon = false);
	const std::vector<BeShareDamageData>& GetVisionDamageData(int iHeroSeat, UnitGroupID& rkVisionUnitID);
	void GenVisionUnit(int iHeroSeat, UnitGroupID& rkGroupID);

	void DisplaceItemAdd(BeUnit* pkUnit, int iItemPrice);

protected:
	void			ReleaseModule(void);
	void			LoadLevelMainMapFile(void);

	void			UpdatePlayerLeave();
	void			UpdateSurrender();

public:
	UInt			m_uiSecretAreaEndTime;
	UInt			m_uiSecretAreaLayer;
	UInt			m_uiGameTime;
	SeRandNum* m_pkRandNum;
	BeMap* m_pkMap;
	BeDataMgr* m_pkDataMgr;
	BeUnitMgr* m_pkUnitMgr;
	BeEffectMgr* m_pkEffectMgr;
	BeMapItemMgr* m_pkMapItemMgr;
	TePtTriggerMgr* m_pkTriggerMgr;
	BeFormulaInfo* m_pkFormulaInfo;
	BeTaskTriggerHandle	m_pkTaskTriggerHandle;
	BeNewRenjiMgr* m_pkRenJiMgr;

protected:
	BePlayerInfo	m_akPlayer[MAX_PLAYERS];
	char			m_acRaceID[36];
	int				m_iMapID;
	std::string		m_kMapName;
	int				m_iOwner;
	bool			m_bAutoMatch;
	int				m_iGuoZhanCityFlag;
	bool			m_bCurMapFirstGame;
	bool			m_bNewMapData;

	BeMainState		m_eState;
	UInt			m_uiFrameCount;
	int				m_iGameTimeLeft;
	UInt			m_uiRealTimeNow;
	int				m_aiGenID[GIT_MAX];
	bool			m_bIsMovieOn;
	bool			m_bIsCameraRotate;
	bool			m_bIsCameraChange;
	bool			m_bNeedCameraReset;
	int				m_iOverTime;
	bool			m_bGameValid;
	int				m_iDifficultyGrade;
	int             m_aiGroupComboKill[20];

	std::map<int, ShareUnitData> m_akMapShareUnit;


	std::map<unsigned int, bool>			m_kMChongWuKongZhiSwitch;
	std::map<unsigned int, std::map<unsigned int, bool>> m_kMShareSwitch;

	std::map<int, std::vector<int>>			m_kShareLeave;

	std::vector<BeShareAttachData>			m_kAttachData;
	std::vector<BeShareMapItemData>			m_kMapItemData;
	std::vector<BeShareBufferData>			m_kBufferShowData;
	std::vector<BeShareKillHeroData>		m_kKillHeroData;
	std::vector<BeShareUIWindowData>		m_kWindowData;
	BeShareSelfData							m_kSelfData;
	BeShareUnitData							m_kShareUnitData;
	BeShareEffectData						m_kShareEffectData;
	std::vector<BeShareEffectData>			m_akEffectData;

	int										m_iFirstBloodPlayer;
	int										m_iFirstDestoryTower;

	unsigned int	m_fProgress;

	int				m_iLeftSurrenderBeginTime;
	int				m_iRightSurrenderBeginTime;

	int				m_iMaxSidePlayer;

	bool			m_bBaseHasDestroy;

	bool			m_bZMap;
	bool			m_bPVEMap;
	bool			m_b8v8Map;
	bool			m_bSecretArea;

	std::vector<BeShareChainEffectData>				m_kShareChainData;
	std::vector<BeShareDamageData>					m_kAllShareDamageData;
	std::vector<BeShareDamageData>					m_kVisionDamageData;
	std::vector<int> m_aiVecHeroTypeID;

	int									m_iVersion[MAX_ACTIVEPLAYERS];

	float								m_fZhanGongSoliderAdd[SRPC_NUMS];

	int									m_iPreGiveCommandTimes;

	unsigned int						m_iCurMapID;
	const MapInfoTable* m_pkCurMapInfo;
	bool								m_bShowAllDamage;

	std::vector<BeTownItemStack> m_kTownItemStack;

	std::vector<BeShareUnitFlyData>			m_kUnitFlyData;

	bool								m_bOpenZuoBi;
	int									m_iSelfZuoBi;
	bool								m_bAllZuoBi;
	bool		m_bNoon;
	bool		m_bCheatNight;
	bool		m_abAllTowerDeadCamp[SRPC_NUMS];
	int			m_aiOtherCampAllDeadCount[SRPC_NUMS];

	bool								m_bOver;

	SeRaceRecord						m_akPureDataCmdRecord[SERVERMODE_MAXPLAYER];
	SeRaceRecord						m_kViewData;

	char								m_pcSend[4096];
	SeNetStream							m_kNetStream;

	bool								m_bRacePause;
	BeRaceGameModel						m_eRaceGameModel;
	int									m_aiRandomSkillTimes[MAX_ACTIVEPLAYERS];
	int									m_aiRandomSkillGroupScore[MAX_ACTIVEPLAYERS];
	int									m_iRSBossCount;
	std::vector<int>					m_aiVecExistRSGroupIndex;
	int									m_iDropSkillTime;

	int									m_iCampKills[MAX_GROUPS];
	int									m_iCampTeamScore[MAX_GROUPS];
	int									m_iRedGuardFlag;
	int									m_iBlueGuardFlag;
	int									m_iNextGuardRebirthTime;

	BeMain8V8State						m_e8V8State;

	int									m_iHackRushDiff;

	int									m_iGameBeginTime;

	int									m_aiFocusHeroID[MAX_ACTIVEPLAYERS];
protected:
	bool		NeedSendPing(int iIdx)
	{
		CHECK_IDX(iIdx);
		return m_bSendPing[iIdx];
	}

private:
	bool					m_bSendPing[MAX_ACTIVEPLAYERS];
	std::vector<std::pair<int, BeShareSmallMapMessage>>		m_kSmallMapMessage;
	std::vector<BeShareGameNotice>							m_kGameNoticeData;

public:
	void	AddEntityPointer(BeGenIDType eType, int iID, void* pkVoid);
	void	DelEntityPointer(BeGenIDType eType, int iID);
	void* GetEntityPointer(BeGenIDType eType, int iID);
	bool	IsPlayerCharacterMeet(int iIdx, int iCharacter);

	bool	IsShowAllDamage()
	{
		return m_bShowAllDamage;
	}

	void	AddUnitFlyData(BeShareUnitFlyData& kData)
	{
		m_kUnitFlyData.push_back(kData);
	}
	void	ClearUnitFlyData()
	{
		m_kUnitFlyData.clear();
	}
	const std::vector<BeShareUnitFlyData>& GetUnitFlyData()	const
	{
		return m_kUnitFlyData;
	}

	void    AddPlayerDeathData(int iSeat, int iBeginTime, int iReliveTime);

	void	RemoveGrassUnit(int iUnitID, int iGrassIndex)
	{
		std::map<int, int>::iterator iter = m_kGrassUnit.find(iUnitID);
		if (iter != m_kGrassUnit.end())
		{
			m_kGrassUnit.erase(iter);
		}
	}
	void	SetGrassUnit(int iUnitID, int iGrassIndex)
	{
		m_kGrassUnit[iUnitID] = iGrassIndex;
	}
	bool	HasGrassVision(int iSrcUnitID, int iTarUnitID);
	bool	HasGrassVisionForCamp(int iGrassIndex, int iSrcCamp, int iDstCamp);

private:
	void* m_akEntityPointer[GIT_EFFECT + 1][65535];

	std::map<int, int>			m_kGrassUnit;
	std::map<int, std::vector<int>>		m_kSkillUserData;

public:
	inline	void		SetCampLastKiller(int iCamp, int iUnitID);
	inline	int			GetCampLastKiller(int iCamp);
	inline	void		AddGameNoticeData(BeShareGameNotice& kData);
	inline	float		GetEquipAddArmor(int iPlayer);
	inline	float		GetEquipAddMagicDamage(int iPlayer);
	inline	float		GetEquipAddMagicArmor(int iPlayer);
	inline	std::map<int, std::vector<int>>& GetSkillUserData();
	inline	void		SetCampMaxScore(int iValue);
	inline	int			GetCampMaxScore();
	inline	void		Set8V8State(BeMain8V8State iState);
	inline	BeMain8V8State		Get8V8State();

private:
	int					m_iCampLastKiller[2];
	int					m_iCampMaxScore;
};

#define gMain		(*((BeMain *)pkAttachMain))
#define gUnit		(*((BeUnit *)pkAttachUnit))
#define gTime		(gMain.m_uiGameTime)
#define gRandNum	(*(gMain.m_pkRandNum))
#define gMap		(*(gMain.m_pkMap))
#define gData		(*(gMain.m_pkDataMgr))
#define gUnitMgr	(*(gMain.m_pkUnitMgr))
#define gEffectMgr	(*(gMain.m_pkEffectMgr))
#define gMapItemMgr (*(gMain.m_pkMapItemMgr))
#define gTrgMgr		(*(gMain.m_pkTriggerMgr))
#define gFormulaInfo (*(gMain.m_pkFormulaInfo))

#define gLevelMain	(*((BeLevelMain*)(gMain.m_pkTaskTriggerHandle)))
#define gLabelMgr	(*(gLevelMain.GetLabelMgr()))
#define gDataConvert (*(gLevelMain.GetDataConvertManager()))

inline	void		BeMain::SetCampLastKiller(int iCamp, int iUnitID)
{
	if (iCamp == 2 || iCamp == 1)
	{
		m_iCampLastKiller[iCamp - 1] = iUnitID;
	}
}
inline	int			BeMain::GetCampLastKiller(int iCamp)
{
	if (iCamp == 2 || iCamp == 1)
	{
		return m_iCampLastKiller[iCamp - 1];
	}
	return 0;
}
inline	void		BeMain::AddGameNoticeData(BeShareGameNotice& kData)
{
	m_kGameNoticeData.push_back(kData);
}
inline	float		BeMain::GetEquipAddArmor(int iPlayer)
{
	CHECK_IDX(iPlayer);
	return	m_akPlayer[iPlayer].fLevelAddArmor;
}
inline	float		BeMain::GetEquipAddMagicDamage(int iPlayer)
{
	CHECK_IDX(iPlayer);
	return	m_akPlayer[iPlayer].fLevelAddMagicDamage;
}
inline	float		BeMain::GetEquipAddMagicArmor(int iPlayer)
{
	CHECK_IDX(iPlayer);
	return	m_akPlayer[iPlayer].fLevelAddMagicArmor;
}
inline	std::map<int, std::vector<int>>& BeMain::GetSkillUserData()
{
	return m_kSkillUserData;
}
inline	void		BeMain::SetCampMaxScore(int iValue)
{
	m_iCampMaxScore = iValue;
}
inline	int			BeMain::GetCampMaxScore()
{
	return	m_iCampMaxScore;
}
inline	void		BeMain::Set8V8State(BeMain8V8State iState)
{
	m_e8V8State = iState;
}
inline	BeMain8V8State		BeMain::Get8V8State()
{
	return	m_e8V8State;
}
