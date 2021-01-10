#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_LogicBase.h"

enum class BeExeResult
{
	BER_TIME_OUT,
	BER_EXE_END,
	BER_ALL_OVER,
};

enum class BeTaskType
{
	STT_NONE = 0,

	STT_ACTION_STAND,
	STT_ACTION_WALK,
	STT_ACTION_ATTACK,
	STT_ACTION_SPELL,
	STT_ACTION_MORPH,
	STT_ACTION_DEATH,

	STT_MOVE_TO_POS,
	STT_MOVE_TO_UNIT,
	STT_ATTACK_UNIT,
	STT_ATTACK_TO_POS,
	STT_FOLLOW_UNIT,

	STT_ATTACK_ITEM,
};

class TwUnit;

class TwTask : public TwMainPtr, public TwUnitPtr
{
public:
	TwTask(void);
	virtual ~TwTask(void);
	BeTaskType				GetType();
	virtual BeExeResult		Execute(int& iDeltaTime) = 0;

protected:
	BeTaskType		m_eType;
	int				m_iTaskTime;
};
