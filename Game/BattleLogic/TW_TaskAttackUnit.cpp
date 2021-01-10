/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TaskAttackUnit.h"
#include "TW_MemoryObject.h"
#include "TW_TaskMoveToUnit.h"
#include "TW_TaskActionAttack.h"
#include "TW_Main.h"
#include "TW_Unit.h"
#include "TW_UnitMgr.h"

BeTaskAttackUnit::BeTaskAttackUnit()
{
	m_eType = BeTaskType::STT_ATTACK_UNIT;
	m_eState = BeAttackUnitState::BAU_END;

	m_iSkillTypeID = 0;
	m_iSkillLevel = 0;
	m_iTargetUnitID = 0;
	m_bIsOrb = false;
	m_iMoveTime = 0;

	m_pkMoveToUnit.reset(mpTaskMoveToUnit.alloc());
	m_pkActionAttack.reset(mpTaskActionAttack.alloc());
}

BeTaskAttackUnit::~BeTaskAttackUnit()
{
	mpTaskMoveToUnit.free(m_pkMoveToUnit.get());
	m_pkMoveToUnit.release();
	mpTaskActionAttack.free(m_pkActionAttack.get());
	m_pkActionAttack.release();
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
	if (gUnit->GetAttackingUnitID() == iID)
	{
		if (m_eState == BeAttackUnitState::BAU_END)
		{
			m_eState = BeAttackUnitState::BAU_TRACE;
		}
		return;
	}

	gUnit->SetAttackingUnitID(iID, bIsOrb);

	m_pkActionAttack->SetTargetID(iID, bIsOrb, m_iSkillTypeID, m_iSkillLevel);
	m_bIsOrb = bIsOrb;
	m_eState = BeAttackUnitState::BAU_TRACE;
}

int BeTaskAttackUnit::GetTargetID() const
{
	return gUnit->GetAttackingUnitID();
}

bool BeTaskAttackUnit::IsAttacking() const
{
	return (bool)(m_eState == BeAttackUnitState::BAU_ATTACK);
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
	TwTask::Execute(iDeltaTime);
	const std::shared_ptr<TwUnit> pkTarget = gUnitMgr->GetUnitByID(GetTargetID());

	if (!m_pkActionAttack || !m_pkMoveToUnit)
	{
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
		switch (m_eState)
		{
		case BeAttackUnitState::BAU_ATTACK:
		{
			m_iMoveTime = 0;
			BeExeResult eRet = m_pkActionAttack->Execute(iDeltaTime);
			if (m_pkActionAttack->IsPerDamageEnd())
			{
				if (pkTarget)
				{
					float fDistance2 = GetDistance2(gUnit->GetPosX(), gUnit->GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
					float fD = gUnit->GetAttackRange(pkTarget);
					if (fDistance2 > fD * fD)
					{
						m_pkMoveToUnit->SetTargetID(pkTarget->GetID(), fD);
						m_eState = BeAttackUnitState::BAU_TRACE;
						break;
					}
				}
			}
			if (eRet == BeExeResult::BER_TIME_OUT)
			{
				return BeExeResult::BER_TIME_OUT;
			}

			if (!pkTarget || !gUnit->UnitCanAttack(pkTarget, true, true))
			{
				m_eState = BeAttackUnitState::BAU_REFRESH;
				break;
			}
			float fDistance2 = GetDistance2(gUnit->GetPosX(), gUnit->GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
			float fD = gUnit->GetAttackRange(pkTarget);
			if (fDistance2 > fD * fD)
			{
				m_pkMoveToUnit->SetTargetID(pkTarget->GetID(), fD);
				m_eState = BeAttackUnitState::BAU_TRACE;
			}
			break;
		}
		case BeAttackUnitState::BAU_REFRESH:
		{
			if (gUnit->HasUnitCarryFlag(BUCF_CANNOTATTACK))
			{
				return BeExeResult::BER_TIME_OUT;
			}

			m_eState = BeAttackUnitState::BAU_TRACE;

			SetTargetID(pkTarget->GetID(), 0.0f, m_bIsOrb, m_iSkillTypeID, m_iSkillLevel);
		}
		case BeAttackUnitState::BAU_TRACE:
		{
			m_iMoveTime += iDeltaTime;
			if (pkTarget)
			{
				float fDistance2 = GetDistance2(gUnit->GetPosX(), gUnit->GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
				float fRange = gUnit->GetAttackRange(pkTarget);
				if (fDistance2 <= (fRange * fRange))
				{
					m_pkActionAttack->SetTargetID(0, m_bIsOrb, m_iSkillTypeID, m_iSkillLevel);
					m_pkActionAttack->SetTargetID(pkTarget->GetID(), m_bIsOrb, m_iSkillTypeID, m_iSkillLevel);
					m_eState = BeAttackUnitState::BAU_ATTACK;

					gUnit->SetActionState(0);
					break;
				}
			}

			BeExeResult eRet = BeExeResult::BER_EXE_END;
			if (gUnit->CanMove()) {
				eRet = m_pkMoveToUnit->Execute(iDeltaTime);
			}

			if (eRet == BeExeResult::BER_TIME_OUT)
			{
				return BeExeResult::BER_TIME_OUT;
			}
			else
			{
				if (m_pkMoveToUnit->GetMoveResult() == TwMoveResult::MR_SUCCESS && pkTarget)
				{
					m_pkActionAttack->SetTargetID(0, m_bIsOrb, m_iSkillTypeID, m_iSkillLevel);
					m_pkActionAttack->SetTargetID(pkTarget->GetID(), m_bIsOrb, m_iSkillTypeID, m_iSkillLevel);
					m_eState = BeAttackUnitState::BAU_ATTACK;
					break;
				}
				m_eState = BeAttackUnitState::BAU_END;
			}
		}
		case BeAttackUnitState::BAU_END:
		{
			m_iMoveTime = 0;
			m_eState = BeAttackUnitState::BAU_TRACE;
			return BeExeResult::BER_EXE_END;
		}
		default:
		{
			break;
		}
		}
	}
	return BeExeResult::BER_TIME_OUT;
}
