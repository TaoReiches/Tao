/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TaskAttackItem.h"
#include "TW_TaskMoveToPos.h"
#include "TW_TaskActionAttack.h"
#include "TW_MemoryObject.h"
#include "TW_Main.h"
#include "TW_MapItem.h"
#include "TW_MapItemMgr.h"
#include "TW_Unit.h"

BeTaskAttackItem::BeTaskAttackItem()
{
	TaskType = TwTaskType::STT_ATTACK_ITEM;
	m_eState = BeAttackItemState::BAI_END;
	m_pkMoveToPos.reset(mpTaskMoveToPos.alloc());
	m_pkActionAttack.reset(mpTaskActionAttack.alloc());
	m_iItemID = 0;
}

BeTaskAttackItem::~BeTaskAttackItem()
{
	mpTaskMoveToPos.free(m_pkMoveToPos.get());
	m_pkMoveToPos.release();
	mpTaskActionAttack.free(m_pkActionAttack.get());
	m_pkActionAttack.release();
}

void BeTaskAttackItem::SetTargetID(int iID, float fDistance)
{
	if (m_iItemID == iID)
	{
		return;
	}
	m_iItemID = iID;
	const auto pkTarget = gMapItemMgr.GetMapItemByID(m_iItemID);
	m_pkMoveToPos->AttachMain(pkAttachMain);
	m_pkMoveToPos->AttachUnit(pAttachUnit);

	m_pkActionAttack->AttachMain(pkAttachMain);
	m_pkActionAttack->AttachUnit(pAttachUnit);

	m_pkMoveToPos->SetTargetPos(TwPos2(gUnit->GetPosX(), gUnit->GetPosY()), 50.0f);
	if (pkTarget)
	{
		m_pkMoveToPos->SetTargetPos(TwPos2(pkTarget->GetPosX(), pkTarget->GetPosY()), 0.0f);
	}
	gUnit->SetAttackingUnitID(0);

	// m_pkActionAttack->SetTargetItem(iID);
	m_eState = BeAttackItemState::BAI_TRACE;
}

int BeTaskAttackItem::GetTargetID() const
{
	return m_iItemID;
}

bool BeTaskAttackItem::IsAttacking() const
{
	return (bool)(m_eState == BeAttackItemState::BAI_ATTACK);
}

TwExeResult BeTaskAttackItem::Execute(int& iDeltaTime)
{
	TwTask::Execute(iDeltaTime);

	const auto pkTarget = gMapItemMgr.GetMapItemByID(m_iItemID);

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
		case BeAttackItemState::BAI_ATTACK:
		{
			TwExeResult eRet = m_pkActionAttack->Execute(iDeltaTime);
			if (eRet == TwExeResult::ER_TIME_OUT)
			{
				return TwExeResult::ER_TIME_OUT;
			}

			if (!pkTarget)
			{
				m_eState = BeAttackItemState::BAI_END;
				break;
			}

			float fDistance2 = GetDistance2(gUnit->GetPosX(), gUnit->GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
			float fD = 200.0f;
			if (fDistance2 > fD * fD)
			{
				m_eState = BeAttackItemState::BAI_TRACE;
			}
			break;
		}
		case BeAttackItemState::BAI_TRACE:
		{
			if (!pkTarget)
			{
				return TwExeResult::ER_EXE_END;
			}

			float fDistance2 = GetDistance2(gUnit->GetPosX(), gUnit->GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
			float fRange = 200.0f;
			if (fDistance2 <= (fRange * fRange))
			{
				m_eState = BeAttackItemState::BAI_ATTACK;
				break;
			}

			TwExeResult eRet = m_pkMoveToPos->Execute(iDeltaTime);
			if (eRet == TwExeResult::ER_TIME_OUT)
			{
				return TwExeResult::ER_TIME_OUT;
			}
			else
			{
				if (m_pkMoveToPos->GetMoveResult() == TwMoveResult::MR_SUCCESS)
				{
					m_eState = BeAttackItemState::BAI_ATTACK;
					break;
				}
				m_eState = BeAttackItemState::BAI_END;
			}
		}
		case BeAttackItemState::BAI_END:
		{
			return TwExeResult::ER_EXE_END;
		}
		default:break;
		}
	}
	return TwExeResult::ER_TIME_OUT;
}
