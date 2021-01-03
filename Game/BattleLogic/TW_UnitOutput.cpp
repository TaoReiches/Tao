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
}
