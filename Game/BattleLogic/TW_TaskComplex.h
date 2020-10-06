#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TaskAction.h"
#include "TW_MapDefine.h"

enum BeMoveRetryState
{
	BMRS_DITECT = 0,
	BMRS_MIDDLE,
	BMRS_FINNAL,
	BMRS_CHECK,
};

class BeTaskMoveToPos : public BeTask
{
	DECLARE_POOL(BeTaskMoveToPos);

protected:
	BeTaskMoveToPos();
	~BeTaskMoveToPos();

public:
	BeMoveResult GetMoveResult(void) const;
	void SetMoveResult(BeMoveResult eResult);
	void SetTargetPos(const TePos2& kPos, float fDistance = 0.0f, bool bTurn = false);
	TePos2 GetTargetPos() const;

	virtual BeExeResult Execute(int& iDeltaTime);

protected:
	BeMoveState			m_eMoveState;
	int					m_iStandTime;
	int					m_iWalkBlockTime;
	int 				m_iRetryTime;

	BeMoveResult		m_eState;
	BeMoveRetryState	m_eRetryState;

	TeFindResult	m_eFindPathRet;

	TePos2				m_kTarPos;
	TePos2				m_kMiddlePos;
	TePos2				m_kDirectPos;
	bool				m_bSoliderCheck;
	float				m_fDistance;

	BeTaskActionWalk* m_pkWalk;
};

class BeTaskMoveToUnit : public BeTask
{
	DECLARE_POOL(BeTaskMoveToUnit);

protected:
	BeTaskMoveToUnit();
	~BeTaskMoveToUnit();

public:
	BeMoveResult GetMoveResult(void);
	void SetTargetID(int iID, float fDistance = 0.0f);
	int GetTargetID() const;

	virtual BeExeResult Execute(int& iDeltaTime);

protected:
	int					m_iTargetID;
	BeTaskMoveToPos* m_pkMoveToPos;
	float				m_fDistance;
};

enum BeAttackUnitState
{
	BAU_ATTACK = 0,
	BAU_REFRESH,
	BAU_TRACE,
	BAU_END,
};

class BeTaskAttackUnit : public BeTask
{
	DECLARE_POOL(BeTaskAttackUnit);

protected:
	BeTaskAttackUnit();
	~BeTaskAttackUnit();

public:
	void SetTargetID(int iID, float fDistance = 0.0f, bool bIsOrb = false, int iSkillTypeID = 0, int iSkillLevel = 0, int iAttackCountLimit = -1);
	int GetTargetID() const;
	bool IsAttacking() const;
	bool IsDamaged() const;
	bool IsCanCancel() const;
	bool CanSkip(void) const;

	virtual BeExeResult Execute(int& iDeltaTime);

protected:
	bool                m_bIsOrb;
	int					m_iSkillTypeID;
	int					m_iSkillLevel;
	int					m_iTargetUnitID;
	int					m_iMoveTime;
	BeTaskMoveToUnit* m_pkMoveToUnit;
	BeTaskActionAttack* m_pkActionAttack;

	BeAttackUnitState	m_eState;
};

enum BeAttackToPosState
{
	BAP_MOVE = 0,
	BAP_REFRESH,
	BAP_TRACE,
	BAP_ATTACK,
	BAP_END,
};

class BeTaskAttackToPos : public BeTask
{
	DECLARE_POOL(BeTaskAttackToPos);

protected:
	BeTaskAttackToPos();
	~BeTaskAttackToPos();

public:
	void SetTargetPos(const TePos2& kPos, float fRange = 0.0f);

	virtual BeExeResult Execute(int& iDeltaTime);

	BeMoveResult GetMoveResult(void);

	bool IsCanCancel() const;

protected:
	TePos2						m_kTarPos;
	float						m_fRange;
	int							m_iRetryTime;
	BeTaskActionAttack* m_pkAttack;
	BeTaskMoveToPos* m_pkMoveToPos;
	BeAttackToPosState			m_eState;
	unsigned int				m_dwMoveTime;
	unsigned int				m_dwOutSightTime;
	int							m_iStartTime;
	int							m_iStartAttackTime;
};

enum BeFollowState
{
	BFS_IDLE = 0,
	BFS_MOVE,
	BFS_ATTACK,
};

class BeTaskFollowUnit : public BeTask
{
	DECLARE_POOL(BeTaskFollowUnit);

protected:
	BeTaskFollowUnit();
	~BeTaskFollowUnit();

public:
	void SetFollowID(int  iID, float fDistance);
	int GetFollowID() const;
	int GetTargetID() const;

	virtual BeExeResult Execute(int& iDeltaTime);

protected:

	BeFollowState		m_eState;
	BeTaskMoveToUnit* m_pkMoveToUnit;
	BeTaskAttackUnit* m_pkAttackUnit;
	int					m_iStandTime;
};

enum BeAttackItemState
{
	BAI_ATTACK = 0,
	BAI_TRACE,
	BAI_END,
};

class BeTaskAttackItem : public BeTask
{
	DECLARE_POOL(BeTaskAttackItem);

protected:
	BeTaskAttackItem();
	~BeTaskAttackItem();

public:
	void SetTargetID(int iID, float fDistance = 0.0f);
	int GetTargetID() const;
	bool IsAttacking() const;

	virtual BeExeResult Execute(int& iDeltaTime);

protected:
	BeTaskMoveToPos* m_pkMoveToPos;
	BeTaskActionAttack* m_pkActionAttack;

	BeAttackItemState	m_eState;

	int					m_iItemID;
};
