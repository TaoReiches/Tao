/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TaskActionWalk.h"
#include "TW_Functions.h"
#include "TW_Main.h"
#include "TW_Unit.h"
#include "TW_Map.h"

inline float f_line_y(float x, float x1, float y1, float x2, float y2)
{
	return ((y1 - y2) * x + (x1 * y2 - x2 * y1)) / (x1 - x2);
}

inline float f_line_x(float y, float x1, float y1, float x2, float y2)
{
	return ((x1 - x2) * y + (x2 * y1 - x1 * y2)) / (y1 - y2);
}

TwTaskActionWalk::TwTaskActionWalk()
{
	m_eType = BeTaskType::STT_ACTION_WALK;
	m_bBlocked = false;
	m_bStopAction = false;
	m_iWalkTime = 0;
}

TwTaskActionWalk::~TwTaskActionWalk()
{
	m_eType = BeTaskType::STT_ACTION_WALK;
	m_bBlocked = false;
	m_iWalkTime = 0;
	if (pkAttachUnit)
	{
		gUnit->SetActionState(0);
	}
}

void TwTaskActionWalk::SetTargetPos(const TwPos2& kTargetPos, bool bTurn)
{
	float fUnitX = gUnit->GetPosX();
	float fUnitY = gUnit->GetPosY();
	float fTarFace = atan2f(kTargetPos.fY - fUnitY, kTargetPos.fX - fUnitX);
	gUnit->SetTarFace(fTarFace);

	m_kTargetPos = kTargetPos;
	m_kSrcPos = TwPos2(fUnitX, fUnitY);
	m_iWalkTime = 0;

	gUnit->SetTarPosX(m_kTargetPos.fX);
	gUnit->SetTarPosY(m_kTargetPos.fY);
}

bool	TwTaskActionWalk::IsBlocked(void)
{
	return	m_bBlocked;
}

