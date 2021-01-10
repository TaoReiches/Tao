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

TwTaskMoveToPos::TwTaskMoveToPos()
{
	TaskType = TwTaskType::STT_MOVE_TO_POS;
	MoveState = TwMoveState::MS_INIT;
	RetryState = TwMoveRetryState::MS_DITECT;
	PathFindResult = TwFindResult::TFR_NONE;
	ResultState = TwMoveResult::MR_NONE;
	Distance = 0.0f;
	StandTime = 0;
	WalkBlockTime = WALK_BLOCK_TIME;
	RetryTime = 0;
	WalkAction.reset(mpTaskActionWalk.alloc());
}

TwTaskMoveToPos::~TwTaskMoveToPos()
{
	mpTaskActionWalk.free(WalkAction.get());
	WalkAction.release();
};

void TwTaskMoveToPos::SetTargetPos(const TwPos2& kPos, float fDistance, bool bTurn)
{
	Distance = fDistance;
	TargetPos = kPos;
	MiddlePos = TargetPos;
	ResultState = TwMoveResult::MR_INITED;
	MoveState = TwMoveState::MS_INIT;
	StandTime = 0;
	WalkAction->AttachMain(pkAttachMain);
	WalkAction->AttachUnit(pAttachUnit);
}

TwPos2 TwTaskMoveToPos::GetTargetPos() const
{
	return TargetPos;
}

TwMoveResult TwTaskMoveToPos::GetMoveResult(void) const
{
	return ResultState;
}

void TwTaskMoveToPos::SetMoveResult(TwMoveResult eResult)
{
	ResultState = eResult;
}

