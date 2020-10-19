/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <algorithm>
#include "TW_CarrySkill.h"
#include "TW_MemoryObject.h"
#include "TW_Skill.h"

BeCarrySkill::BeCarrySkill(int iID) : BeCarry(iID)
{
}

BeCarrySkill::~BeCarrySkill()
{
	for (auto skillIter = m_akSkill.begin(); skillIter != m_akSkill.end(); ++skillIter)
	{
		auto& pkSkill = *skillIter;
		if (pkSkill)
		{
			mpSkill.free(pkSkill.get());
			pkSkill.release();
		}
	}
	m_akSkill.clear();
}
