/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Task.h"

TwTask::TwTask(void)
{
	m_eType = BeTaskType::STT_NONE;
	m_iTaskTime = 0;
}

TwTask::~TwTask(void)
{
}

BeTaskType TwTask::GetType()
{
	return m_eType;
}

BeExeResult TwTask::Execute(int& iDeltaTime)
{
	m_iTaskTime += iDeltaTime;
	return BeExeResult::BER_EXE_END;
}
