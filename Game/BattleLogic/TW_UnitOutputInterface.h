#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <vector>
#include <memory>

class TwUnit;
class TwUnitOutput;

class TwUnitOutputInterface
{
public:
    const std::vector<std::shared_ptr<TwUnit>>& GetOutputNewUnits() const;
    const std::vector<std::shared_ptr<TwUnit>>& GetOutputRemoveUnits() const;
    const std::vector<std::shared_ptr<TwUnit>>& GetOutputUpdateUnits() const;
    void UpdateOutputDataFinished();

protected:
    std::unique_ptr<TwUnitOutput>         UnitOutput;
};
