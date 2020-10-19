#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CarrySkill.h"

class BeEquip : public BeCarrySkill
{
public:
    BeEquip(int iID);
    ~BeEquip(void);
    virtual	bool Initialize(int iTypeID);
    bool	Strengthen(int iCardID, int iStrengthenID);
    void	SetSuitNumber(int iNumber);
    void	SetRandomID(int iID);

protected:
    int		m_iSuitNumber;
    int		m_iRandomID;
};
