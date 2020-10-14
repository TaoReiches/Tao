/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TaskComplex.h"
#include "TW_Main.h"
#include "TW_Functions.h"
#include <Unit_table.hpp>
#include "TW_Unit.h"
#include "TW_UnitMgr.h"
#include "TW_Map.h"
#include "TW_MapItem.h"
#include "TW_MapItemMgr.h"

#define FOLLOW_DISTANCE		(256.0f)
#define FOLLOW_GUARDRANGE	(600.0f)
#define HERO_WALK_BLOCK_TIME (40)
#define WALK_BLOCK_TIME		(100)
#define WALK_BLOCK_COUNT	(3)
#define ATTACK_TO_POS_COUNT (999)

////////////////////////////////////////////////////////////////////////////
BeTaskMoveToPos::BeTaskMoveToPos()
{
	m_eType = STT_MOVE_TO_POS;

	m_eMoveState = BMS_INIT;
	m_eRetryState = BMRS_DITECT;
	m_eFindPathRet = TFR_NONE;
	m_bSoliderCheck = false;
	m_eState = BMR_NONE;
	m_fDistance = 0.0f;
	m_iStandTime = 0;
	m_iWalkBlockTime = WALK_BLOCK_TIME;
	m_iRetryTime = 0;
	m_pkWalk = BeTaskActionWalk::NEW();
}

BeTaskMoveToPos::~BeTaskMoveToPos()
{
	BeTaskActionWalk::DEL(m_pkWalk);
	m_pkWalk = NULL;
};

void BeTaskMoveToPos::SetTargetPos(const TePos2& kPos, float fDistance, bool bTurn)
{
	m_fDistance = fDistance;
	m_kTarPos = kPos;
	m_kMiddlePos = m_kTarPos;
	m_eState = BMR_INITED;
	m_eMoveState = BMS_INIT;
	m_bSoliderCheck = false;

	m_iStandTime = 0;
	m_pkWalk->AttachMain(pkAttachMain);
	m_pkWalk->AttachUnit(pkAttachUnit);
}

TePos2 BeTaskMoveToPos::GetTargetPos() const
{
	return m_kTarPos;
}

BeMoveResult BeTaskMoveToPos::GetMoveResult(void) const
{
	return m_eState;
}

void BeTaskMoveToPos::SetMoveResult(BeMoveResult eResult)
{
	m_eState = eResult;
}

