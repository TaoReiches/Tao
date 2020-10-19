/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Equip.h"

BeEquip::BeEquip(int iID) : BeCarrySkill(iID)
{
	m_eType = BCT_EQUIP;
	m_iSuitNumber = 0;
	m_iRandomID = 0;
}

BeEquip::~BeEquip(void)
{
}

void BeEquip::SetSuitNumber(int iNumber)
{
	m_iSuitNumber = iNumber;
}

void BeEquip::SetRandomID(int iID)
{
	m_iRandomID = iID;
}

bool BeEquip::Initialize(int iTypeID)
{
	m_iTypeID = iTypeID;

	return false;
}

bool BeEquip::Strengthen(int iCardID, int iStrengthenID)
{
	return true;
}

