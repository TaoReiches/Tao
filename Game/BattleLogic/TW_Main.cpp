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

#define GAME_FRAME_TIME	20

BeMain::BeMain(void)
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

BeMain::~BeMain(void)
{
	Finialize();
	ReleaseModule();
}

void BeMain::ResetData(void)
{
	m_eState = BMS_NONE;
	m_uiGameTime = 0;

	m_uiFrameCount = 0;
	m_iGameTimeLeft = 0;
	m_uiRealTimeNow = 0;

	m_bShowAllDamage = false;
	memset(m_aiGenID, 0, sizeof(m_aiGenID));

	m_iOverTime = 90;
	m_bGameValid = true;

	m_iChangeFlag = 0;
	m_iMaxPerLoadID = 0;
	m_bReLink = false;
	FinializeServerMode();

	memset(m_akEntityPointer, 0, sizeof(m_akEntityPointer));
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
}

void BeMain::InitPlayerGroup()
{
	bool bMoreCamp = false;
}

bool BeMain::InitializeGame(void)
{
	if (!m_pkTriggerMgr)
	{
		return false;
	}

	return true;
}

bool BeMain::Initialize(void)
{
	m_eState = BMS_NONE;
	m_uiGameTime = 0;

	m_uiFrameCount = 0;
	m_iGameTimeLeft = 0;
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

	if (!m_pkTriggerMgr->Initialize(BTE_MAX_NUM))
	{
		return false;
	}

	InitPlayerGroup();


	m_eState = BMS_INITED;
	m_eRaceGameModel = BRGM_NORMAL;

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

	if (m_pkMap)
	{
		m_pkMap->Finialize();
	}

	ResetData();
}

void	BeMain::UpdatePlayerLeave()
{

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

		UpdatePlayerLeave();

		m_uiGameTime += GAME_FRAME_TIME;
		m_uiFrameCount++;
	}
	return true;
}

bool BeMain::LoadRes(int iSeed)
{
	ReleaseModule();

	m_pkRandNum = NewRandom();
	m_pkRandNum->Initialize(iSeed);

	m_pkTriggerMgr = new TePtTriggerMgr(this);
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

void BeMain::SetGameFrame(unsigned int dwFrame)
{
	m_uiFrameCount = dwFrame;
	m_uiGameTime = GAME_FRAME_TIME * m_uiFrameCount;
}

int BeMain::GetSkillOrgTypeID(int iSkillTypeID)
{
	const SkillTable* pkSkillRes = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID);
	if (pkSkillRes && pkSkillRes->uiOriginTypeID == (unsigned int)-1)
	{
		return iSkillTypeID;
	}

	return pkSkillRes->uiOriginTypeID;
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

	// AddWindowData(kData);
}

void	BeMain::AddUIMessage(int iUnitID, int iType, int iStrIndex, int iTime)
{
	BeShareUIWindowData	kData;

	kData.eWindowType = (BeUiWindowType)iType;
	kData.iUnitID = iUnitID;
	kData.iResID = iStrIndex;
	kData.fValue1 = iTime;
	kData.fValue4 = 502;

	// AddWindowData(kData);
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

	// AddWindowData(kData);
}

void BeMain::SetHeroDataChangeFlag(int iSeat, int iFlag)
{
	//if (iSeat < 0 || iSeat >= MAX_ACTIVEPLAYERS)
	//{
	//	return;
	//}
	//m_aiHeroDataChangeFlag[iSeat] |= iFlag;
}

int BeMain::GetHeroDataChangeFlag(int iSeat)
{
	//if (iSeat < 0 || iSeat >= MAX_ACTIVEPLAYERS)
	//{
	//	return 0;
	//}
	//return m_aiHeroDataChangeFlag[iSeat];
	return 0;
}

void BeMain::ClrHeroDataChangeFlag(int iSeat, int iFlag)
{
	//if (iSeat < 0 || iSeat >= MAX_ACTIVEPLAYERS)
	//{
	//	return;
	//}
	//m_aiHeroDataChangeFlag[iSeat] &= ~iFlag;
}

bool BeMain::HasHeroDataChangeFlag(int iSeat, int iFlag)
{
	//if (iSeat < 0 || iSeat >= MAX_ACTIVEPLAYERS)
	//{
	//	return false;
	//}

	//int iHeroDataChangeFlag = m_aiHeroDataChangeFlag[iSeat];
	//return ((iHeroDataChangeFlag & iFlag) == iFlag ? true : false);
	return false;
}

bool	BeMain::HasGrassVisionForCamp(int iGrassIndex, int iSrcCamp, int iDstCamp)
{
	//auto	Iter = m_kGrassUnit.begin();
	//while (Iter != m_kGrassUnit.end())
	//{
	//	if (Iter->second == iGrassIndex)
	//	{
	//		BeUnit* pkUnit = gUnitMgr.GetUnitByID(Iter->first);
	//		if (pkUnit)
	//		{
	//			if (pkUnit->GetCamp() == iDstCamp)
	//			{
	//				return true;
	//			}
	//		}
	//	}
	//	++Iter;
	//}
	return false;
}

bool	BeMain::HasGrassVision(int iSrcUnitID, int iTarUnitID)
{
	//std::map<int, int>::iterator iter = m_kGrassUnit.find(iTarUnitID);
	//if (iter == m_kGrassUnit.end())
	//{
	//	return true;
	//}
	//int iTarGrassIndex = iter->second;
	//std::map<int, int>::iterator iterSrc = m_kGrassUnit.begin();

	//int		iSrcCamp = -1;
	//BeUnit* pkSrcUnit = gUnitMgr.GetUnitByID(iSrcUnitID);
	//if (pkSrcUnit)
	//{
	//	iSrcCamp = pkSrcUnit->GetCamp();
	//}
	//int		iTarCamp = -1;
	//BeUnit* pkTarUnit = gUnitMgr.GetUnitByID(iTarUnitID);
	//if (pkTarUnit)
	//{
	//	iTarCamp = pkTarUnit->GetCamp();
	//}
	//if (iTarCamp == iSrcCamp)
	//{
	//	return true;
	//}

	//for (; iterSrc != m_kGrassUnit.end(); iterSrc++)
	//{
	//	int iGrassUnitID = iterSrc->first;
	//	BeUnit* pkGrassUnit = gUnitMgr.GetUnitByID(iGrassUnitID);
	//	if (pkGrassUnit)
	//	{
	//		if (pkGrassUnit->GetCamp() == iSrcCamp)
	//		{
	//			if (iterSrc->second == iTarGrassIndex)
	//			{
	//				return true;
	//			}
	//		}
	//	}
	//}

	return false;
}
