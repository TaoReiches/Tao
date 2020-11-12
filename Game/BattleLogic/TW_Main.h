#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <map>
#include "TW_MainDefine.h"
#include "TW_TriggerMgr.h"

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
class ITeRandom;
class BeSkill;
class BeCarry;
class SkillTable;

class   BeMain
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
	bool DefaultSkill_ActionParam(BeUnit*& pkTrgUnit, unsigned int& uiSkillTypeID, int& iSkillLevel, const SkillTable* pkSkillRes);

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
	bool Proc_START(const SeRaceCmd* pkRaceCmd);
	bool Proc_END(const SeRaceCmd* pkRaceCmd);
	bool Proc_CHART_PLAYER(const SeRaceCmd* pkRaceCmd);
	bool Proc_PLAYER_LEAVE(const SeRaceCmd* pkRaceCmd);
	bool Proc_SYSTEM_CHEAT(const SeRaceCmd* pkRaceCmd);
	bool Proc_HERO_MOVE(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_MOVE(const SeRaceCmd* pkRaceCmd);
	bool Proc_HERO_STOP(const SeRaceCmd* pkRaceCmd);
	bool Proc_HERO_ATTACK_POS(const SeRaceCmd* pkRaceCmd);
	bool Proc_HERO_ATTACK_UNIT(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_SPELL(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_LEARN_SKILL(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_ACTIVE_SKILL(const SeRaceCmd* pkRaceCmd);
	bool Proc_PLAYER_RELIVEHERO(const SeRaceCmd* pkRaceCmd);
	bool Proc_PLAYER_MAP_MESSAGE(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_DISPLACE_ITEM(const SeRaceCmd* pkRaceCmd);
	bool Proc_UNIT_CHANGE_ITEM(const SeRaceCmd* pkRaceCmd);
	bool Proc_Show_Action(const SeRaceCmd* pkRaceCmd);

	bool	Proc_HERO_DIRECT_MOVE(const SeRaceCmd* pkRaceCmd);
	bool	Proc_HERO_ATTACK_CANCEL(const SeRaceCmd* pkRaceCmd);

	bool Initialize(void);
	void Finialize(void);
	bool UpdateFrame(unsigned int dwFrame);

	bool	InitializeGame(void);
	void	InitPlayerGroup(void);
	void	ResetData(void);
	void	SetGameFrame(unsigned int dwFrame);

	TePtCondResult DefaultSkill_Condition(int iOriginTypeID);

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
    ITeRandom*              m_pkRandNum;
    TeMap*                  m_pkMap;
    BeUnitMgr*              m_pkUnitMgr;
    BeEffectMgr*            m_pkEffectMgr;
    BeMapItemMgr*           m_pkMapItemMgr;
    TePtTriggerMgr*         m_pkTriggerMgr;
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
