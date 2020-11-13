#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Task.h"
#include "TW_Pos2.h"

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

class BeTaskActionWalk : public BeTask
{
public:
    BeTaskActionWalk();
    ~BeTaskActionWalk();

public:
    void SetTargetPos(const TwPos2& kTargetPos, bool bTurn = false);
    bool IsBlocked(void);
    virtual BeExeResult Execute(int& iDeltaTime);

protected:
    bool            m_bStopAction;
    bool            m_bBlocked;
    TwPos2          m_kTargetPos;
    TwPos2          m_kSrcPos;
    int	            m_iWalkTime;
};
