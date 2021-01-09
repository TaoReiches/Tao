#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <map>
#include <vector>
#include <memory>
#include "TW_MainDefine.h"
#include "TW_TriggerFunc.h"
#include "TW_CommandProc.h"

#define GAME_FRAME_TIME	20

class BeMapItemMgr;
class TeMap;
class TwUnitMgr;
class BeEffectMgr;
class ITwRandom;
class BeFormulaInfo;
class TwPlayerInfo;

class TwMain final : public TwTriggerFunc, public TwCommandProc
{
public:
	TwMain(void);
	~TwMain(void);
private:
	TwMain* pkAttachMain;

public:
	inline unsigned int GetFrameCount(void)
	{
		return m_uiFrameCount;
	}

	inline void BeginRace(void)
	{
		m_eState = BeMainState::BMS_RACING;
	}

	inline int GenerateID(BeGenIDType eType)
	{
		if (eType < BeGenIDType::GIT_ENTITY || eType >= BeGenIDType::GIT_MAX)
		{
			eType = BeGenIDType::GIT_ENTITY;
		}

		return ++m_aiGenID[static_cast<int>(eType)];
	}

	inline int SetGenerateID(BeGenIDType eType, int iID)
	{
		if (eType < BeGenIDType::GIT_ENTITY || eType >= BeGenIDType::GIT_MAX)
		{
			return 0;
		}

		return m_aiGenID[static_cast<int>(eType)] = iID;
	}

	inline void SetState(BeMainState eState)
	{
		if (eState == BeMainState::BMS_OVER)
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
	bool PlayerSendCommand(std::shared_ptr<TwUnit> pkUnit, int iSendIdx);

public:
	void	UpdateRaceOut();
	int		GetUnitRaceOut(int iSeatIndex, int iOffset, char* pcData, int iSize);
	int		GetSvrModelRaceOut(int iOffset, char* pcData, int iSize);

	void	InitServerMode();
	void	FinializeServerMode();

    bool SetPlayerInfo(const std::shared_ptr<TwPlayerInfo>& playerInfo);
    const std::shared_ptr<TwPlayerInfo> GetPlayerInfo(std::uint64_t playerId) const
    {
        const auto& result = m_akPlayer.find(playerId);
        if (result != m_akPlayer.end())
        {
            return result->second;
        }
        return nullptr;
    }

    bool Initialize(int iSeed);
    void Finialize(void);
    bool UpdateFrame(unsigned int dwFrame);

	int GetSkillOrgTypeID(int iSkillTypeID);

	void	SetGameBeginTime(int iValue)
	{
		m_iGameBeginTime = iValue;
	}
	int		GetGameBeginTime()
	{
		return m_iGameBeginTime;
	}

public:
    unsigned int            m_uiGameTime;
    ITwRandom*              m_pkRandNum;
    TeMap*                  m_pkMap;
    std::unique_ptr<TwUnitMgr>              m_pkUnitMgr;
    BeEffectMgr*            m_pkEffectMgr;
    BeMapItemMgr*           m_pkMapItemMgr;
    BeFormulaInfo*          m_pkFormulaInfo;

protected:
    std::map<std::uint64_t, const std::shared_ptr<TwPlayerInfo>>   m_akPlayer;
    BeMainState             m_eState;
    unsigned int            m_uiFrameCount;
    unsigned int            m_uiRealTimeNow;
    int                     m_aiGenID[static_cast<int>(BeGenIDType::GIT_MAX)];
    std::vector<int>        m_aiVecHeroTypeID;
    unsigned int            m_iCurMapID;
    BeRaceGameModel         m_eRaceGameModel;
    int                     m_iGameBeginTime;
};

#define gMain           pkAttachMain
#define gUnit           pkAttachUnit
#define gTime           (gMain->m_uiGameTime)
#define gRandNum        (*(gMain->m_pkRandNum))
#define gMap            (*(gMain->m_pkMap))
#define gData           (*(gMain->m_pkDataMgr))
#define gUnitMgr        (gMain->m_pkUnitMgr)
#define gEffectMgr      (*(gMain->m_pkEffectMgr))
#define gMapItemMgr     (*(gMain->m_pkMapItemMgr))
#define gTrgMgr         (*(gMain->m_pkTriggerMgr))
#define gFormulaInfo    (*(gMain->m_pkFormulaInfo))