BeExeResult BeTaskMoveToPos::Execute(int& iDeltaTime)
{
	BeTask::Execute(iDeltaTime);

	if (!pkAttachUnit)
	{
		return BER_TIME_OUT;
	}
	if (GetDistance2(gUnit.GetPosX(), gUnit.GetPosY(), m_kTarPos.fX, m_kTarPos.fY) <= (m_fDistance * m_fDistance))
	{
		m_eState = BMR_SUCCESS;
		m_eMoveState = BMS_INIT;
		gUnit.SetActionState(0);
		return BER_EXE_END;
	}

	if (gUnit.GetClass() == UNIT_CLASSTYPE_BUILDING)
	{
		return BER_EXE_END;
	}
	if (gUnit.HasUnitCarryFlag(BUCF_CANNOTMOVE)) {
		return BER_EXE_END;
	}
	if (m_eState == BMR_NONE)
	{
		m_eMoveState = BMS_INIT;
		return BER_EXE_END;
	}

	int iLoopCount = 10;
	while (iDeltaTime > 0)
	{
		--iLoopCount;
		if (iLoopCount <= 0)
		{
			iDeltaTime = 0;
			break;
		}
		switch (m_eMoveState)
		{
		case BMS_INIT:
		{
			if (gUnit.GetClass() == UNIT_CLASSTYPE_SOLIDER)
			{
				gMap.SetUnitPosition(&gUnit, gUnit.GetPosX(), gUnit.GetPosY(), 0.0f, 1000.0f, false, TGF_FIXED_OTS | TGF_SOLIDER, 0, true);
			}
			else
			{
				gMap.SetUnitPosition(&gUnit, gUnit.GetPosX(), gUnit.GetPosY(), 0.0f, 1000.0f, false, TGF_FIXED_OTS | TGF_UNIT, 0, true);
			}

			gMap.DelUnitObstacle(&gUnit, false);
			m_iStandTime = 0;
			m_iRetryTime = 0;
			std::list<TePos2> akPath;

			{
				int iObs = gUnit.HasFlag(BUF_ISBLOCKED) ? TGF_FIXED_OTS | TGF_UNIT : TGF_FIXED_OTS;
				if (gUnit.GetClass() == UNIT_CLASSTYPE_SOLIDER)
				{
					iObs = TGF_FIXED_OTS | TGF_SOLIDER_COLLION;
				}
				else
				{
					iObs = TGF_FIXED_OTS | TGF_UNIT | TGF_SOLIDER;
				}

				m_eFindPathRet = gMap.FindPath(akPath, &gUnit, m_kTarPos.fX, m_kTarPos.fY, m_fDistance, iObs);
			}

			if (m_eFindPathRet == TFR_NONE || !gUnit.CanMove())
			{
				m_pkWalk->SetTargetPos(m_kTarPos, true);
				m_eMoveState = BMS_WALK;
				m_eRetryState = BMRS_DITECT;
			}
			else if (m_eFindPathRet == TFR_ARRIVED)
			{
				m_eState = BMR_SUCCESS;
				m_eMoveState = BMS_END;
			}
			else
			{
				if (akPath.empty())
				{
					m_eMoveState = BMS_STAND;
				}
				else
				{
					m_kDirectPos = akPath.front();
					akPath.pop_front();

					m_kMiddlePos = akPath.size() ? akPath.front() : m_kTarPos;
					m_pkWalk->AttachMain(pkAttachMain);
					m_pkWalk->AttachUnit(pkAttachUnit);
					m_pkWalk->SetTargetPos(m_kDirectPos);
					m_eMoveState = BMS_WALK;
					m_eRetryState = BMRS_DITECT;
				}
			}
			break;
		}
		case BMS_RETRY:
		{
			if (!gUnit.CanMove())
			{
				m_iWalkBlockTime = 200;
				m_eMoveState = BMS_STAND;
				m_iStandTime = 0;
				m_iRetryTime = 0;
				break;
			}

			++m_iRetryTime;
			if (gUnit.GetClass() == UNIT_CLASSTYPE_SOLIDER)
			{
				gMap.SetUnitPosition(&gUnit, gUnit.GetPosX(), gUnit.GetPosY(), 0.0f, 1000.0f, false, TGF_FIXED_OTS | TGF_SOLIDER, 0, true);
			}
			else
			{
				gMap.SetUnitPosition(&gUnit, gUnit.GetPosX(), gUnit.GetPosY(), 0.0f, 1000.0f, false, TGF_FIXED_OTS | TGF_UNIT, 0, true);
			}

			gMap.DelUnitObstacle(&gUnit, false);
			if (m_iRetryTime > WALK_BLOCK_COUNT)
			{
				if (m_eRetryState == BMRS_DITECT)
				{
					m_iRetryTime = 0;
					m_eRetryState = BMRS_MIDDLE;
					m_eMoveState = BMS_STAND;
				}
				else if (m_eRetryState == BMRS_MIDDLE)
				{
					m_eRetryState = BMRS_FINNAL;
					m_eMoveState = BMS_STAND;
				}
				else
				{
					m_eState = BMR_BLOCK;
					m_eMoveState = BMS_END;
				}
				break;
			}

			m_iStandTime = 0;
			m_iWalkBlockTime = gUnit.IsHero() ? HERO_WALK_BLOCK_TIME : WALK_BLOCK_TIME;

			TePos2 kFindPos = m_kTarPos;
			switch (m_eRetryState)
			{
			case BMRS_DITECT:
			{
				float fCanMoveDistance = gUnit.GetMoveSpeed() * iDeltaTime / 1000.0f;
				float fMaxMoveDistance = 128.0f;

				float fWalkX = 0.0f;
				float fWalkY = 0.0f;
				if (gUnit.GetClass() == UNIT_CLASSTYPE_SOLIDER)
				{
					gMap.GetFirstCanStay(&gUnit, m_kDirectPos.fX, m_kDirectPos.fY, fWalkX, fWalkY, fMaxMoveDistance, TGF_FIXED_OTS | TGF_SOLIDER_COLLION | TGF_TEMP);
				}
				else
				{
					gMap.GetFirstCanStay(&gUnit, m_kDirectPos.fX, m_kDirectPos.fY, fWalkX, fWalkY, fMaxMoveDistance, TGF_FIXED_OTS | TGF_UNIT_OTS | TGF_SOLIDER);
				}

				float fNeedMoveDistance = GetDistance(gUnit.GetPosX(), gUnit.GetPosY(), fWalkX, fWalkY);
				if (fNeedMoveDistance > fCanMoveDistance)
				{
					m_pkWalk->SetTargetPos(m_kDirectPos);
					m_eMoveState = BMS_WALK;
					break;
				}

				m_eRetryState = BMRS_MIDDLE;
			}
			case BMRS_MIDDLE:
			{
				kFindPos = m_kMiddlePos;
			}
			case BMRS_FINNAL:
			default:
			{
				std::list<TePos2> akPath;
				int iObs = gUnit.HasFlag(BUF_ISBLOCKED) ? TGF_FIXED_OTS | TGF_UNIT : TGF_FIXED_OTS;

				if (gUnit.GetClass() == UNIT_CLASSTYPE_SOLIDER)
				{
					iObs = TGF_FIXED_OTS | TGF_SOLIDER_COLLION;
				}
				else
				{
					iObs = TGF_FIXED_OTS | TGF_UNIT | TGF_SOLIDER;
				}

				m_eFindPathRet = gMap.FindPath(akPath, &gUnit, m_kTarPos.fX, m_kTarPos.fY, m_fDistance, iObs);

				if (m_eFindPathRet == TFR_NONE)
				{
					m_eRetryState = BMRS_FINNAL;
					m_eMoveState = BMS_STAND;
				}
				else if (m_eFindPathRet == TFR_ARRIVED)
				{
					m_iRetryTime = 0;
					if (m_eRetryState == BMRS_FINNAL || m_kTarPos == m_kMiddlePos)
					{
						m_eState = BMR_SUCCESS;
						m_eMoveState = BMS_END;
					}
					else
					{
						m_eMoveState = BMS_STAND;
						m_eRetryState = BMRS_FINNAL;
					}
				}
				else
				{
					m_kDirectPos = akPath.front();
					akPath.pop_front();

					if (m_eRetryState == BMRS_FINNAL || m_kTarPos == m_kMiddlePos)
					{
						m_kMiddlePos = akPath.size() ? akPath.front() : m_kTarPos;
					}
					m_pkWalk->SetTargetPos(m_kDirectPos);
					m_eMoveState = BMS_WALK;
					m_eRetryState = BMRS_DITECT;
				}

			}break;
			}
			break;
		}
		case BMS_STAND:
		{
			if ((m_iStandTime + iDeltaTime) < m_iWalkBlockTime)
			{
				gUnit.IncActionCurTime(iDeltaTime);
				m_iStandTime += iDeltaTime;
				iDeltaTime = 0;
			}
			else
			{
				int iStandTime = (m_iWalkBlockTime - m_iStandTime);
				gUnit.IncActionCurTime(iStandTime);
				iDeltaTime -= iStandTime;
				m_eMoveState = BMS_RETRY;
			}
			break;
		}
		case BMS_WALK:
		{
			BeExeResult eRet = m_pkWalk->Execute(iDeltaTime);
			if (eRet == BER_EXE_END)
			{
				if (m_pkWalk->IsBlocked())
				{
					gUnit.SetFlag(BUF_ISBLOCKED);
					m_eMoveState = BMS_STAND;
					m_eRetryState = BMRS_DITECT;
					if (m_iRetryTime == 0)
					{
						m_eRetryState = BMRS_FINNAL;
						m_eMoveState = BMS_RETRY;
					}
				}
				else
				{
					gUnit.ClrFlag(BUF_ISBLOCKED);
					m_eState = BMR_SUCCESS;
					m_eMoveState = BMS_RETRY;
					gUnit.SetActionState(0);
					gUnit.SetPathFindSucessTime(GAME_FRAME_TIME - iDeltaTime);
				}
			}
			else
			{
				m_iRetryTime = 0;
			}
			break;
		}
		default:
		{
			return BER_EXE_END;
		}
		}
	}

	return BER_TIME_OUT;
}