TwExeResult TwTaskMoveToPos::Execute(int& iDeltaTime)
{
	TwTask::Execute(iDeltaTime);

	if (!pAttachUnit)
	{
		return TwExeResult::ER_TIME_OUT;
	}
	if (GetDistance2(gUnit->GetPosX(), gUnit->GetPosY(), TargetPos.fX, TargetPos.fY) <= (Distance * Distance))
	{
		ResultState = TwMoveResult::MR_SUCCESS;
		MoveState = TwMoveState::MS_INIT;
		gUnit->SetActionState(0);
		return TwExeResult::ER_EXE_END;
	}

	if (gUnit->GetClass() == UNIT_CLASSTYPE_BUILDING)
	{
		return TwExeResult::ER_EXE_END;
	}
	if (gUnit->HasUnitCarryFlag(BUCF_CANNOTMOVE))
    {
		return TwExeResult::ER_EXE_END;
	}
	if (ResultState == TwMoveResult::MR_NONE)
	{
		MoveState = TwMoveState::MS_INIT;
		return TwExeResult::ER_EXE_END;
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
		switch (MoveState)
		{
		case TwMoveState::MS_INIT:
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
			StandTime = 0;
			RetryTime = 0;
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

			PathFindResult = gMap.FindPath(akPath, gUnit, TargetPos.fX, TargetPos.fY, Distance, iObs);

			if (PathFindResult == TwFindResult::TFR_NONE || !gUnit->CanMove())
			{
				WalkAction->SetTargetPos(TargetPos, true);
				MoveState = TwMoveState::BMS_WALK;
				RetryState = TwMoveRetryState::MS_DITECT;
			}
			else if (PathFindResult == TwFindResult::TFR_ARRIVED)
			{
				ResultState = TwMoveResult::MR_SUCCESS;
				MoveState = TwMoveState::BMS_END;
			}
			else
			{
				if (akPath.empty())
				{
					MoveState = TwMoveState::BMS_STAND;
				}
				else
				{
					DirectPos = akPath.front();
					akPath.pop_front();

					MiddlePos = akPath.size() ? akPath.front() : TargetPos;
					WalkAction->AttachMain(pkAttachMain);
					WalkAction->AttachUnit(pAttachUnit);
					WalkAction->SetTargetPos(DirectPos);
					MoveState = TwMoveState::BMS_WALK;
					RetryState = TwMoveRetryState::MS_DITECT;
				}
			}
			break;
		}
		case TwMoveState::BMS_RETRY:
		{
			if (!gUnit->CanMove())
			{
				WalkBlockTime = 200;
				MoveState = TwMoveState::BMS_STAND;
				StandTime = 0;
				RetryTime = 0;
				break;
			}

			++RetryTime;
			if (gUnit->GetClass() == UNIT_CLASSTYPE_SOLIDER)
			{
				gMap.SetUnitPosition(gUnit, gUnit->GetPosX(), gUnit->GetPosY(), 0.0f, 1000.0f, false, TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_SOLIDER, TwGridFlag::TGF_NONE, true);
			}
			else
			{
				gMap.SetUnitPosition(gUnit, gUnit->GetPosX(), gUnit->GetPosY(), 0.0f, 1000.0f, false, TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_UNIT, TwGridFlag::TGF_NONE, true);
			}

			gMap.DelUnitObstacle(gUnit, false);
			if (RetryTime > WALK_BLOCK_COUNT)
			{
				if (RetryState == TwMoveRetryState::MS_DITECT)
				{
					RetryTime = 0;
					RetryState = TwMoveRetryState::MS_MIDDLE;
					MoveState = TwMoveState::BMS_STAND;
				}
				else if (RetryState == TwMoveRetryState::MS_MIDDLE)
				{
					RetryState = TwMoveRetryState::MS_FINNAL;
					MoveState = TwMoveState::BMS_STAND;
				}
				else
				{
					ResultState = TwMoveResult::MR_BLOCK;
					MoveState = TwMoveState::BMS_END;
				}
				break;
			}

			StandTime = 0;
			WalkBlockTime = HERO_WALK_BLOCK_TIME;

			TwPos2 kFindPos = TargetPos;
			switch (RetryState)
			{
			case TwMoveRetryState::MS_DITECT:
			{
				float fCanMoveDistance = gUnit->GetMoveSpeed() * iDeltaTime / 1000.0f;
				float fMaxMoveDistance = 128.0f;

				float fWalkX = 0.0f;
				float fWalkY = 0.0f;
				if (gUnit->GetClass() == UNIT_CLASSTYPE_SOLIDER)
				{
					gMap.GetFirstCanStay(gUnit, DirectPos.fX, DirectPos.fY, fWalkX, fWalkY, fMaxMoveDistance, TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_SOLIDER_COLLION | TwGridFlag::TGF_TEMP);
				}
				else
				{
					gMap.GetFirstCanStay(gUnit, DirectPos.fX, DirectPos.fY, fWalkX, fWalkY, fMaxMoveDistance, TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_UNIT_OTS | TwGridFlag::TGF_SOLIDER);
				}

				float fNeedMoveDistance = GetDistance(gUnit->GetPosX(), gUnit->GetPosY(), fWalkX, fWalkY);
				if (fNeedMoveDistance > fCanMoveDistance)
				{
					WalkAction->SetTargetPos(DirectPos);
					MoveState = TwMoveState::BMS_WALK;
					break;
				}

				RetryState = TwMoveRetryState::MS_MIDDLE;
			}
			case TwMoveRetryState::MS_MIDDLE:
			{
				kFindPos = MiddlePos;
			}
			case TwMoveRetryState::MS_FINNAL:
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

				PathFindResult = gMap.FindPath(akPath, gUnit, TargetPos.fX, TargetPos.fY, Distance, iObs);

				if (PathFindResult == TwFindResult::TFR_NONE)
				{
					RetryState = TwMoveRetryState::MS_FINNAL;
					MoveState = TwMoveState::BMS_STAND;
				}
				else if (PathFindResult == TwFindResult::TFR_ARRIVED)
				{
					RetryTime = 0;
					if (RetryState == TwMoveRetryState::MS_FINNAL || TargetPos == MiddlePos)
					{
						ResultState = TwMoveResult::MR_SUCCESS;
						MoveState = TwMoveState::BMS_END;
					}
					else
					{
						MoveState = TwMoveState::BMS_STAND;
						RetryState = TwMoveRetryState::MS_FINNAL;
					}
				}
				else
				{
					DirectPos = akPath.front();
					akPath.pop_front();

					if (RetryState == TwMoveRetryState::MS_FINNAL || TargetPos == MiddlePos)
					{
						MiddlePos = akPath.size() ? akPath.front() : TargetPos;
					}
					WalkAction->SetTargetPos(DirectPos);
					MoveState = TwMoveState::BMS_WALK;
					RetryState = TwMoveRetryState::MS_DITECT;
				}

			}break;
			}
			break;
		}
		case TwMoveState::BMS_STAND:
		{
			if ((StandTime + iDeltaTime) < WalkBlockTime)
			{
				gUnit->IncActionCurTime(iDeltaTime);
				StandTime += iDeltaTime;
				iDeltaTime = 0;
			}
			else
			{
				int iStandTime = (WalkBlockTime - StandTime);
				gUnit->IncActionCurTime(iStandTime);
				iDeltaTime -= iStandTime;
				MoveState = TwMoveState::BMS_RETRY;
			}
			break;
		}
		case TwMoveState::BMS_WALK:
		{
			TwExeResult eRet = WalkAction->Execute(iDeltaTime);
			if (eRet == TwExeResult::ER_EXE_END)
			{
				if (WalkAction->IsBlocked())
				{
					gUnit->SetFlag(TwUnitFlag::BUF_ISBLOCKED);
					MoveState = TwMoveState::BMS_STAND;
					RetryState = TwMoveRetryState::MS_DITECT;
					if (RetryTime == 0)
					{
						RetryState = TwMoveRetryState::MS_FINNAL;
						MoveState = TwMoveState::BMS_RETRY;
					}
				}
				else
				{
					gUnit->ClrFlag(TwUnitFlag::BUF_ISBLOCKED);
					ResultState = TwMoveResult::MR_SUCCESS;
					MoveState = TwMoveState::BMS_RETRY;
					gUnit->SetActionState(0);
				}
			}
			else
			{
				RetryTime = 0;
			}
			break;
		}
		default:
		{
			return TwExeResult::ER_EXE_END;
		}
		}
	}

	return TwExeResult::ER_TIME_OUT;
}
