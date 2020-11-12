/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_GameLog.h"

#include "Mapinfo_table.hpp"
#include "Buffer_table.hpp"
#include "Item_table.hpp"
#include "Skill_table.hpp"

int main()
{
    TW_LOG("Battle server start!");
    TW_WARN("Test warning message.");
    TW_ERROR("Test error message.");

    MapInfoTableMgr::Get();
    BufferTableMgr::Get();
    ItemTableMgr::Get();
    SkillTableMgr::Get();

    getchar();
}
