#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_LogicBase.h"

enum BeExeResult
{
	BER_TIME_OUT,
	BER_EXE_END,
	BER_ALL_OVER,
};

enum BeMoveResult
{
	BMR_NONE = 0,
	BMR_INITED,
	BMR_BLOCK,
	BMR_SUCCESS,
};

enum BeMoveState
{
	BMS_INIT = 0,
	BMS_RETRY,
	BMS_STAND,
	BMS_WALK,
	BMS_END,
};

enum BeSpellPhase
{
	BSP_INIT = 0,
	BSP_PREPARE,
	BSP_CAST,
	BSP_EFFECT,
	BSP_SHAKES,
	BSP_FINISH,
	BSP_END,
};

enum BeTaskType
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

class BeUnit;

class BeTask : public BeMainPtr, public BeUnitPtr
{
protected:
	BeTask(void);
	virtual ~BeTask(void);

public:
	BeTaskType GetType();
	virtual BeExeResult Execute(int& iDeltaTime) = 0;

protected:
	BeTaskType	m_eType;
	int m_iTaskTime;
};