BeExeResult TwTaskActionWalk::Execute(int& iDeltaTime)
{
	TwTask::Execute(iDeltaTime);

	m_bBlocked = false;
	if (!gUnit->UnitCanMove(true))
	{
		if (gUnit->GetActionState() == 1)
		{
			gUnit->SetActionState(0);
		}
	}
	else
	{
		if (m_bStopAction)
		{
			m_bStopAction = false;
			gUnit->SetActionState(1);
		}
	}

	if (!gUnit->IsActionNow(BUA_WALK))
	{
		m_iWalkTime = 0;
		gUnit->SetUnitAction(BUA_WALK, BAN_walk);
	}

	float fUnitX = gUnit->GetPosX();
	float fUnitY = gUnit->GetPosY();

	gUnit->IncActionCurTime(iDeltaTime);

	float fMoveSpeed = gUnit->GetMoveSpeed();
	if (fMoveSpeed <= 0)
	{
		return BeExeResult::BER_EXE_END;
	}

	float fCanMoveDistance = fMoveSpeed * iDeltaTime / 1000.0f;
	float fMaxMoveDistance = 64.0f;

	float fWalkX = 0.0f;
	float fWalkY = 0.0f;

	auto iObs = TwGridFlag::TGF_FIXED_OTS;
	if (gUnit->GetClass() == UNIT_CLASSTYPE_SOLIDER)
	{
		iObs = TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_SOLIDER;
	}
	else
	{
		iObs = TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_UNIT;
	}

	int bRet = gMap.GetFirstCanStay(gUnit, m_kTargetPos.fX, m_kTargetPos.fY, fWalkX, fWalkY, fMaxMoveDistance, iObs);

	float fNeedMoveDistance = GetDistance(fUnitX, fUnitY, fWalkX, fWalkY);

	if (fNeedMoveDistance > fCanMoveDistance)
	{
		if (fNeedMoveDistance > 0.0f)
		{
			fWalkX = (float)(fUnitX + fCanMoveDistance * (fWalkX - fUnitX) / fNeedMoveDistance);
			fWalkY = (float)(fUnitY + fCanMoveDistance * (fWalkY - fUnitY) / fNeedMoveDistance);
		}
		else
		{
			fWalkX = (float)(fUnitX + fCanMoveDistance * (fWalkX - fUnitX) / 1.0f);
			fWalkY = (float)(fUnitY + fCanMoveDistance * (fWalkY - fUnitY) / 1.0f);
		}

		if (fabs(fWalkX - fUnitX) > fabs(fWalkY - fUnitY))
		{
			float fY = f_line_y(fWalkX, m_kSrcPos.fX, m_kSrcPos.fY, m_kTargetPos.fX, m_kTargetPos.fY);
			fWalkY = fY;
		}
		else
		{
			float fX = f_line_x(fWalkY, m_kSrcPos.fX, m_kSrcPos.fY, m_kTargetPos.fX, m_kTargetPos.fY);
			fWalkX = fX;
		}

		float fCanMoveX = fUnitX;
		float fCanMoveY = fUnitY;

		int bRet1 = gMap.GetFirstCanStay(gUnit, fWalkX, fWalkY, fCanMoveX, fCanMoveY, fMaxMoveDistance, iObs);
		if (fWalkX != fCanMoveX || fWalkY != fCanMoveY)
		{
			m_bBlocked = true;
		}
	}
	else
	{
		if (fWalkX != m_kTargetPos.fX || fWalkY != m_kTargetPos.fY)
		{
			m_bBlocked = true;
		}
	}

	if (m_bBlocked)
	{
		bool bFound = false;
		float afOffset[][2] =
		{
			{-1.0f, 0.0f},
			{1.0f, 0.0f},
			{0.0f, 1.0f},
			{0.0f, -1.0f},
			{-1.0f, 1.0f},
			{1.0f, -1.0f},
			{1.0f, 1.0f},
			{-1.0f, -1.0f},
		};
		float fOX = fWalkX;
		float fOY = fWalkY;

		float fCanMoveX = fUnitX;
		float fCanMoveY = fUnitY;

		float fDist2 = GetDistance2(fUnitX, fUnitY, m_kTargetPos.fX, m_kTargetPos.fY);
		for (int iRange = 0; iRange < fCanMoveDistance; ++iRange)
		{
			for (int i = 0; i < sizeof(afOffset) / sizeof(afOffset[0]); ++i)
			{
				float fX = fOX + afOffset[i][0] * (iRange + 1);
				float fY = fOY + afOffset[i][1] * (iRange + 1);
				int		iCheckRet = gMap.GetFirstCanStay(fX, fY, gUnit->GetOBSize(), m_kTargetPos.fX, m_kTargetPos.fY, fCanMoveX, fCanMoveY, fMaxMoveDistance, iObs);
				if (iCheckRet == 3 || iCheckRet == 5)
				{
					float fD2 = GetDistance2(fX, fY, m_kTargetPos.fX, m_kTargetPos.fY);
					if (fD2 < fDist2)
					{
						fDist2 = fD2;
						fWalkX = fX;
						fWalkY = fY;
						bFound = true;
					}
				}
			}

			if (bFound)
			{
				m_bBlocked = false;
				break;
			}
		}
	}

	if (m_bBlocked)
	{
		gUnit->SetActionState(0);
		m_iWalkTime = 0;
		return BeExeResult::BER_EXE_END;
	}

	int iActionTime = iDeltaTime;
	if (m_kTargetPos.fX != fWalkX || m_kTargetPos.fY != fWalkY)
	{
		iDeltaTime = 0;
	}
	else
	{
		if (fMoveSpeed > 0)
		{
			iActionTime = (int)(fNeedMoveDistance * 1000.0f / fMoveSpeed);
		}

		iDeltaTime -= iActionTime;
	}

	m_iWalkTime += iActionTime;
	if (m_iWalkTime > 200)
	{
		gUnit->SetFlag(TwUnitFlag::BUF_MOVING);
	}

	gUnit->SetActionState(1);

	gUnit->SetPosition(fWalkX, fWalkY, 0.0f, true);

	if (iDeltaTime > 0)
	{
		m_iWalkTime = 0;
		m_bBlocked = false;
		return BeExeResult::BER_EXE_END;
	}

	return BeExeResult::BER_TIME_OUT;
}
