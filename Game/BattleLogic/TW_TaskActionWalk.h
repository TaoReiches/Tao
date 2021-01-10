#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Task.h"
#include "TW_Pos2.h"

enum class TwMoveResult
{
    MR_NONE = 0,
    MR_INITED,
    MR_BLOCK,
    MR_SUCCESS,
};

enum class TwMoveState
{
    MS_INIT = 0,
    BMS_RETRY,
    BMS_STAND,
    BMS_WALK,
    BMS_END,
};

class TwTaskActionWalk : public TwTask
{
public:
    TwTaskActionWalk();
    ~TwTaskActionWalk();

public:
    void SetTargetPos(const TwPos2& kTargetPos, bool bTurn = false);
    bool IsBlocked(void);
    virtual TwExeResult Execute(int& iDeltaTime);

protected:
    bool            m_bStopAction;
    bool            m_bBlocked;
    TwPos2          m_kTargetPos;
    TwPos2          m_kSrcPos;
    int	            m_iWalkTime;
};
