/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_UnitBlockMgr.h"
#include "TW_Main.h"
#include "TW_Map.h"
#include "TW_UnitMgr.h"
#include "TW_Unit.h"

void TwUnitBlockMgr::InitializeBlocks()
{
    BlockSize = 1024;
    m_iBlocksW = (int)gMap.GetWidth() / BlockSize;
    m_iBlocksH = (int)gMap.GetHeight() / BlockSize;

    int iBlocks = m_iBlocksW * m_iBlocksH;
    Blocks.resize(iBlocks);
}

void TwUnitBlockMgr::UpdateUnitBlocks()
{
    const auto& allUnits = gUnitMgr->GetID2Unit();
    for (auto& unit : allUnits)
    {
        auto blockX = static_cast<int>(unit.second->GetPosX()) / BlockSize;
        auto blockY = static_cast<int>(unit.second->GetPosY()) / BlockSize;
        auto blockIndex = blockY * m_iBlocksW + blockX;
        auto oldIndex = unit.second->GetBlockIndex();
        if (oldIndex != blockIndex)
        {
            if (oldIndex != -1)
            {
                Blocks.at(oldIndex).remove(unit.second);
            }
            Blocks.at(blockIndex).push_back(unit.second);
            unit.second->SetBlockIndex(blockIndex);
        }
    }
}

void TwUnitBlockMgr::GetBlockArea(float fX, float fY, float fRadius, int& iBX, int& iBY, int& iEX, int& iEY) const
{
    iBX = static_cast<int>(fX - fRadius) / BlockSize;
    iBY = static_cast<int>(fY - fRadius) / BlockSize;
    iEX = static_cast<int>(fX + fRadius) / BlockSize;
    iEY = static_cast<int>(fY + fRadius) / BlockSize;

    iBX = iBX < 0 ? 0 : iBX;
    iBY = iBY < 0 ? 0 : iBY;
    iEX = iEX >= m_iBlocksW ? m_iBlocksW - 1 : iEX;
    iEY = iEY >= m_iBlocksH ? m_iBlocksH - 1 : iEY;
}
