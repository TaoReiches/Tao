/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_UnitOutputInterface.h"
#include "TW_UnitOutput.h"

const std::vector<std::shared_ptr<TwUnit>>& TwUnitOutputInterface::GetOutputNewUnits() const
{
    return UnitOutput->GetOutputNewUnits();
}

const std::vector<std::shared_ptr<TwUnit>>& TwUnitOutputInterface::GetOutputRemoveUnits() const
{
    return UnitOutput->GetOutputRemoveUnits();
}

const std::vector<std::shared_ptr<TwUnit>>& TwUnitOutputInterface::GetOutputUpdateUnits() const
{
    return UnitOutput->GetOutputUpdateUnits();
}

void TwUnitOutputInterface::UpdateOutputDataFinished()
{
    UnitOutput->UpdateOutputFinished();
}
