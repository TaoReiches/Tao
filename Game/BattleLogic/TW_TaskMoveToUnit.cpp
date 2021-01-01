/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TaskMoveToUnit.h"
#include "TW_MemoryObject.h"
#include "TW_TaskMoveToPos.h"
#include "TW_Main.h"
#include "TW_Unit.h"
#include "TW_UnitMgr.h"

BeTaskMoveToUnit::BeTaskMoveToUnit()
{
	m_eType = BeTaskType::STT_MOVE_TO_UNIT;
	m_iTargetID = 0;
	m_fDistance = 0.0f;
	m_pkMoveToPos.reset(mpTaskMoveToPos.alloc());
}

BeTaskMoveToUnit::~BeTaskMoveToUnit()
{
	mpTaskMoveToPos.free(m_pkMoveToPos.get());
	m_pkMoveToPos.release();
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
		TwPos2 kTarPos = TwPos2(gUnit->GetPosX(), gUnit->GetPosY());
		m_pkMoveToPos->SetTargetPos(kTarPos);
	}

	std::shared_ptr<TwUnit> pkTarget = gUnitMgr->GetUnitByID(m_iTargetID);
	if (pkTarget)
	{
		TwPos2 kTarPos = TwPos2(pkTarget->GetPosX(), pkTarget->GetPosY());
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

	if (!gUnit->CanSpell())
	{
		return BeExeResult::BER_EXE_END;
	}

	std::shared_ptr<TwUnit> pkTarget = gUnitMgr->GetUnitByID(m_iTargetID);
	if (pkTarget
		&& !pkTarget->IsDead()
		&& (!(pkTarget->HasFlag(BUF_HASINVISIBLE))))
	{
		TwPos2 kTarPos = m_pkMoveToPos->GetTargetPos();
		float fDistance2 = GetDistance2(kTarPos.fX, kTarPos.fY, pkTarget->GetPosX(), pkTarget->GetPosY());
		if (fDistance2 > 2000.0f * 2000.0f)
		{
			m_iTargetID = -1;
		}
		else if (fDistance2 > 32.0f * 32.0f)
		{
			float fAllDistance = GetDistance2(gUnit->GetPosX(), gUnit->GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
			if (fAllDistance > 500.0f * 500.0f)
			{
				if (fDistance2 > 64.0f * 64.0f)
				{
					kTarPos = TwPos2(pkTarget->GetPosX(), pkTarget->GetPosY());
					m_pkMoveToPos->SetTargetPos(kTarPos, m_fDistance);
				}
			}
			else
			{
				kTarPos = TwPos2(pkTarget->GetPosX(), pkTarget->GetPosY());
				m_pkMoveToPos->SetTargetPos(kTarPos, m_fDistance);
			}
		}
	}
	else
	{
		TwPos2 kTarPos = m_pkMoveToPos->GetTargetPos();
		m_pkMoveToPos->SetTargetPos(kTarPos);
		std::shared_ptr<TwUnit> pkT = gUnitMgr->GetUnitByID(m_iTargetID, true);
		if (pkT && pkT->GetPlayer() == gUnit->GetPlayer())
		{
			return BeExeResult::BER_EXE_END;
		}
	}

	BeExeResult eRet = m_pkMoveToPos->Execute(iDeltaTime);
	return eRet;
}
