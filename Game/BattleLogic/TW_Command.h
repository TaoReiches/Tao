#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <cstring>
#include "TW_Pos2.h"
#include "TW_MemoryPool.h"

enum BeGuanQiaScene
{
	BGQS_SAVETOWN,
	BGQS_XIANCHENG,
	BGQS_SISHEN,
};

enum BeHeroShowCommand
{
	HSC_REFRESH_RUNE,
	HSC_KILL_SELF,
	HSC_RESUME,
	HSC_LVLUP,
	HSC_LVL_MAX,
	HSC_ADD_GOLD,
	HSC_CHANGESKILL,
	HSC_CHOOSE_HERO,
	HSC_CONVENTEQUIP,
	HSC_CHANGUNITTYPE,
	HSC_ADD_HP,
	HSC_DEC_HP,
	HSC_ADD_ARMOR,
	HSC_DEC_ARMOR,
	HSC_ADD_FK,
	HSC_DEC_FK,
	HSC_CLEAR,
	HSC_RESET,
	HSC_ADDHEROOWN,
	HSC_ADDHEROENEMY,
	HSC_MOREHERORESET,
	HSC_ENEMYHEROAI,
	HSC_ENEMYHERONOAI,
	HSC_MONSTERBOSSRESET,
	HSC_CHANGETIME,
	HSC_REFRESHSOLDIER,
	HSC_CREATESOLDIER,
	HSC_CLEARSOLDIER,
};

enum BeYuLeCommand
{
	YLC_AWARD_NONE = 0,
	YLC_AWARD_MANA,
	YLC_AWARD_ATTR,
	YLC_AWARD_MONEY,
	YLC_AWARD_LEVEL,
	YLC_AWARD_SKILL,
	YLC_AWARD_RANDOM,
	YLC_AWARD_RECOMMEND,

	YLC_AWARD_NUM,
};

enum BeGuoZhanAwardCommand
{
	GZAC_WISH1 = 0,
	GZAC_WISH2,
	GZAC_WISH3,
	GZAC_WISH4,
	GZAC_WISH5,
	GZAC_RANDOM,

	GZAC_AWARD_NUM,
};

enum BeGuoZhan_RoBot_AwardCommand
{
	GZRAC_T859 = 0,
	GZRAC_T860,
	GZRAC_T861,
	GZRAC_RANDOM,
};

enum BeGuanDuAwardCommand
{
	GDAC_AWARD_NONE = 0,
	GDAC_AWARD_HPREGEN,
	GDAC_AWARD_ATTACK,
	GDAC_AWARD_GOLD,
	GDAC_AWARD_CD,
	GDAC_AWARD_TP,
	GDAC_AWARD_RANDOM,
	GDAC_AWARD_RECOMMEOND,

	GDAC_AWARD_NUM,
};

enum BeChiBi3AwardCommand
{
	CB3AC_NONE = 0,
	CB3AC_MANA,
	CB3AC_TP,
	CB3AC_EYE,
	CB3AC_FARM_GOLD_PER,
	CB3AC_FARM_EXP_PER,

	CB3AC_RANDOM,
};

enum BePKAwardCommand
{
	BPAC_AWARD_NONE = 0,
	BPAC_AWARD_EQUIP,
	BPAC_AWARD_CD,
	BPAC_AWARD_HP,
	BPAC_AWARD_ATTACK,
	//BPAC_AWARD_SPEED,
	BPAC_AWARD_GOLD,
	BPAC_AWARD_RANDOM,

	BPAC_AWARD_NUM,
};

enum BePKCommand
{
	PKC_CHUZHAN = 0,
	PKC_YUANZHU,
	PKC_FANYIN,
};

enum BeFirstGameChoose
{
	BFGC_IS = 0,
	BFGC_NOT,
	BFGC_MAX,
};

enum BeBossFightAwardCommand
{
	BBFAC_BOSS_TIGAOSHENGMING = 0,
	BBFAC_BOSS_TIGAOHUIFULI,
	BBFAC_BOSS_TIGAOSHUXING,
	BBFAC_BOSS_TIGAOFANGYULI,
	BBFAC_BOSS_TIGAOJINENG,
	BBFAC_BOSS_TINGTIANYOUMING,

	BBFAC_PLAYER_CHENGWEIROUDUN,
	BBFAC_PLAYER_ZENGQIANGDIKANG,
	BBFAC_PLAYER_ZENGJIAJINQIAN,
	BBFAC_PLAYER_KUAISUJINENG,
	BBFAC_PLAYER_ZHIMINGFASHU,
	BBFAC_PLAYER_TINGTIANYOUMING,
};

