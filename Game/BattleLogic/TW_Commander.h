#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <list>
#include "TW_Command.h"
#include "TW_LogicBase.h"

class BeExeCommand;

class TwCommander : public TwMainPtr, public TwUnitPtr
{
public:
	TwCommander();
	~TwCommander();

	BeExeCommand* GetCurCmd(void);
	bool	IsIdle(void) const;

	void	GiveCommand(TwCommand kCmd, TwGiveCmdType eType = TwGiveCmdType::BCT_IMMEDIATE, int iUnitID = 0, bool bNeedHangCurrent = true);
	void	ExecuteCmd(int iDeltaTime);
	int		GetCommandCount() const;
	TwCommand& GetCurCommand(void);
	std::list<TwCommand>& GetCommands(void);

	void    SetCommands(const std::list<TwCommand>& rkCommands);

	void	OnDelete(void);
	void    ClearAllCommands();

protected:
	bool	SwitchCmd(const TwCommand& kCmd, bool bConnect = true);
	bool	GoNextCmd(bool bSkipCurrent = false);
	void	SafeDeleteCommand(BeExeCommand*& pkCommand);

protected:
	BeExeCommand* m_pkCurCmd;
	TwCommand				m_kCurCmd;
	std::list<TwCommand>	m_kCommands;
	bool					m_bForceNexeCmd;
};
