#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandExe.h"

class BeSpellCommand : public BeExeCommand
{
public:
	BeSpellCommand();
	~BeSpellCommand() {};

	void SpellTargetID(int iSkillTypeID, int iTargetID, const TwPos2& kPos, int iSkillLevel = 1, int iItemID = 0, bool bExpendMP = true, int iUsePlayer = -1, int iTargetType = 0);
	void SpellTargetItem(int iSkillTypeID, int iTargetID, int iSkillLevel = 1, int iItemID = 0, bool bExpendMP = true, int iUsePlayer = -1);
	void SpellTargetPos(int iSkillTypeID, const TwPos2& kPos, const TwPos2& kDirPos, int iSkillLevel = 1, int iItemID = 0, bool bExpendMP = true, int iUsePlayer = -1);

	virtual BeExeResult Execute(int& iDeltaTime);
	virtual bool CanHungUp(BeGiveCmdType eCmdType = BeGiveCmdType::BCT_IMMEDIATE, bool bNeedHangCurrent = true) const;
	virtual bool CanCancel() const;
	virtual bool CanInterrupt() const;
	virtual bool CanSkip(void) const;

protected:
	int 						m_iSkillTypeID;
	int							m_iSkillLevel;
	bool						m_bExpendMP;
	int							m_iTargetID;
	BeCommandTargetType			m_iTargetType;
	int							m_iItemID;
	int							m_iUsePlayer;
	TwPos2						m_kTargetPos;
	TwPos2						m_kDirectPos;
};
