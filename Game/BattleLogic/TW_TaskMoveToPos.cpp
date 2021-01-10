/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TaskMoveToPos.h"
#include "TW_MemoryObject.h"
#include "TW_Functions.h"
#include "TW_Main.h"
#include "TW_Unit.h"
#include "TW_Map.h"

static const int WALK_BLOCK_TIME = 100;
static const int WALK_BLOCK_COUNT = 3;
static const int HERO_WALK_BLOCK_TIME = 40;

BeTaskMoveToPos::BeTaskMoveToPos()
{
	m_eType = BeTaskType::STT_MOVE_TO_POS;
	m_eMoveState = TwMoveState::BMS_INIT;
	m_eRetryState = BeMoveRetryState::BMRS_DITECT;
	m_eFindPathRet = TwFindResult::TFR_NONE;
	m_bSoliderCheck = false;
	m_eState = TwMoveResult::MR_NONE;
	m_fDistance = 0.0f;
	m_iStandTime = 0;
	m_iWalkBlockTime = WALK_BLOCK_TIME;
	m_iRetryTime = 0;
	m_pkWalk.reset(mpTaskActionWalk.alloc());
}

BeTaskMoveToPos::~BeTaskMoveToPos()
{
	mpTaskActionWalk.free(m_pkWalk.get());
	m_pkWalk.release();
};

void BeTaskMoveToPos::SetTargetPos(const TwPos2& kPos, float fDistance, bool bTurn)
{
	m_fDistance = fDistance;
	m_kTarPos = kPos;
	m_kMiddlePos = m_kTarPos;
	m_eState = TwMoveResult::MR_INITED;
	m_eMoveState = TwMoveState::BMS_INIT;
	m_bSoliderCheck = false;

	m_iStandTime = 0;
	m_pkWalk->AttachMain(pkAttachMain);
	m_pkWalk->AttachUnit(pkAttachUnit);
}

TwPos2 BeTaskMoveToPos::GetTargetPos() const
{
	return m_kTarPos;
}

TwMoveResult BeTaskMoveToPos::GetMoveResult(void) const
{
	return m_eState;
}

void BeTaskMoveToPos::SetMoveResult(TwMoveResult eResult)
{
	m_eState = eResult;
}

