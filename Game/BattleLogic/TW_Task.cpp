/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Task.h"

TwTask::TwTask(void)
{
	TaskType = TwTaskType::STT_NONE;
	TaskTime = 0;
}

TwTask::~TwTask(void)
{
}

TwTaskType TwTask::GetType()
{
	return TaskType;
}

TwExeResult TwTask::Execute(int& iDeltaTime)
{
	TaskTime += iDeltaTime;
	return TwExeResult::ER_EXE_END;
}
