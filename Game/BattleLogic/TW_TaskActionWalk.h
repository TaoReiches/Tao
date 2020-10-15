#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Task.h"
#include "TW_Pos2.h"

class BeTaskActionWalk : public BeTask
{
protected:
    BeTaskActionWalk();
    ~BeTaskActionWalk();

public:
    void SetTargetPos(const TePos2& kTargetPos, bool bTurn = false);
    bool IsBlocked(void);
    virtual BeExeResult Execute(int& iDeltaTime);

protected:
    bool            m_bStopAction;
    bool            m_bBlocked;
    TePos2          m_kTargetPos;
    TePos2          m_kSrcPos;
    int	            m_iWalkTime;
};