BeExeResult BeTaskMoveToPos::Execute(int& iDeltaTime)
{
	TwTask::Execute(iDeltaTime);

	if (!pkAttachUnit)
	{
		return BeExeResult::BER_TIME_OUT;
	}
	if (GetDistance2(gUnit->GetPosX(), gUnit->GetPosY(), m_kTarPos.fX, m_kTarPos.fY) <= (m_fDistance * m_fDistance))
	{
		m_eState = TwMoveResult::MR_SUCCESS;
		m_eMoveState = TwMoveState::BMS_INIT;
		gUnit->SetActionState(0);
		return BeExeResult::BER_EXE_END;
	}

	if (gUnit->GetClass() == UNIT_CLASSTYPE_BUILDING)
	{
		return BeExeResult::BER_EXE_END;
	}
	if (gUnit->HasUnitCarryFlag(BUCF_CANNOTMOVE)) {
		return BeExeResult::BER_EXE_END;
	}
	if (m_eState == TwMoveResult::MR_NONE)
	{
		m_eMoveState = TwMoveState::BMS_INIT;
		return BeExeResult::BER_EXE_END;
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
		case TwMoveState::BMS_INIT:
		{
			if (gUnit->GetClass() == UNIT_CLASSTYPE_SOLIDER)
			{
				gMap.SetUnitPosition(gUnit, gUnit->GetPosX(), gUnit->GetPosY(), 0.0f, 1000.0f, false, TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_SOLIDER, TwGridFlag::TGF_NONE, true);
			}
			else
			{
				gMap.SetUnitPosition(gUnit, gUnit->GetPosX(), gUnit->GetPosY(), 0.0f, 1000.0f, false, TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_UNIT, TwGridFlag::TGF_NONE, true);
			}

			gMap.DelUnitObstacle(gUnit, false);
			m_iStandTime = 0;
			m_iRetryTime = 0;
			std::list<TwPos2> akPath;

			{
				auto iObs = gUnit->HasFlag(TwUnitFlag::BUF_ISBLOCKED) ? TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_UNIT : TwGridFlag::TGF_FIXED_OTS;
				if (gUnit->GetClass() == UNIT_CLASSTYPE_SOLIDER)
				{
					iObs = TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_SOLIDER_COLLION;
				}
				else
				{
					iObs = TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_UNIT | TwGridFlag::TGF_SOLIDER;
				}

				m_eFindPathRet = gMap.FindPath(akPath, gUnit, m_kTarPos.fX, m_kTarPos.fY, m_fDistance, iObs);
			}

			if (m_eFindPathRet == TwFindResult::TFR_NONE || !gUnit->CanMove())
			{
				m_pkWalk->SetTargetPos(m_kTarPos, true);
				m_eMoveState = TwMoveState::BMS_WALK;
				m_eRetryState = BeMoveRetryState::BMRS_DITECT;
			}
			else if (m_eFindPathRet == TwFindResult::TFR_ARRIVED)
			{
				m_eState = TwMoveResult::MR_SUCCESS;
				m_eMoveState = TwMoveState::BMS_END;
			}
			else
			{
				if (akPath.empty())
				{
					m_eMoveState = TwMoveState::BMS_STAND;
				}
				else
				{
					m_kDirectPos = akPath.front();
					akPath.pop_front();

					m_kMiddlePos = akPath.size() ? akPath.front() : m_kTarPos;
					m_pkWalk->AttachMain(pkAttachMain);
					m_pkWalk->AttachUnit(pkAttachUnit);
					m_pkWalk->SetTargetPos(m_kDirectPos);
					m_eMoveState = TwMoveState::BMS_WALK;
					m_eRetryState = BeMoveRetryState::BMRS_DITECT;
				}
			}
			break;
		}
		case TwMoveState::BMS_RETRY:
		{
			if (!gUnit->CanMove())
			{
				m_iWalkBlockTime = 200;
				m_eMoveState = TwMoveState::BMS_STAND;
				m_iStandTime = 0;
				m_iRetryTime = 0;
				break;
			}

			++m_iRetryTime;
			if (gUnit->GetClass() == UNIT_CLASSTYPE_SOLIDER)
			{
				gMap.SetUnitPosition(gUnit, gUnit->GetPosX(), gUnit->GetPosY(), 0.0f, 1000.0f, false, TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_SOLIDER, TwGridFlag::TGF_NONE, true);
			}
			else
			{
				gMap.SetUnitPosition(gUnit, gUnit->GetPosX(), gUnit->GetPosY(), 0.0f, 1000.0f, false, TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_UNIT, TwGridFlag::TGF_NONE, true);
			}

			gMap.DelUnitObstacle(gUnit, false);
			if (m_iRetryTime > WALK_BLOCK_COUNT)
			{
				if (m_eRetryState == BeMoveRetryState::BMRS_DITECT)
				{
					m_iRetryTime = 0;
					m_eRetryState = BeMoveRetryState::BMRS_MIDDLE;
					m_eMoveState = TwMoveState::BMS_STAND;
				}
				else if (m_eRetryState == BeMoveRetryState::BMRS_MIDDLE)
				{
					m_eRetryState = BeMoveRetryState::BMRS_FINNAL;
					m_eMoveState = TwMoveState::BMS_STAND;
				}
				else
				{
					m_eState = TwMoveResult::MR_BLOCK;
					m_eMoveState = TwMoveState::BMS_END;
				}
				break;
			}

			m_iStandTime = 0;
			m_iWalkBlockTime = HERO_WALK_BLOCK_TIME;

			TwPos2 kFindPos = m_kTarPos;
			switch (m_eRetryState)
			{
			case BeMoveRetryState::BMRS_DITECT:
			{
				float fCanMoveDistance = gUnit->GetMoveSpeed() * iDeltaTime / 1000.0f;
				float fMaxMoveDistance = 128.0f;

				float fWalkX = 0.0f;
				float fWalkY = 0.0f;
				if (gUnit->GetClass() == UNIT_CLASSTYPE_SOLIDER)
				{
					gMap.GetFirstCanStay(gUnit, m_kDirectPos.fX, m_kDirectPos.fY, fWalkX, fWalkY, fMaxMoveDistance, TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_SOLIDER_COLLION | TwGridFlag::TGF_TEMP);
				}
				else
				{
					gMap.GetFirstCanStay(gUnit, m_kDirectPos.fX, m_kDirectPos.fY, fWalkX, fWalkY, fMaxMoveDistance, TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_UNIT_OTS | TwGridFlag::TGF_SOLIDER);
				}

				float fNeedMoveDistance = GetDistance(gUnit->GetPosX(), gUnit->GetPosY(), fWalkX, fWalkY);
				if (fNeedMoveDistance > fCanMoveDistance)
				{
					m_pkWalk->SetTargetPos(m_kDirectPos);
					m_eMoveState = TwMoveState::BMS_WALK;
					break;
				}

				m_eRetryState = BeMoveRetryState::BMRS_MIDDLE;
			}
			case BeMoveRetryState::BMRS_MIDDLE:
			{
				kFindPos = m_kMiddlePos;
			}
			case BeMoveRetryState::BMRS_FINNAL:
			default:
			{
				std::list<TwPos2> akPath;
				auto iObs = gUnit->HasFlag(TwUnitFlag::BUF_ISBLOCKED) ? TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_UNIT : TwGridFlag::TGF_FIXED_OTS;

				if (gUnit->GetClass() == UNIT_CLASSTYPE_SOLIDER)
				{
					iObs = TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_SOLIDER_COLLION;
				}
				else
				{
					iObs = TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_UNIT | TwGridFlag::TGF_SOLIDER;
				}

				m_eFindPathRet = gMap.FindPath(akPath, gUnit, m_kTarPos.fX, m_kTarPos.fY, m_fDistance, iObs);

				if (m_eFindPathRet == TwFindResult::TFR_NONE)
				{
					m_eRetryState = BeMoveRetryState::BMRS_FINNAL;
					m_eMoveState = TwMoveState::BMS_STAND;
				}
				else if (m_eFindPathRet == TwFindResult::TFR_ARRIVED)
				{
					m_iRetryTime = 0;
					if (m_eRetryState == BeMoveRetryState::BMRS_FINNAL || m_kTarPos == m_kMiddlePos)
					{
						m_eState = TwMoveResult::MR_SUCCESS;
						m_eMoveState = TwMoveState::BMS_END;
					}
					else
					{
						m_eMoveState = TwMoveState::BMS_STAND;
						m_eRetryState = BeMoveRetryState::BMRS_FINNAL;
					}
				}
				else
				{
					m_kDirectPos = akPath.front();
					akPath.pop_front();

					if (m_eRetryState == BeMoveRetryState::BMRS_FINNAL || m_kTarPos == m_kMiddlePos)
					{
						m_kMiddlePos = akPath.size() ? akPath.front() : m_kTarPos;
					}
					m_pkWalk->SetTargetPos(m_kDirectPos);
					m_eMoveState = TwMoveState::BMS_WALK;
					m_eRetryState = BeMoveRetryState::BMRS_DITECT;
				}

			}break;
			}
			break;
		}
		case TwMoveState::BMS_STAND:
		{
			if ((m_iStandTime + iDeltaTime) < m_iWalkBlockTime)
			{
				gUnit->IncActionCurTime(iDeltaTime);
				m_iStandTime += iDeltaTime;
				iDeltaTime = 0;
			}
			else
			{
				int iStandTime = (m_iWalkBlockTime - m_iStandTime);
				gUnit->IncActionCurTime(iStandTime);
				iDeltaTime -= iStandTime;
				m_eMoveState = TwMoveState::BMS_RETRY;
			}
			break;
		}
		case TwMoveState::BMS_WALK:
		{
			BeExeResult eRet = m_pkWalk->Execute(iDeltaTime);
			if (eRet == BeExeResult::BER_EXE_END)
			{
				if (m_pkWalk->IsBlocked())
				{
					gUnit->SetFlag(TwUnitFlag::BUF_ISBLOCKED);
					m_eMoveState = TwMoveState::BMS_STAND;
					m_eRetryState = BeMoveRetryState::BMRS_DITECT;
					if (m_iRetryTime == 0)
					{
						m_eRetryState = BeMoveRetryState::BMRS_FINNAL;
						m_eMoveState = TwMoveState::BMS_RETRY;
					}
				}
				else
				{
					gUnit->ClrFlag(TwUnitFlag::BUF_ISBLOCKED);
					m_eState = TwMoveResult::MR_SUCCESS;
					m_eMoveState = TwMoveState::BMS_RETRY;
					gUnit->SetActionState(0);
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
			return BeExeResult::BER_EXE_END;
		}
		}
	}

	return BeExeResult::BER_TIME_OUT;
}
