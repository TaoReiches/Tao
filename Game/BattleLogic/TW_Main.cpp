/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Main.h"
#include "TW_Define.h"
#include "TW_IRandom.h"
#include "TW_UnitMgr.h"
#include "TW_EffectMgr.h"
#include "TW_MapItemMgr.h"
#include "TW_Map.h"
#include "TW_ShareUnitData.h"
#include "TW_TriggerEvent.h"
#include "TW_FormulaInfo.h"
#include "Skill_table.hpp"
#include "TW_TriggerMgr.h"

TwMain::TwMain(void)
{
	pkAttachMain = this;

	m_pkRandNum = nullptr;
	m_pkMap = nullptr;
	m_pkUnitMgr = nullptr;
	m_pkEffectMgr = nullptr;
	m_pkMapItemMgr = nullptr;
	m_pkTriggerMgr = nullptr;
	m_pkFormulaInfo = nullptr;
	m_iGameBeginTime = 0;

	ResetData();

	m_iCurMapID = 0;
}

TwMain::~TwMain(void)
{
	Finialize();
	ReleaseModule();
}

void TwMain::ResetData(void)
{
	m_eState = BeMainState::BMS_NONE;
	m_uiGameTime = 0;
	m_uiFrameCount = 0;
	m_uiRealTimeNow = 0;
	memset(m_aiGenID, 0, sizeof(m_aiGenID));
	m_iChangeFlag = 0;
	m_iMaxPerLoadID = 0;
	m_bReLink = false;
	FinializeServerMode();
}

void TwMain::ReleaseModule(void)
{
	SAFE_DELETE(m_pkMapItemMgr);
	SAFE_DELETE(m_pkEffectMgr);
	SAFE_DELETE(m_pkUnitMgr);
	SAFE_DELETE(m_pkMap);
	SAFE_DELETE(m_pkTriggerMgr);
	SAFE_DELETE(m_pkRandNum);
	SAFE_DELETE(m_pkFormulaInfo);
}

void TwMain::InitPlayerGroup()
{
	bool bMoreCamp = false;
}

bool TwMain::InitializeGame(void)
{
	if (!m_pkTriggerMgr)
	{
		return false;
	}

	return true;
}

bool TwMain::Initialize(void)
{
	m_eState = BeMainState::BMS_NONE;
	m_uiGameTime = 0;

	m_uiFrameCount = 0;
	m_uiRealTimeNow = 0;

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

	if (!m_pkTriggerMgr->Initialize(static_cast<int>(TwTriggerEvent::BTE_MAX_NUM)))
	{
		return false;
	}

	InitPlayerGroup();


	m_eState = BeMainState::BMS_INITED;
	m_eRaceGameModel = BeRaceGameModel::BRGM_NORMAL;

	return true;
}

void TwMain::Finialize(void)
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

	if (m_pkMap)
	{
		m_pkMap->Finialize();
	}

	ResetData();
}

void	TwMain::UpdatePlayerLeave()
{

}

bool	TwMain::UpdateFrame(unsigned int dwFrame)
{
	TwMain* pkAttachMain = this;

	while (m_uiFrameCount < dwFrame)
	{
		m_pkTriggerMgr->Update(GAME_FRAME_TIME);
		m_pkEffectMgr->Update(GAME_FRAME_TIME);
		m_pkUnitMgr->Update(GAME_FRAME_TIME);
		m_pkMapItemMgr->Update(GAME_FRAME_TIME);
		m_pkMap->Update(GAME_FRAME_TIME);

		UpdatePlayerLeave();

		m_uiGameTime += GAME_FRAME_TIME;
		m_uiFrameCount++;
	}
	return true;
}

bool TwMain::LoadRes(int iSeed)
{
	ReleaseModule();

	m_pkRandNum = NewRandom();
	m_pkRandNum->Initialize(iSeed);

	m_pkTriggerMgr = new TwPtTriggerMgr(this);
	m_pkEffectMgr = new BeEffectMgr;
	m_pkEffectMgr->AttachMain(this);

	m_pkMapItemMgr = new BeMapItemMgr;
	m_pkMapItemMgr->AttachMain(this);

	m_pkUnitMgr = new BeUnitMgr;
	m_pkUnitMgr->AttachMain(this);

	m_pkFormulaInfo = new BeFormulaInfo;
	m_pkFormulaInfo->AttachMain(this);

	m_pkMap = new TeMap;
	m_pkMap->AttachMain(this);
	if (!m_pkMap->Initialize())
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

void TwMain::SetGameFrame(unsigned int dwFrame)
{
	m_uiFrameCount = dwFrame;
	m_uiGameTime = GAME_FRAME_TIME * m_uiFrameCount;
}

int TwMain::GetSkillOrgTypeID(int iSkillTypeID)
{
	auto& pkSkillRes = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID);
	if (pkSkillRes && pkSkillRes->uiOriginTypeID == (unsigned int)-1)
	{
		return iSkillTypeID;
	}

	return pkSkillRes->uiOriginTypeID;
}

void TwMain::SetPlayerInfo(int iIdx, int iID, int iHeroID, const char* acName)
{

}

bool TwMain::CreateHero(int iPlayer)
{
    return true;
}
