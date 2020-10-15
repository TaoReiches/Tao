/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Task.h"

BeTask::BeTask(void)
{
	m_eType = BeTaskType::STT_NONE;
	m_iTaskTime = 0;
}

BeTask::~BeTask(void)
{
}

BeTaskType BeTask::GetType()
{
	return m_eType;
}

BeExeResult BeTask::Execute(int& iDeltaTime)
{
	m_iTaskTime += iDeltaTime;
	return BeExeResult::BER_EXE_END;
}
