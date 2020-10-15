#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <cstring>
#include "TW_Pos2.h"

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
