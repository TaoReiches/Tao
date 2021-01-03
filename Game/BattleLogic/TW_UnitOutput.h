#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <list>
#include <memory>
#include "TW_LogicBase.h"

static const float OutputRadius = 500.f;

class TwUnit;

class UnitOutput final : public BeMainPtr
{
public:
    UnitOutput();
    ~UnitOutput();

    void Update();

private:
    std::list<std::shared_ptr<TwUnit>>      UnitsHasSent;
    std::list<std::shared_ptr<TwUnit>>      UnitsNeedSend;
    std::list<std::shared_ptr<TwUnit>>      UnitsNeedRemove;
};
