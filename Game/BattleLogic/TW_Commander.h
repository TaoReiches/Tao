#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <list>
#include "TW_Command.h"

class BeCommander : public BeMainPtr, public BeUnitPtr
{
public:
	BeCommander();
	~BeCommander();

	BeExeCommand* GetCurCmd(void);
	bool	IsIdle(void) const;

	void	GiveCommand(BeCommand kCmd, BeGiveCmdType eType = BeGiveCmdType::BCT_IMMEDIATE, int iUnitID = 0, bool bNeedHangCurrent = true);
	void	ExecuteCmd(int iDeltaTime);
	int		GetCommandCount() const;
	BeCommand& GetCurCommand(void);
	std::list<BeCommand>& GetCommands(void);

	void    SetCommands(const std::list<BeCommand>& rkCommands);

	void	OnDelete(void);
	void    ClearAllCommands();

protected:
	bool	SwitchCmd(const BeCommand& kCmd, bool bConnect = true);
	bool	GoNextCmd(bool bSkipCurrent = false);
	void	SafeDeleteCommand(BeExeCommand*& pkCommand);

protected:
	BeExeCommand* m_pkCurCmd;
	BeCommand				m_kCurCmd;
	std::list<BeCommand>	m_kCommands;
	bool					m_bForceNexeCmd;
};
