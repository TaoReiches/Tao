#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

class SeRaceCmd;

class TwCommandProc
{
public:
    TwCommandProc();
    virtual ~TwCommandProc();

    bool ProcFrameCMD(const SeRaceCmd* pkRaceCmd);

    bool Proc_FRAME(const SeRaceCmd* pkRaceCmd);
    bool Proc_PLAYER_INFO(const SeRaceCmd* pkRaceCmd);
    bool Proc_START(const SeRaceCmd* pkRaceCmd);
    bool Proc_END(const SeRaceCmd* pkRaceCmd);
    bool Proc_CHART_PLAYER(const SeRaceCmd* pkRaceCmd);
    bool Proc_PLAYER_LEAVE(const SeRaceCmd* pkRaceCmd);
    bool Proc_SYSTEM_CHEAT(const SeRaceCmd* pkRaceCmd);
    bool Proc_HERO_MOVE(const SeRaceCmd* pkRaceCmd);
    bool Proc_UNIT_MOVE(const SeRaceCmd* pkRaceCmd);
    bool Proc_HERO_STOP(const SeRaceCmd* pkRaceCmd);
    bool Proc_HERO_ATTACK_POS(const SeRaceCmd* pkRaceCmd);
    bool Proc_HERO_ATTACK_UNIT(const SeRaceCmd* pkRaceCmd);
    bool Proc_UNIT_SPELL(const SeRaceCmd* pkRaceCmd);
    bool Proc_UNIT_LEARN_SKILL(const SeRaceCmd* pkRaceCmd);
    bool Proc_UNIT_ACTIVE_SKILL(const SeRaceCmd* pkRaceCmd);
    bool Proc_PLAYER_RELIVEHERO(const SeRaceCmd* pkRaceCmd);
    bool Proc_PLAYER_MAP_MESSAGE(const SeRaceCmd* pkRaceCmd);
    bool Proc_UNIT_DISPLACE_ITEM(const SeRaceCmd* pkRaceCmd);
    bool Proc_UNIT_CHANGE_ITEM(const SeRaceCmd* pkRaceCmd);
    bool Proc_Show_Action(const SeRaceCmd* pkRaceCmd);

    bool	Proc_HERO_DIRECT_MOVE(const SeRaceCmd* pkRaceCmd);
    bool	Proc_HERO_ATTACK_CANCEL(const SeRaceCmd* pkRaceCmd);
};
