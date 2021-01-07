/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_UnitBlockMgr.h"
#include "TW_Main.h"
#include "TW_Map.h"

void TwUnitBlockMgr::Initialize()
{
    m_iBlocksW = (int)gMap.GetWidth() / BLOCK_ELE_SIZE;
    m_iBlocksH = (int)gMap.GetHeight() / BLOCK_ELE_SIZE;

    int iBlocks = m_iBlocksW * m_iBlocksH;
    Blocks.resize(iBlocks);
}

void TwUnitBlockMgr::UpdateUnitBlocks()
{

}

void TwUnitBlockMgr::GetBlockArea(float fX, float fY, float fRadius, int& iBX, int& iBY, int& iEX, int& iEY) const
{
    iBX = static_cast<int>(fX - fRadius) / BLOCK_ELE_SIZE;
    iBY = static_cast<int>(fY - fRadius) / BLOCK_ELE_SIZE;
    iEX = static_cast<int>(fX + fRadius) / BLOCK_ELE_SIZE;
    iEY = static_cast<int>(fY + fRadius) / BLOCK_ELE_SIZE;

    iBX = iBX < 0 ? 0 : iBX;
    iBY = iBY < 0 ? 0 : iBY;
    iEX = iEX >= m_iBlocksW ? m_iBlocksW - 1 : iEX;
    iEY = iEY >= m_iBlocksH ? m_iBlocksH - 1 : iEY;
}