enum BeSanGuoDaZhanAwardCommand
{
	BSGD_ATTACK = 0,
	BSGD_BEATBACK,
	BSGD_REGEN,
	BSGD_SPEED,
	BSGD_SPELL,

	BSGD_RANDOM,
	BSGD_RECOMMEND
};

enum BeDanLuanDouAwardCommand
{
	BDLD_Y552 = 0,
	BDLD_Y553,
	BDLD_Y554,
	BDLD_Y555,
	BDLD_Y556,
	BDLD_RANDOM,
};

enum BeDZMBAwardCommand
{
	BDZMB_Y567 = 0,
	BDZMB_Y568,
	BDZMB_Y569,
	BDZMB_Y570,
	BDZMB_Y571,
	BDZMB_RANDOM,
};

enum BeGouZiDaZhanAwardCommand
{
	GZDZ_SKILLPOS1 = 0,
	GZDZ_SKILLPOS2,
	GZDZ_SKILLPOS3,
	GZDZ_SKILLPOS4,
	GZDZ_RANDOM,
};

enum BeGouZiDaZhanOtherAwardCommand
{
	GZDZO_SKILL1 = 0,
	GZDZO_SKILL2,
	GZDZO_RANDOM,
};

enum BeGiveCmdType
{
	BCT_NONE = 0,

	BCT_SYSTEM_SHIFT,
	BCT_PLAYER_SHIFT,
	BCT_PLAYER_REDUCE,
	BCT_PLAYER_CMD,
	BCT_PLAYER_INSERT,
	BCT_PLAYER_ACTION,
	BCT_PLAYER_HUNG,
	BCT_SKILL_ADDON,
	BCT_DIZZY_INTERRUPT,
	BCT_DEATH,
	BCT_SYSTEM_CLEAR,

	BCT_IMMEDIATE = BCT_PLAYER_ACTION,
	BCT_NEXT_EXE = BCT_PLAYER_HUNG,
	BCT_INTERRUPT_CURRENT = BCT_DIZZY_INTERRUPT,
	BCT_HUNG_CURRENT = BCT_PLAYER_INSERT,
	BCT_PUSH_BACK = BCT_PLAYER_SHIFT,
};

enum BeCommandType
{
	BCT_STOP = 0,
	BCT_HOLD,
	BCT_MOVE,
	BCT_PATROL,
	BCT_ATTACK,
	BCT_SPELL,
	BCT_MOVE_DIRECT,

	BCT_USE_ITEM,
	BCT_DROP_ITEM,
	BCT_PICK_ITEM,

	BCT_NUMS,
};

enum BeCommandTargetType
{
	BCTT_NONE = 0,
	BCTT_MAP = 1,
	BCTT_UNIT = 2,
	BCTT_MAPITEM,

	BCTT_EFFECT,
	BCTT_CMD,
};

class BeCommand
{
public:
	BeCommand(BeCommandType cmd = BCT_STOP, int unit = 0,
		float x = 0.0f, float y = 0.0f, int data = 0,
		bool bShiftDelete = false, int data2 = 0,
		BeCommandTargetType eTType = BCTT_UNIT,
		bool bForceAttackSG = false,
		float fDirX = 0.0f, float fDirY = 0.0f);

	BeCommandType	eCmdType;
	int				iUnitID;
	TePos2			kPos;
	TePos2			kDirPos;
	int				iData;
	int				iData2;
	bool			bForceAttackOnce;
	bool			bShiftCanDelete;
	BeCommandTargetType		eTargetType;
	BeGiveCmdType			eGiveType;
	bool			bForceExecute;
};

struct BeSwapItemData
{
	BeSwapItemData(int iBeginPos = 0, int iEndPos = 0)
	{
		iBPos = iBeginPos;
		iEPos = iEndPos;
	}
	int iBPos;
	int iEPos;
};

struct BeBuyItemData
{
	BeBuyItemData(int iUnit = 0, int iPos = 0, int iPlay = -1, int iBuyer = 0, int iLevelUpID = 0)
	{
		iUnitID = iUnit;
		iItemPos = iPos;
		iPlayer = iPlay;
		iBuyerID = iBuyer;
		iLevelUpItemID = iLevelUpID;
	}
	int	iUnitID;
	int iItemPos;
	int iPlayer;
	int iBuyerID;
	int iLevelUpItemID;
};

struct  BeLearnSkill
{
	BeLearnSkill(int iPos = 0, int iLearnID = 0)
	{
		iLearnSkillPos = iPos;
		iLearnSkillID = iLearnID;
	}

	int iLearnSkillPos;
	int iLearnSkillID;
};

struct BeActiveSkill
{
	BeActiveSkill(int iTypeID = 0, bool bAct = false)
	{
		iSkillTypeID = iTypeID;
		bActive = bAct;
	}
	int iSkillTypeID;
	bool bActive;
};

