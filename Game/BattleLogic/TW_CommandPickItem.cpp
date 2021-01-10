/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandPickItem.h"
#include "TW_Main.h"
#include "TW_MapItem.h"
#include "TW_MapItemMgr.h"
#include "TW_TaskMoveToPos.h"
#include "TW_Unit.h"
#include "TW_MemoryObject.h"

BePickItemCommand::BePickItemCommand()
{
	m_eCmdType = TwCommandType::BCT_PICK_ITEM;
	m_iItemID = 0;
}

bool BePickItemCommand::SetItemID(int iItemID)
{
	m_iItemID = iItemID;
	auto pkMapItem = gMapItemMgr.GetMapItemByID(m_iItemID);
	if (pkMapItem)
	{
		m_kTargetPos.fX = pkMapItem->GetPosX();
		m_kTargetPos.fY = pkMapItem->GetPosY();
	}
	else
	{
		return false;
	}

	SafeDeleteTask(m_pkCurTask);
	m_pkCurTask.reset(dynamic_cast<TwTask*>(mpTaskMoveToPos.alloc()));
	m_pkCurTask->AttachMain(pkAttachMain);
	m_pkCurTask->AttachUnit(pAttachUnit);

	((TwTaskMoveToPos*)m_pkCurTask.get())->SetTargetPos(m_kTargetPos, 150.0f);
	return true;
}

TwExeResult BePickItemCommand::Execute(int& iDeltaTime)
{
	BeExeCommand::Execute(iDeltaTime);
	if (m_pkCurTask->Execute(iDeltaTime) == TwExeResult::ER_EXE_END)
	{
		if (m_pkCurTask->GetType() == TwTaskType::STT_MOVE_TO_POS)
		{
			if (((TwTaskMoveToPos*)m_pkCurTask.get())->GetMoveResult() == TwMoveResult::MR_SUCCESS)
			{
				auto pkMapItem = gMapItemMgr.GetMapItemByID(m_iItemID);
				if (!pkMapItem)
				{
					return TwExeResult::ER_EXE_END;
				}

				gUnit->PickMapItem(pkMapItem.get());
				return TwExeResult::ER_EXE_END;
			}
		}
	}
	return TwExeResult::ER_TIME_OUT;
}

bool BePickItemCommand::CanHungUp(TwGiveCmdType eCmdType, bool bNeedHangCurrent) const
{
	if (!m_pkCurTask)
	{
		return false;
	}

	if (m_pkCurTask->GetType() == TwTaskType::STT_MOVE_TO_POS || m_pkCurTask->GetType() == TwTaskType::STT_MOVE_TO_UNIT)
	{
		return true;
	}
	return false;
}

bool BePickItemCommand::CanCancel() const
{
	return true;
}

bool BePickItemCommand::CanInterrupt() const
{
	return true;
}
