/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TaskAttackToPos.h"
#include "TW_TaskActionAttack.h"
#include "TW_TaskMoveToPos.h"
#include "TW_MemoryObject.h"
#include "TW_Functions.h"
#include "TW_Unit.h"
#include "TW_UnitMgr.h"
#include "TW_Main.h"

BeTaskAttackToPos::BeTaskAttackToPos()
{
	m_eType = BeTaskType::STT_ATTACK_TO_POS;

	m_fRange = 0.0f;
	m_iRetryTime = 0;
	m_pkAttack.reset(mpTaskActionAttack.alloc());
	m_pkMoveToPos.reset(mpTaskMoveToPos.alloc());
	m_eState = BeAttackToPosState::BAP_REFRESH;
	m_dwMoveTime = 0;
	m_dwOutSightTime = 0;
	m_iStartTime = 0;
	m_iStartAttackTime = 0;
}

BeTaskAttackToPos::~BeTaskAttackToPos()
{
	mpTaskActionAttack.free(m_pkAttack.get());
	m_pkAttack.release();
	mpTaskMoveToPos.free(m_pkMoveToPos.get());
	m_pkMoveToPos.release();
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
void BeTaskAttackToPos::SetTargetPos(const TwPos2& kPos, float fRange)
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
		return BeExeResult::BER_EXE_END;
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
		case BeAttackToPosState::BAP_MOVE:
		{
			m_iStartAttackTime = 0;
			m_dwMoveTime += iDeltaTime;
			BeExeResult eRet = m_pkMoveToPos->Execute(iDeltaTime);

			if (gUnit->GetCommandCount() > 0)
			{
				float fDistance2 = GetDistance2(gUnit->GetPosX(), gUnit->GetPosY(), m_kTarPos.fX, m_kTarPos.fY);
				if (fDistance2 < m_fRange * m_fRange)
				{
					m_dwMoveTime = 0;
					m_iRetryTime = 0;
					m_eState = BeAttackToPosState::BAP_END;
				}
			}

			if (eRet == BeExeResult::BER_TIME_OUT)
			{
				return BeExeResult::BER_TIME_OUT;
			}
			else
			{
				m_dwMoveTime -= iDeltaTime;
				if (m_pkMoveToPos->GetMoveResult() == BeMoveResult::BMR_SUCCESS)
				{
					m_dwMoveTime = 0;
					m_iRetryTime = 0;
				}
				else
				{
					++m_iRetryTime;
				}
				m_eState = BeAttackToPosState::BAP_END;
			}
			break;
		}
		case BeAttackToPosState::BAP_REFRESH:
		{
			m_dwMoveTime = 0;
			if (gUnit->GetAttackingUnitID())
			{
				std::shared_ptr<TwUnit> pkOrgTarget = gUnitMgr->GetUnitByID(gUnit->GetAttackingUnitID());
				if (pkOrgTarget && !pkOrgTarget->IsDead() && (!pkOrgTarget->HasFlag(BUF_HASINVISIBLE)))
				{
					TwPos2 kPos;
					kPos.fX = pkOrgTarget->GetPosX();
					kPos.fY = pkOrgTarget->GetPosY();
					float fDistance = gUnit->GetAttackRange(pkOrgTarget);
					m_pkMoveToPos->SetTargetPos(kPos, fDistance);
					m_pkAttack->SetTargetID(gUnit->GetAttackingUnitID());
					m_eState = BeAttackToPosState::BAP_TRACE;
					break;
				}
			}
			else
			{
				m_pkAttack->SetTargetID(0);
				TwPos2 kPos = m_pkMoveToPos->GetTargetPos();
				if (m_kTarPos.fX != kPos.fX || m_kTarPos.fY != kPos.fY)
				{
					m_pkMoveToPos->SetTargetPos(m_kTarPos);
				}
				m_eState = BeAttackToPosState::BAP_MOVE;
			}
			break;
		}
		case BeAttackToPosState::BAP_TRACE:
		{
			{
				std::shared_ptr<TwUnit> pkTarget = gUnitMgr->GetUnitByID(gUnit->GetAttackingUnitID());

				if (pkTarget && !pkTarget->IsDead())
				{
						m_dwOutSightTime = 0;
						TwPos2 kPos = m_pkMoveToPos->GetTargetPos();
						if (fabs(kPos.fX - pkTarget->GetPosX()) > 16.0f || fabs(kPos.fY - pkTarget->GetPosY()) > 16.0f)
						{
							kPos.fX = pkTarget->GetPosX();
							kPos.fY = pkTarget->GetPosY();
							float fDistance = gUnit->GetAttackRange(pkTarget);
							m_pkMoveToPos->SetTargetPos(kPos, fDistance);
						}

					m_dwOutSightTime += iDeltaTime;
					m_dwMoveTime += iDeltaTime;
					BeExeResult eRet = m_pkMoveToPos->Execute(iDeltaTime);

					if (eRet == BeExeResult::BER_TIME_OUT)
					{
					}
					else
					{
						m_dwMoveTime -= iDeltaTime;
						if (m_pkMoveToPos->GetMoveResult() == BeMoveResult::BMR_SUCCESS)
						{
							m_dwMoveTime = 0;
							m_iRetryTime = 0;
							m_eState = BeAttackToPosState::BAP_ATTACK;
						}
						else
						{
							++m_iRetryTime;
							m_eState = BeAttackToPosState::BAP_REFRESH;
						}
					}
				}
				else
				{
					m_eState = BeAttackToPosState::BAP_REFRESH;
				}
			}
			if (m_dwOutSightTime > 2000)
			{
				m_dwOutSightTime = 0;
				m_eState = BeAttackToPosState::BAP_REFRESH;
			}
			break;
		}
		case BeAttackToPosState::BAP_ATTACK:
		{
			m_iStartAttackTime += iDeltaTime;
			BeExeResult eRet = m_pkAttack->Execute(iDeltaTime);
			if (eRet == BeExeResult::BER_EXE_END)
			{
				std::shared_ptr<TwUnit> pkTarget = gUnitMgr->GetUnitByID(gUnit->GetAttackingUnitID());
				if (pkTarget && pkTarget->GetClass() == UNIT_CLASSTYPE_BUILDING)
				{
					m_eState = BeAttackToPosState::BAP_REFRESH;
					break;
				}
				{
					if (!pkTarget || !gUnit->UnitCanAttack(pkTarget, true, true))
					{
						m_eState = BeAttackToPosState::BAP_REFRESH;
						break;
					}

					float fDistance2 = GetDistance2(gUnit->GetPosX(), gUnit->GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
					float fRange = gUnit->GetAttackRange(pkTarget);
					if (fDistance2 > fRange * fRange)
					{
						TwPos2 kPos(gUnit->GetPosX(), gUnit->GetPosY());
						m_pkMoveToPos->SetTargetPos(kPos, fRange);
						m_eState = BeAttackToPosState::BAP_TRACE;
					}
				}
			}
			break;
		}
		case BeAttackToPosState::BAP_END:
		{
			m_eState = BeAttackToPosState::BAP_REFRESH;
			return BeExeResult::BER_EXE_END;
		}
		default:break;
		}
	}
	return BeExeResult::BER_TIME_OUT;
}