BeTaskMoveToUnit::BeTaskMoveToUnit()
{
	m_eType = STT_MOVE_TO_UNIT;
	m_iTargetID = 0;
	m_fDistance = 0.0f;
	m_pkMoveToPos = BeTaskMoveToPos::NEW();
}

BeTaskMoveToUnit::~BeTaskMoveToUnit()
{
	BeTaskMoveToPos::DEL(m_pkMoveToPos);
	m_pkMoveToPos = NULL;
}

BeMoveResult BeTaskMoveToUnit::GetMoveResult(void)
{
	return m_pkMoveToPos->GetMoveResult();
}

void BeTaskMoveToUnit::SetTargetID(int iID, float fDistance)
{
	m_iTargetID = iID;
	m_fDistance = fDistance;

	m_pkMoveToPos->AttachMain(pkAttachMain);
	m_pkMoveToPos->AttachUnit(pkAttachUnit);

	if (!iID)
	{
		TePos2 kTarPos = TePos2(gUnit.GetPosX(), gUnit.GetPosY());
		m_pkMoveToPos->SetTargetPos(kTarPos);
	}

	BeUnit* pkTarget = gUnitMgr.GetUnitByID(m_iTargetID);
	if (pkTarget)
	{
		TePos2 kTarPos = TePos2(pkTarget->GetPosX(), pkTarget->GetPosY());
		m_pkMoveToPos->SetTargetPos(kTarPos, m_fDistance);
	}
	else
	{
		m_iTargetID = 0;
	}
}

int BeTaskMoveToUnit::GetTargetID() const
{
	return m_iTargetID;
}

