#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <map>
#include <vector>
#include "TW_MainDefine.h"
#include "TW_TriggerFunc.h"
#include "TW_CommandProc.h"

#define GAME_FRAME_TIME	20

class BeUnit;
class BePlayerInfo;
class BeFormulaInfo;
class BeBuffer;
class BeEffect;
class BeItem;
class BeMapItem;
class BeAttackingAttr;
class SeRaceCmd;
class BeMapItemMgr;
class TeMap;
class BeUnitMgr;
class BeEffectMgr;
class ITwRandom;
class BeSkill;
class BeCarry;
class SkillTable;

class   BeMain : public TwTriggerFunc, public TwCommandProc
{
public:
	BeMain(void);
	~BeMain(void);
private:
	BeMain* pkAttachMain;

public:
	void SetPlayerInfo(int iIdx, int iID, int iHeroID, const char* acName);

	bool LoadRes(int iSeed);

	inline unsigned int GetFrameCount(void)
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

	inline int GetGameTime(void) const
	{
		return (int)m_uiGameTime;
	}

	inline void ForceSetGameTime(unsigned int iTime)
	{
		m_uiGameTime = iTime;
	}

	inline 	int GetGameStartTime(void) const
	{
		return 0;
	}

protected:
	bool PlayerSendCommand(BeUnit* pkUnit, int iSendIdx);

public:
	bool CreateHero(int iPlayer = -1);

public:
	void	UpdateRaceOut();
	int		GetUnitRaceOut(int iSeatIndex, int iOffset, char* pcData, int iSize);
	int		GetSvrModelRaceOut(int iOffset, char* pcData, int iSize);

	void	InitServerMode();
	void	FinializeServerMode();

protected:
	int								m_iChangeFlag;

private:
	int		m_iMaxPerLoadID;
	bool	m_bReLink;

public:

	bool Initialize(void);
	void Finialize(void);
	bool UpdateFrame(unsigned int dwFrame);

	bool	InitializeGame(void);
	void	InitPlayerGroup(void);
	void	ResetData(void);
	void	SetGameFrame(unsigned int dwFrame);

	int GetSkillOrgTypeID(int iSkillTypeID);

	void	SetGameBeginTime(int iValue)
	{
		m_iGameBeginTime = iValue;
	}
	int		GetGameBeginTime()
	{
		return m_iGameBeginTime;
	}

protected:
	void			ReleaseModule(void);
	void			UpdatePlayerLeave();

public:
    unsigned int            m_uiGameTime;
    ITwRandom*              m_pkRandNum;
    TeMap*                  m_pkMap;
    BeUnitMgr*              m_pkUnitMgr;
    BeEffectMgr*            m_pkEffectMgr;
    BeMapItemMgr*           m_pkMapItemMgr;
    BeFormulaInfo*          m_pkFormulaInfo;

protected:
    std::map<unsigned int, BePlayerInfo*>   m_akPlayer;
    BeMainState             m_eState;
    unsigned int            m_uiFrameCount;
    unsigned int            m_uiRealTimeNow;
    int                     m_aiGenID[GIT_MAX];
    std::vector<int>        m_aiVecHeroTypeID;
    unsigned int            m_iCurMapID;
    BeRaceGameModel         m_eRaceGameModel;
    int                     m_iGameBeginTime;
};

#define gMain           (*((BeMain *)pkAttachMain))
#define gUnit           (*((BeUnit *)pkAttachUnit))
#define gTime           (gMain.m_uiGameTime)
#define gRandNum        (*(gMain.m_pkRandNum))
#define gMap            (*(gMain.m_pkMap))
#define gData           (*(gMain.m_pkDataMgr))
#define gUnitMgr        (*(gMain.m_pkUnitMgr))
#define gEffectMgr      (*(gMain.m_pkEffectMgr))
#define gMapItemMgr     (*(gMain.m_pkMapItemMgr))
#define gTrgMgr         (*(gMain.m_pkTriggerMgr))
#define gFormulaInfo    (*(gMain.m_pkFormulaInfo))
