#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include "TW_Task.h"
#include <TW_Pos2.h>
#include "TW_TaskActionWalk.h"
#include <TW_MapDefine.h>

enum class TwMoveRetryState
{
    MS_DITECT = 0,
    MS_MIDDLE,
    MS_FINNAL,
    MS_CHECK,
};

class TwTaskMoveToPos : public TwTask
{
public:
    TwTaskMoveToPos();
    ~TwTaskMoveToPos();
    TwMoveResult GetMoveResult(void) const;
    void SetMoveResult(TwMoveResult eResult);
    void SetTargetPos(const TwPos2& kPos, float fDistance = 0.0f, bool bTurn = false);
    TwPos2 GetTargetPos() const;
    virtual TwExeResult Execute(int& iDeltaTime);

protected:
    TwMoveState MoveState;
    int StandTime;
    int WalkBlockTime;
    int RetryTime;
    TwMoveResult ResultState;
    TwMoveRetryState RetryState;
    TwFindResult PathFindResult;
    TwPos2 TargetPos;
    TwPos2 MiddlePos;
    TwPos2 DirectPos;
    float Distance;
    std::unique_ptr<TwTaskActionWalk> WalkAction;
};
