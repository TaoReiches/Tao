#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include "TW_Carry.h"

class BeSkill;

class BeCarrySkill : public BeCarry
{
public:
    BeCarrySkill(int iID);
    ~BeCarrySkill();

    const std::vector<std::unique_ptr<BeSkill>>& GetAllSkill(void) const;

protected:
    std::vector<std::unique_ptr<BeSkill>>	m_akSkill;
};

inline const std::vector<std::unique_ptr<BeSkill>>& BeCarrySkill::GetAllSkill(void) const
{
    return m_akSkill;
}
