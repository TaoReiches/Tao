/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_UnitBase.h"

TwUnitBase::TwUnitBase(int iID)
{
    Id = iID;
    m_iOtherFlag = 0;
    m_iImmunityFlag = 0;
    m_iCarryFlag = 0;
    m_iTypeID = 0;
    BlockIndex = 0;
    UnitFlag = BeUnitFlag::BUF_NULL;
    OutputFlag = TwUnitOutputFlag::BSUDCF_NULL;
    UserDataInt.clear();
    UserDataFloat.clear();
}

TwUnitBase::~TwUnitBase()
{

}
