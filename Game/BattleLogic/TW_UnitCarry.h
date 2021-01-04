#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_UnitData.h"

class BeBuffer;
class BeSkill;
class BeMapItem;
class BeItem;
class BeCarry;

class TwUnitCarry : public TwUnitDataOperation
{
protected:
    TwUnitCarry(int iID);
    ~TwUnitCarry();
    std::vector<std::shared_ptr<BeSkill>>& GetNormalSkillVec();

    std::vector<std::shared_ptr<BeCarry>>		m_apkCarry;
    std::vector<std::shared_ptr<BeSkill>>		m_apkNormalSkill;
    std::vector<std::shared_ptr<BeItem>>        m_apkItem;
    std::vector<std::shared_ptr<BeBuffer>>      m_apkMultyBuffer;
    std::vector<std::shared_ptr<BeBuffer>>	    m_apkBuffer;
};

inline std::vector<std::shared_ptr<BeSkill>>& TwUnitCarry::GetNormalSkillVec()
{
    return m_apkNormalSkill;
}
