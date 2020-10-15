#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/



enum class BeMoveResult
{
	BMR_NONE = 0,
	BMR_INITED,
	BMR_BLOCK,
	BMR_SUCCESS,
};

enum class BeMoveState
{
	BMS_INIT = 0,
	BMS_RETRY,
	BMS_STAND,
	BMS_WALK,
	BMS_END,
};

enum class BeSpellPhase
{
	BSP_INIT = 0,
	BSP_PREPARE,
	BSP_CAST,
	BSP_EFFECT,
	BSP_SHAKES,
	BSP_FINISH,
	BSP_END,
};