BeExeResult BeTaskMoveToUnit::Execute(int& iDeltaTime)
{
	BeTask::Execute(iDeltaTime);

	if (!gUnit.CanSpell())
	{
		return BER_EXE_END;
	}

	BeUnit* pkTarget = gUnitMgr.GetUnitByID(m_iTargetID);
	if (pkTarget
		&& !pkTarget->IsDead()
		&& (!(pkTarget->HasFlag(BUF_HASINVISIBLE)) || (pkTarget->GetCamp() == gUnit.GetCamp()))
		)
	{
		TePos2 kTarPos = m_pkMoveToPos->GetTargetPos();
		float fDistance2 = GetDistance2(kTarPos.fX, kTarPos.fY, pkTarget->GetPosX(), pkTarget->GetPosY());
		if (fDistance2 > 2000.0f * 2000.0f)
		{
			m_iTargetID = -1;
		}
		else if (fDistance2 > 32.0f * 32.0f)
		{
			float fAllDistance = GetDistance2(gUnit.GetPosX(), gUnit.GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
			if (fAllDistance > 500.0f * 500.0f)
			{
				if (fDistance2 > 64.0f * 64.0f)
				{
					kTarPos = TePos2(pkTarget->GetPosX(), pkTarget->GetPosY());
					m_pkMoveToPos->SetTargetPos(kTarPos, m_fDistance);
				}
			}
			else
			{
				kTarPos = TePos2(pkTarget->GetPosX(), pkTarget->GetPosY());
				m_pkMoveToPos->SetTargetPos(kTarPos, m_fDistance);
			}

		}
	}
	else
	{
		TePos2 kTarPos = m_pkMoveToPos->GetTargetPos();
		m_pkMoveToPos->SetTargetPos(kTarPos);
		BeUnit* pkT = gUnitMgr.GetUnitByID(m_iTargetID, true);
		if (pkT && pkT->GetPlayer() == gUnit.GetPlayer())
		{
			return BER_EXE_END;
		}
	}

	BeExeResult eRet = m_pkMoveToPos->Execute(iDeltaTime);
	return eRet;
}

BeTaskAttackUnit::BeTaskAttackUnit()
{
	m_eType = STT_ATTACK_UNIT;
	m_eState = BAU_END;

	m_iSkillTypeID = 0;
	m_iSkillLevel = 0;
	m_iTargetUnitID = 0;
	m_bIsOrb = false;
	m_iMoveTime = 0;

	m_pkMoveToUnit = BeTaskMoveToUnit::NEW();
	m_pkActionAttack = BeTaskActionAttack::NEW();
}

BeTaskAttackUnit::~BeTaskAttackUnit()
{
	BeTaskMoveToUnit::DEL(m_pkMoveToUnit);
	m_pkMoveToUnit = NULL;
	BeTaskActionAttack::DEL(m_pkActionAttack);
	m_pkActionAttack = NULL;
}

void BeTaskAttackUnit::SetTargetID(int iID, float fDistance, bool bIsOrb, int iSkillTypeID, int iSkillLevel, int iAttackCountLimit)
{
	m_pkMoveToUnit->AttachMain(pkAttachMain);
	m_pkMoveToUnit->AttachUnit(pkAttachUnit);

	m_pkActionAttack->AttachMain(pkAttachMain);
	m_pkActionAttack->AttachUnit(pkAttachUnit);

	m_iSkillTypeID = iSkillTypeID;
	m_iSkillLevel = iSkillLevel;
	m_iTargetUnitID = iID;
	m_iMoveTime = 0;

	m_pkMoveToUnit->SetTargetID(iID);
	if (gUnit.GetAttackingUnitID() == iID)
	{
		if (m_eState == BAU_END)
		{
			m_eState = BAU_TRACE;
		}
		return;
	}

	gUnit.SetAttackingUnitID(iID, bIsOrb);

	m_pkActionAttack->SetTargetID(iID, bIsOrb, m_iSkillTypeID, m_iSkillLevel);
	m_bIsOrb = bIsOrb;
	m_eState = BAU_TRACE;
}

int BeTaskAttackUnit::GetTargetID() const
{
	return gUnit.GetAttackingUnitID();
}

bool BeTaskAttackUnit::IsAttacking() const
{
	return (bool)(m_eState == BAU_ATTACK);
}

bool BeTaskAttackUnit::IsDamaged() const
{
	if (m_pkActionAttack)
	{
		return m_pkActionAttack->IsDamaged();
	}
	return false;
}

bool BeTaskAttackUnit::IsCanCancel() const
{
	if (m_pkActionAttack)
	{
		return m_pkActionAttack->IsCanCancel();
	}
	return false;
}

bool BeTaskAttackUnit::CanSkip(void) const
{
	return true;
}

BeExeResult BeTaskAttackUnit::Execute(int& iDeltaTime)
{
	BeTask::Execute(iDeltaTime);
	const BeUnit* pkTarget = gUnitMgr.GetUnitByID(GetTargetID());

	if (!m_pkActionAttack || !m_pkMoveToUnit)
	{
		return BER_EXE_END;
	}

	int iLoopCount = 10;
	while (iDeltaTime > 0)
	{
		--iLoopCount;
		if (iLoopCount <= 0)
		{
			iDeltaTime = 0;
			break;
		}
		switch (m_eState)
		{
		case BAU_ATTACK:
		{
			m_iMoveTime = 0;
			BeExeResult eRet = m_pkActionAttack->Execute(iDeltaTime);
			if (m_pkActionAttack->IsPerDamageEnd())
			{
				if (pkTarget)
				{
					float fDistance2 = GetDistance2(gUnit.GetPosX(), gUnit.GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
					float fD = gUnit.GetAttackRange(pkTarget);
					if (fDistance2 > fD * fD)
					{
						m_pkMoveToUnit->SetTargetID(pkTarget->GetID(), fD);
						m_eState = BAU_TRACE;
						break;
					}
				}
			}
			if (eRet == BER_TIME_OUT)
			{
				return BER_TIME_OUT;
			}

			if (!pkTarget || !gUnit.UnitCanAttack(pkTarget, true, true))
			{
				m_eState = BAU_REFRESH;
				break;
			}
			float fDistance2 = GetDistance2(gUnit.GetPosX(), gUnit.GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
			float fD = gUnit.GetAttackRange(pkTarget);
			if (fDistance2 > fD * fD)
			{
				m_pkMoveToUnit->SetTargetID(pkTarget->GetID(), fD);
				m_eState = BAU_TRACE;
			}
			break;
		}
		case BAU_REFRESH:
		{
			if (gUnit.HasUnitCarryFlag(BUCF_CANNOTATTACK))
			{
				return BER_TIME_OUT;
			}

			m_eState = BAU_TRACE;

			SetTargetID(pkTarget->GetID(), 0.0f, m_bIsOrb, m_iSkillTypeID, m_iSkillLevel);
		}
		case BAU_TRACE:
		{
			m_iMoveTime += iDeltaTime;
			if (pkTarget)
			{
				float fDistance2 = GetDistance2(gUnit.GetPosX(), gUnit.GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
				float fRange = gUnit.GetAttackRange(pkTarget);
				if (fDistance2 <= (fRange * fRange))
				{
					m_pkActionAttack->SetTargetID(0, m_bIsOrb, m_iSkillTypeID, m_iSkillLevel);
					m_pkActionAttack->SetTargetID(pkTarget->GetID(), m_bIsOrb, m_iSkillTypeID, m_iSkillLevel);
					m_eState = BAU_ATTACK;

					gUnit.SetActionState(0);
					break;
				}
			}

			BeExeResult eRet = BER_EXE_END;
			if (gUnit.CanMove()) {
				eRet = m_pkMoveToUnit->Execute(iDeltaTime);
			}

			if (eRet == BER_TIME_OUT)
			{
				return BER_TIME_OUT;
			}
			else
			{
				if (m_pkMoveToUnit->GetMoveResult() == BMR_SUCCESS && pkTarget)
				{
					m_pkActionAttack->SetTargetID(0, m_bIsOrb, m_iSkillTypeID, m_iSkillLevel);
					m_pkActionAttack->SetTargetID(pkTarget->GetID(), m_bIsOrb, m_iSkillTypeID, m_iSkillLevel);
					m_eState = BAU_ATTACK;
					break;
				}
				m_eState = BAU_END;
			}
		}
		case BAU_END:
		{
			m_iMoveTime = 0;
			m_eState = BAU_TRACE;
			return BER_EXE_END;
		}
		default:
		{
			break;
		}
		}
	}
	return BER_TIME_OUT;
}

BeTaskAttackToPos::BeTaskAttackToPos()
{
	m_eType = STT_ATTACK_TO_POS;

	m_fRange = 0.0f;
	m_iRetryTime = 0;
	m_pkAttack = BeTaskActionAttack::NEW();
	m_pkMoveToPos = BeTaskMoveToPos::NEW();
	m_eState = BAP_REFRESH;
	m_dwMoveTime = 0;
	m_dwOutSightTime = 0;
	m_iStartTime = 0;
	m_iStartAttackTime = 0;
}

BeTaskAttackToPos::~BeTaskAttackToPos()
{
	BeTaskActionAttack::DEL(m_pkAttack);
	m_pkAttack = NULL;
	BeTaskMoveToPos::DEL(m_pkMoveToPos);
	m_pkMoveToPos = NULL;
	m_dwOutSightTime = 0;
}

BeMoveResult BeTaskAttackToPos::GetMoveResult(void)
{
	return m_pkMoveToPos->GetMoveResult();
}

bool BeTaskAttackToPos::IsCanCancel() const
{
	return true;
}
void BeTaskAttackToPos::SetTargetPos(const TePos2& kPos, float fRange)
{
	if (!m_pkAttack || !m_pkMoveToPos)
	{
		return;
	}
	m_pkAttack->AttachMain(pkAttachMain);
	m_pkAttack->AttachUnit(pkAttachUnit);

	m_pkMoveToPos->AttachMain(pkAttachMain);
	m_pkMoveToPos->AttachUnit(pkAttachUnit);

	m_fRange = fRange;
	m_iRetryTime = 0;
	m_pkMoveToPos->SetTargetPos(kPos);
	m_kTarPos = kPos;
	m_dwOutSightTime = 0;
}

BeExeResult BeTaskAttackToPos::Execute(int& iDeltaTime)
{
	BeTask::Execute(iDeltaTime);

	m_iStartTime += iDeltaTime;

	if (!m_pkMoveToPos || !m_pkAttack)
	{
		return BER_EXE_END;
	}

	int iLoopCount = 20;
	while (iDeltaTime > 0)
	{
		--iLoopCount;
		if (iLoopCount <= 0)
		{
			iDeltaTime = 0;
			break;
		}
		switch (m_eState)
		{
		case BAP_MOVE:
		{
			m_iStartAttackTime = 0;
			m_dwMoveTime += iDeltaTime;
			BeExeResult eRet = m_pkMoveToPos->Execute(iDeltaTime);

			if (gUnit.GetCommandCount() > 0)
			{
				float fDistance2 = GetDistance2(gUnit.GetPosX(), gUnit.GetPosY(), m_kTarPos.fX, m_kTarPos.fY);
				if (fDistance2 < m_fRange * m_fRange)
				{
					m_dwMoveTime = 0;
					m_iRetryTime = 0;
					m_eState = BAP_END;
				}
			}

			if (eRet == BER_TIME_OUT)
			{
				return BER_TIME_OUT;
			}
			else
			{
				m_dwMoveTime -= iDeltaTime;
				if (m_pkMoveToPos->GetMoveResult() == BMR_SUCCESS)
				{
					m_dwMoveTime = 0;
					m_iRetryTime = 0;
				}
				else
				{
					++m_iRetryTime;
				}
				m_eState = BAP_END;
			}
			break;
		}
		case BAP_REFRESH:
		{
			m_dwMoveTime = 0;
			if (gUnit.GetAttackingUnitID())
			{
				BeUnit* pkOrgTarget = gUnitMgr.GetUnitByID(gUnit.GetAttackingUnitID());
				if (pkOrgTarget && !pkOrgTarget->IsDead() && (!pkOrgTarget->HasFlag(BUF_HASINVISIBLE) || pkOrgTarget->IsUnitNotInvisToCamp(gUnit.GetCamp())))
				{
					TePos2 kPos;
					kPos.fX = pkOrgTarget->GetPosX();
					kPos.fY = pkOrgTarget->GetPosY();
					float fDistance = gUnit.GetAttackRange(pkOrgTarget);
					m_pkMoveToPos->SetTargetPos(kPos, fDistance);
					m_pkAttack->SetTargetID(gUnit.GetAttackingUnitID());
					m_eState = BAP_TRACE;
					break;
				}
			}
			else
			{
				m_pkAttack->SetTargetID(0);
				TePos2 kPos = m_pkMoveToPos->GetTargetPos();
				if (m_kTarPos.fX != kPos.fX || m_kTarPos.fY != kPos.fY)
				{
					m_pkMoveToPos->SetTargetPos(m_kTarPos);
				}
				m_eState = BAP_MOVE;
			}
			break;
		}
		case BAP_TRACE:
		{
			{
				BeUnit* pkTarget = gUnitMgr.GetUnitByID(gUnit.GetAttackingUnitID());

				if (pkTarget && !pkTarget->IsDead())
				{
					if (pkTarget->GetUnitVisionForCamp(gUnit.GetCamp() - 1))
					{
						m_dwOutSightTime = 0;
						TePos2 kPos = m_pkMoveToPos->GetTargetPos();
						if (fabs(kPos.fX - pkTarget->GetPosX()) > 16.0f || fabs(kPos.fY - pkTarget->GetPosY()) > 16.0f)
						{
							kPos.fX = pkTarget->GetPosX();
							kPos.fY = pkTarget->GetPosY();
							float fDistance = gUnit.GetAttackRange(pkTarget);
							m_pkMoveToPos->SetTargetPos(kPos, fDistance);
						}
					}
					else
					{
						m_eState = BAP_REFRESH;
						break;
					}

					m_dwOutSightTime += iDeltaTime;
					m_dwMoveTime += iDeltaTime;
					BeExeResult eRet = m_pkMoveToPos->Execute(iDeltaTime);

					if (eRet == BER_TIME_OUT)
					{
					}
					else
					{
						m_dwMoveTime -= iDeltaTime;
						if (m_pkMoveToPos->GetMoveResult() == BMR_SUCCESS)
						{
							m_dwMoveTime = 0;
							m_iRetryTime = 0;
							m_eState = BAP_ATTACK;
						}
						else
						{
							++m_iRetryTime;
							m_eState = BAP_REFRESH;
						}
					}
				}
				else
				{
					m_eState = BAP_REFRESH;
				}
			}
			if (m_dwOutSightTime > 2000)
			{
				m_dwOutSightTime = 0;
				m_eState = BAP_REFRESH;
			}
			break;
		}
		case BAP_ATTACK:
		{
			m_iStartAttackTime += iDeltaTime;
			BeExeResult eRet = m_pkAttack->Execute(iDeltaTime);
			if (eRet == BER_EXE_END)
			{
				BeUnit* pkTarget = gUnitMgr.GetUnitByID(gUnit.GetAttackingUnitID());
				if (pkTarget && pkTarget->GetClass() == UNIT_CLASSTYPE_BUILDING)
				{
					m_eState = BAP_REFRESH;
					break;
				}
				{
					if (!pkTarget || !gUnit.UnitCanAttack(pkTarget, true, true))
					{
						m_eState = BAP_REFRESH;
						break;
					}

					float fDistance2 = GetDistance2(gUnit.GetPosX(), gUnit.GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
					float fRange = gUnit.GetAttackRange(pkTarget);
					if (fDistance2 > fRange * fRange)
					{
						TePos2 kPos(gUnit.GetPosX(), gUnit.GetPosY());
						m_pkMoveToPos->SetTargetPos(kPos, fRange);
						m_eState = BAP_TRACE;
					}
				}
			}
			break;
		}
		case BAP_END:
		{
			m_eState = BAP_REFRESH;
			return BER_EXE_END;
		}
		default:break;
		}
	}
	return BER_TIME_OUT;
}

BeTaskFollowUnit::BeTaskFollowUnit()
{
	m_eType = STT_FOLLOW_UNIT;

	m_eState = BFS_MOVE;
	m_iStandTime = 0;
	m_pkMoveToUnit = BeTaskMoveToUnit::NEW();
	m_pkAttackUnit = BeTaskAttackUnit::NEW();
}

BeTaskFollowUnit::~BeTaskFollowUnit()
{
	BeTaskMoveToUnit::DEL(m_pkMoveToUnit);
	m_pkMoveToUnit = NULL;
	BeTaskAttackUnit::DEL(m_pkAttackUnit);
	m_pkAttackUnit = NULL;
}

void BeTaskFollowUnit::SetFollowID(int iID, float fDistance)
{
	fDistance = 100.0f;

	m_pkAttackUnit->AttachMain(pkAttachMain);
	m_pkAttackUnit->AttachUnit(pkAttachUnit);

	m_pkMoveToUnit->AttachMain(pkAttachMain);
	m_pkMoveToUnit->AttachUnit(pkAttachUnit);

	m_pkMoveToUnit->SetTargetID(iID, fDistance);
	gUnit.SetAttackingUnitID(0);
	m_eState = BFS_MOVE;
}

int BeTaskFollowUnit::GetFollowID() const
{
	return m_pkMoveToUnit->GetTargetID();
}
int BeTaskFollowUnit::GetTargetID() const
{
	return m_pkAttackUnit->GetTargetID();
}

BeExeResult BeTaskFollowUnit::Execute(int& iDeltaTime)
{
	BeTask::Execute(iDeltaTime);

	BeExeResult eRet = BER_TIME_OUT;
	BeUnit* pkFollowUnit = gUnitMgr.GetUnitByID(GetFollowID());
	if (!pkFollowUnit)
	{
		return BER_EXE_END;
	}

	int iLoopCount = 20;
	while (iDeltaTime > 0)
	{
		--iLoopCount;
		if (iLoopCount <= 0)
		{
			iDeltaTime = 0;
			break;
		}
		switch (m_eState)
		{
		case BFS_IDLE:
		{
			gUnit.ClrFlag(BUF_MOVING);

			if (!gUnit.IsActionNow(BUA_STAND))
			{
				gUnit.SetUnitAction(BUA_STAND, BAN_stand);
			}

			if ((m_iStandTime + iDeltaTime) < WALK_BLOCK_TIME)
			{
				gUnit.IncActionCurTime(iDeltaTime);
				m_iStandTime += iDeltaTime;
				iDeltaTime = 0;
			}
			else
			{
				int iStandTime = (WALK_BLOCK_TIME - m_iStandTime);
				gUnit.IncActionCurTime(iStandTime);
				iDeltaTime -= iStandTime;
				{
					m_eState = BFS_MOVE;
				}
			}
		}break;
		case BFS_MOVE:
		{
			eRet = m_pkMoveToUnit->Execute(iDeltaTime);
			if (BER_EXE_END == eRet)
			{
				m_iStandTime = 0;
				m_eState = BFS_IDLE;
			}
		}break;
		case BFS_ATTACK:
		{
			eRet = m_pkAttackUnit->Execute(iDeltaTime);
			float fDistance2 = GetDistance2(gUnit.GetPosX(), gUnit.GetPosY(), pkFollowUnit->GetPosX(), pkFollowUnit->GetPosY());
			if (fDistance2 > ((FOLLOW_GUARDRANGE * FOLLOW_GUARDRANGE) + (FOLLOW_DISTANCE * FOLLOW_DISTANCE)))
			{
				m_eState = BFS_MOVE;
			}
			else if (fDistance2 > (FOLLOW_GUARDRANGE * FOLLOW_GUARDRANGE))
			{
				if (!m_pkAttackUnit->IsAttacking())
				{
					m_eState = BFS_MOVE;
				}
			}
			else
			{
				if (eRet == BER_EXE_END)
				{
					m_iStandTime = 0;
					m_eState = BFS_IDLE;
				}
			}
		}break;
		default:
			break;
		}
	}
	return BER_TIME_OUT;
}

BeTaskAttackItem::BeTaskAttackItem()
{
	m_eType = STT_ATTACK_ITEM;
	m_eState = BAI_END;
	m_pkMoveToPos = BeTaskMoveToPos::NEW();
	m_pkActionAttack = BeTaskActionAttack::NEW();
	m_iItemID = 0;
}

BeTaskAttackItem::~BeTaskAttackItem()
{
	BeTaskMoveToPos::DEL(m_pkMoveToPos);
	m_pkMoveToPos = NULL;
	BeTaskActionAttack::DEL(m_pkActionAttack);
	m_pkActionAttack = NULL;
}

void BeTaskAttackItem::SetTargetID(int iID, float fDistance)
{
	if (m_iItemID == iID)
	{
		return;
	}
	m_iItemID = iID;
	const BeMapItem* pkTarget = gMapItemMgr.GetMapItemByID(m_iItemID);
	m_pkMoveToPos->AttachMain(pkAttachMain);
	m_pkMoveToPos->AttachUnit(pkAttachUnit);

	m_pkActionAttack->AttachMain(pkAttachMain);
	m_pkActionAttack->AttachUnit(pkAttachUnit);

	m_pkMoveToPos->SetTargetPos(TePos2(gUnit.GetPosX(), gUnit.GetPosY()), 50.0f);
	if (pkTarget)
	{
		m_pkMoveToPos->SetTargetPos(TePos2(pkTarget->GetPosX(), pkTarget->GetPosY()), 0.0f);
	}
	gUnit.SetAttackingUnitID(0);

	m_pkActionAttack->SetTargetItem(iID);
	m_eState = BAI_TRACE;
}

int BeTaskAttackItem::GetTargetID() const
{
	return m_iItemID;
}

bool BeTaskAttackItem::IsAttacking() const
{
	return (bool)(m_eState == BAI_ATTACK);
}

BeExeResult BeTaskAttackItem::Execute(int& iDeltaTime)
{
	BeTask::Execute(iDeltaTime);

	const BeMapItem* pkTarget = gMapItemMgr.GetMapItemByID(m_iItemID);

	int iLoopCount = 10;
	while (iDeltaTime > 0)
	{
		--iLoopCount;
		if (iLoopCount <= 0)
		{
			iDeltaTime = 0;
			break;
		}
		switch (m_eState)
		{
		case BAI_ATTACK:
		{
			BeExeResult eRet = m_pkActionAttack->Execute(iDeltaTime);
			if (eRet == BER_TIME_OUT)
			{
				return BER_TIME_OUT;
			}

			if (!pkTarget)
			{
				m_eState = BAI_END;
				break;
			}

			float fDistance2 = GetDistance2(gUnit.GetPosX(), gUnit.GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
			float fD = 200.0f;
			if (fDistance2 > fD * fD)
			{
				m_eState = BAI_TRACE;
			}
			break;
		}
		case BAI_TRACE:
		{
			if (!pkTarget)
			{
				return BER_EXE_END;
			}

			float fDistance2 = GetDistance2(gUnit.GetPosX(), gUnit.GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
			float fRange = 200.0f;
			if (fDistance2 <= (fRange * fRange))
			{
				m_eState = BAI_ATTACK;
				break;
			}

			BeExeResult eRet = m_pkMoveToPos->Execute(iDeltaTime);
			if (eRet == BER_TIME_OUT)
			{
				return BER_TIME_OUT;
			}
			else
			{
				if (m_pkMoveToPos->GetMoveResult() == BMR_SUCCESS)
				{
					m_eState = BAI_ATTACK;
					break;
				}
				m_eState = BAI_END;
			}
		}
		case BAI_END:
		{
			return BER_EXE_END;
		}
		default:break;
		}
	}
	return BER_TIME_OUT;
}
