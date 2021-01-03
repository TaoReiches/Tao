/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_UnitOutput.h"
#include "TW_UnitMgr.h"
#include "TW_Unit.h"
#include "TW_Main.h"

TwUnitOutput::TwUnitOutput()
{

}

TwUnitOutput::~TwUnitOutput()
{

}

void TwUnitOutput::Update()
{
    // get all units in the output radius
    UnitGroup Group;
    gUnitMgr->GetAreaGroup(Group, gUnit->GetPosX(), gUnit->GetPosY(), OutputRadius);

    // new units that first appear
    for (auto& unit : Group)
    {
        if (UnitsHasSent.find(unit) == UnitsHasSent.end())
        {
            UnitsNeedAdd.push_back(unit);
        }
    }

    // units that need to remove
    for (auto iterHasSent = UnitsHasSent.begin(); iterHasSent != UnitsHasSent.end();)
    {
        if (!UnitInVector(Group, *iterHasSent))
        {
            UnitsNeedRemove.push_back(*iterHasSent);
            iterHasSent = UnitsHasSent.erase(iterHasSent);
        }
        else
        {
            ++iterHasSent;
        }
    }

    // units that need update
    for (auto& unit : UnitsHasSent)
    {
        if (unit->GetOutputFlag() != 0)
        {
            UnitsNeedUpdate.push_back(unit);
        }
    }
}

void TwUnitOutput::OutputFinished()
{
    for (auto unit : UnitsNeedAdd)
    {
        UnitsHasSent.insert(unit);
    }
    UnitsNeedAdd.clear();
    UnitsNeedRemove.clear();
    UnitsNeedUpdate.clear();
}

bool TwUnitOutput::UnitInVector(const std::vector<std::shared_ptr<TwUnit>>& group, const std::shared_ptr<TwUnit>& unit) const
{
    for (const auto& iterUnit : group)
    {
        if (iterUnit.get() == unit.get())
        {
            return true;
        }
    }
    return false;
}
