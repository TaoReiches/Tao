/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_GameLog.h"

#include "Mapinfo_table.hpp"
#include "Buffer_table.hpp"

int main()
{
    TW_LOG("Battle server start!");
    TW_WARN("Test warning message.");
    TW_ERROR("Test error message.");

    MapInfoTableMgr::Get();
    BufferTableMgr::Get();

    getchar();
}