struct BeReliveHero
{
	BeReliveHero(int iID = 0)
	{
		iHeroID = iID;
	}
	int iHeroID;
};

struct BeLeaveGame
{
	BeLeaveGame(int iLeavePlayer = 0)
	{
		iLeavePlayerIdx = iLeavePlayer;
	}
	int iLeavePlayerIdx;
};

struct BeMapMessage
{
	BeMapMessage(int iGroup = -1, float fX = 0.0f, float fY = 0.0f, int iTargetID = 0)
	{
		iPlayGroup = iGroup;
		fTargetX = fX;
		fTargetY = fY;
		iTargetUnitID = iTargetID;
	}
	int iPlayGroup;
	float fTargetX;
	float fTargetY;
	int iTargetUnitID;
};

struct BeUnitSurrender
{
	BeUnitSurrender(bool bFirst = false, bool bAgree = false, int iPlay = 0)
	{
		bFirstSurrender = bFirst;
		bAgreeSurrender = bAgree;
		iPlayer = iPlay;
	}
	bool bFirstSurrender;
	bool bAgreeSurrender;
	int iPlayer;
};

struct BeUnitSuspendPlayer
{
	BeUnitSuspendPlayer(bool bSuspend = false, bool bAgree = false, int iPlay = 0)
	{
		bSuspendPlayer = bSuspend;
		bAgreeKickPlayer = bAgree;
		iPlayer = iPlay;
	}
	bool bSuspendPlayer;
	bool bAgreeKickPlayer;
	int iPlayer;
};

struct BeSelectHero
{
	BeSelectHero(int iTypeID = 0, int iPlay = -1)
	{
		iUnitTypeID = iTypeID;
		iPlayer = iPlay;
	}
	int iUnitTypeID;
	int iPlayer;
};

struct BeCheatCommand
{
	BeCheatCommand(float fData = 0.0f, int iData = 0, int iPlayer = -1)
	{
		std::memset(acCheatCommand, 0, sizeof(acCheatCommand));
		fCheatData = fData;
		iCheatData = iData;
		iCheatPlayer = iPlayer;
	}
	char acCheatCommand[64];
	float fCheatData;
	int iCheatData;
	int iCheatPlayer;
};

struct BeGuanQiaCommand
{
	BeGuanQiaCommand(unsigned int dwCmd = 0, int iPlayer = -1, float fNew1 = 0.0f, float fNew2 = 0.0f)
	{
		memset(acGuanQiaCommand, 0, sizeof(acGuanQiaCommand));
		dwGuanQiaCmd = dwCmd;
		iGuanQiaPlayer = iPlayer;
		fData1 = fNew1;
		fData2 = fNew2;
	}
	char acGuanQiaCommand[256];
	unsigned int dwGuanQiaCmd;
	int iGuanQiaPlayer;
	float fData1;
	float fData2;
};

struct BeTownItemCommand
{
	BeTownItemCommand(int iPlayer, unsigned int dwItemTypeID, unsigned int dwUnitID, unsigned int dwData)
	{
		bSuccess = false;
		iPlayerIdx = iPlayer;
		dwTownItemTypeID = dwItemTypeID;
		dwTargetUnitID = dwUnitID;
		dwOtherData = dwData;
	}
	bool bSuccess;
	int	iPlayerIdx;
	unsigned int dwTownItemTypeID;
	unsigned int dwTargetUnitID;
	unsigned int dwOtherData;
};

struct BeTownItemStack
{
	BeTownItemStack()
	{
		bActive = false;
		iPlayer = 0;
		iTownItemID = 0;
		iItemTypeID = 0;
		iOwnerPlayer = 0;
		dwShopID = 0;
		dwBuyerID = 0;
		dwTargetBuyerID = 0;
	}
	bool bActive;
	int iPlayer;
	int iTownItemID;
	int iItemTypeID;
	int iOwnerPlayer;
	unsigned int dwShopID;
	unsigned int dwBuyerID;
	unsigned int dwTargetBuyerID;
};

class BeExeCommand : public BeMainPtr, public BeUnitPtr
{
public:
	BeExeCommand();
	virtual ~BeExeCommand();

	BeCommandType GetType() const;
	BeTaskType GetTaskType() const;

	virtual BeExeResult Execute(int& iDeltaTime) = 0;
	virtual bool CanInterrupt() const = 0;
	virtual bool CanCancel() const = 0;
	virtual bool CanHungUp(BeGiveCmdType eCmdType = BCT_IMMEDIATE, bool bNeedHangCurrent = true) const = 0;
	virtual bool CanSkip(void) const;
	void SafeDeleteTask(BeTask*& pkTask);

