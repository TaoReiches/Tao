#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <set>
#include <vector>
#include <memory>
#include "TW_LogicBase.h"

static const float OutputRadius = 500.f;

class TwUnit;

class TwUnitOutput final : public BeMainPtr, public BeUnitPtr
{
public:
    TwUnitOutput();
    ~TwUnitOutput();

    void Update();
    void UpdateOutputFinished();

    const std::vector<std::shared_ptr<TwUnit>>& GetOutputNewUnits() const
    {
        return UnitsNeedAdd;
    }
    const std::vector<std::shared_ptr<TwUnit>>& GetOutputRemoveUnits() const
    {
        return UnitsNeedRemove;
    }
    const std::vector<std::shared_ptr<TwUnit>>& GetOutputUpdateUnits() const
    {
        return UnitsNeedUpdate;
    }

private:
    bool UnitInVector(const std::vector<std::shared_ptr<TwUnit>>& group, const std::shared_ptr<TwUnit>& unit) const;

private:
    std::set<std::shared_ptr<TwUnit>>       UnitsHasSent;
    std::vector<std::shared_ptr<TwUnit>>    UnitsNeedUpdate;
    std::vector<std::shared_ptr<TwUnit>>    UnitsNeedRemove;
    std::vector<std::shared_ptr<TwUnit>>    UnitsNeedAdd;
};
