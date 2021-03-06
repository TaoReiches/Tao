#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <vector>
#include <list>
#include <memory>
#include "TW_LogicBase.h"

class TwUnit;

class TwUnitBlockMgr : public TwMainPtr
{
public:
    void InitializeBlocks();
    void UpdateUnitBlocks();
    void GetBlockArea(float fX, float fY, float fRadius, int& iBX, int& iBY, int& iEX, int& iEY) const;

protected:
    std::vector<std::list<std::shared_ptr<TwUnit>>>     Blocks;
    int         m_iBlocksW;
    int         m_iBlocksH;
    int         BlockSize;
};