	void AttachMain(void* pkMain)
	{
		pkAttachMain = pkMain;
		if (m_pkCurTask)
		{
			m_pkCurTask->AttachMain(pkMain);
		}
	}

	void AttachUnit(void* pkUnit)
	{
		pkAttachUnit = pkUnit;
		if (m_pkCurTask)
		{
			m_pkCurTask->AttachUnit(pkUnit);
		}
	}

protected:
	BeCommandType	m_eCmdType;
	BeTask* m_pkCurTask;
};

class BeStopCommand : public BeExeCommand
{
protected:
	BeStopCommand();
	~BeStopCommand() {};

public:
	bool IsDead();
	void SetDead();
	void SetStopTime(int iTime)
	{
		m_iStopTime = iTime;
	}

	virtual BeExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(BeGiveCmdType eCmdType = BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;

protected:
	bool	m_bDead;
	int		m_iStopTime;
};

class BeHoldCommand : public BeExeCommand
{
protected:
	BeHoldCommand();
	~BeHoldCommand() {};

public:
	virtual BeExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(BeGiveCmdType eCmdType = BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;
protected:
	bool FindAttack(void);
};

class BeMoveCommand : public BeExeCommand
{
protected:
	BeMoveCommand();
	~BeMoveCommand() {};
public:
	void SetFollowID(int iID, float fDistance);
	void SetTargetPos(const TePos2& kPos, float fDistance, bool bTurn);
	TePos2 GetTargetPos() const;

	virtual BeExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(BeGiveCmdType eCmdType = BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;
};

class BePatrolCommand : public BeExeCommand
{
protected:
	BePatrolCommand();
	~BePatrolCommand() {};

public:
	void SetPatrol(const TePos2& kTargetPos, const TePos2& kNextPos);

	virtual BeExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(BeGiveCmdType eCmdType = BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;
	void SetStopTime(int iTime)
	{
		m_iStopTime = iTime;
	}

protected:
	TePos2	m_kTargetPos;
	TePos2	m_kNextPos;
	int			m_iCurStopTime;
	int			m_iStopTime;
};

class BeAttackCommand : public BeExeCommand
{
protected:
	BeAttackCommand();
	~BeAttackCommand() {};

public:
	void SetTargetID(int iID, float fDistance, bool bIsOrb = false, int iSkillTypeID = 0, int iSkillLevel = 0, int iAttackCountLimit = -1);
	void SetTargetPos(const TePos2& kPos, float fRange = 0.0f);
	void SetTargetItem(int iItemID);

	virtual BeExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(BeGiveCmdType eCmdType = BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;
	virtual bool CanSkip(void) const;
};

class BeSpellCommand : public BeExeCommand
{
protected:
	BeSpellCommand();
	~BeSpellCommand() {};

public:
	void SpellTargetID(int iSkillTypeID, int iTargetID, const TePos2& kPos, int iSkillLevel = 1, int iItemID = 0, bool bExpendMP = true, int iUsePlayer = -1, int iTargetType = 0);
	void SpellTargetItem(int iSkillTypeID, int iTargetID, int iSkillLevel = 1, int iItemID = 0, bool bExpendMP = true, int iUsePlayer = -1);
	void SpellTargetPos(int iSkillTypeID, const TePos2& kPos, const TePos2& kDirPos, int iSkillLevel = 1, int iItemID = 0, bool bExpendMP = true, int iUsePlayer = -1);

	virtual BeExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(BeGiveCmdType eCmdType = BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;
	virtual bool CanSkip(void) const;

protected:
	int 	m_iSkillTypeID;
	int		m_iSkillLevel;
	bool	m_bExpendMP;
	int		m_iTargetID;
	int		m_iTargetType;
	int     m_iItemID;
	int		m_iUsePlayer;
	TePos2	m_kTargetPos;
	TePos2	m_kDirectPos;
};

class BeDropItemCommand : public BeExeCommand
{
protected:
	BeDropItemCommand();
	~BeDropItemCommand() {};

public:
	void SetTargetIDDropItem(int iID, int iItemID);
	void SetTargetPosDropItem(const TePos2& kPos, int iItemID);

	virtual BeExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(BeGiveCmdType eCmdType = BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;

protected:
	int		m_iTargetID;
	int     m_iItemID;
	TePos2	m_kTargetPos;
};

class BePickItemCommand : public BeExeCommand
{
protected:
	BePickItemCommand();
	~BePickItemCommand() {};

public:
	bool SetItemID(int iItemID);

	virtual BeExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(BeGiveCmdType eCmdType = BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;

protected:
	int		m_iItemID;
	TePos2	m_kTargetPos;
};
