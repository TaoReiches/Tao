#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#pragma once
#pragma pack(push, 1)

enum SeRaceCmdType
{
	SRC_FRAME = 0,
	SRC_PLAYER_INFO,
	SRC_CHART_PLAYER,

	SRC_PLAYER_LEAVE,
	SRC_SYSTEM_CHEAT,
	SRC_RACE_PAUSE,
	SRC_PAUSE_CANCEL,

	SRC_HERO_DIRECT_MOVE,
	SRC_HERO_MOVE,
	SRC_UNIT_MOVE,
	SRC_HERO_STOP,
	SRC_UNIT_STOP,
	SRC_HERO_HOLD,
	SRC_UNIT_HOLD,
	SRC_HERO_ATTACK_POS,
	SRC_HERO_ATTACK_CANCEL,
	SRC_UNIT_ATTACK_POS,
	SRC_HERO_ATTACK_UNIT,
	SRC_UNIT_ATTACK_UNIT,
	SRC_UNIT_ATTACK,
	SRC_UNIT_PATROL,
	SRC_UNIT_FOLLOW,
	SRC_UNIT_SPELL,
	SRC_UNIT_SPELLXULI,
	SRC_UNIT_USE_ITEM,
	SRC_UNIT_DROP_ITEM,
	SRC_UNIT_PICK_ITEM,

	SPRCT_UNITDISAPPEAR,
	SPRCT_KILLHERODATA,
	SPRCT_DAMAGEDATA,
	SPRCT_SELFDATA,
	SPRCT_MAPITEMDATA,
	SPRCT_ATTACHDATA,
	SPRCT_UIWINDOWDATA,
	SPRCT_UNITSHOWDATA,
	SPRCT_BUFFERDATA,
	SPRCT_CHAINEFFECTDATA,
	SPRCT_EFFECTDATA,
	SPRCT_EFFECTDEL,
	SPRCT_GLOBALDATA,
	SPRCT_MAPPOINTDATA,
	SPRCT_PING,
	SPRCT_LOCKUNITDATA,
	SPRCT_LOAD,
	SPRCT_GAME_END,
	SPRCT_TABINFO,
	SPRCT_UNIT_FLY,
	SPRCT_SMALLMAP,

	SPRCT_END,

	SRC_VIEW,
	SRC_SMALLMAP,

	SRC_CONNECT,
	SRC_LOAD_PERCENT,
	SPRCT_GAME_NOTICE,

	SRC_GAME_HAS_END,
	SRC_MAX_HEARTBEAT,
	SRC_TOOMANY_PACK,

	SRC_UNIT_CHANGE_ITEM,
	SRC_TRUN_TABLE_INFO,
	SRC_SHOW_ACTION,
	SRC_COST_EXTERNAL_RESOURCE,
	SRC_8V8_DEL_PLAYER,
	SRC_8V8_ADD_PLAYER,
	SRC_8V8_SYNC_ALL_PLAYER,
	SRC_LOAD_RACING_ONLYONE,
	SRC_LOAD_RACING_ALL_LOAD,
	SRC_NOTIFY_SECRETAREA_INFO,
	SRC_CMD_NUM,
};

struct SeRaceCmd
{
	SeRaceCmd(SeRaceCmdType cmd = SRC_FRAME, unsigned int frame = 0) :dwCmd(cmd), dwFrame(frame), dwSeat(0), dwQueue(0)
	{
	}

	unsigned int dwFrame : 19;
	unsigned int dwSeat : 5;
	unsigned int dwQueue : 1;
	unsigned int dwCmd : 7;
};

struct BeTargetData
{
	BeTargetData()
	{
		byTargetType = 0;
		iTargetID = 0;
	}
	char byTargetType;
	union
	{
		int iTargetID;
		struct
		{
			short sTargetX;
			short sTargetY;
		};
	};
};

struct SeRaceFrame : public SeRaceCmd
{
	SeRaceFrame() : SeRaceCmd(SRC_FRAME)
	{
	}
};

struct SeRaceConnect : public SeRaceCmd
{
	SeRaceConnect() : SeRaceCmd(SRC_CONNECT)
	{
		iUserID = 0;
	}

	int			iUserID;
};

struct	SeRaceMaxHeratBeat : public SeRaceCmd
{
	SeRaceMaxHeratBeat() : SeRaceCmd(SRC_MAX_HEARTBEAT)
	{
		iLen = 8;
	}
	int				iLen;
};

struct	SeRaceTooManyPack : public SeRaceCmd
{
	SeRaceTooManyPack() : SeRaceCmd(SRC_TOOMANY_PACK)
	{
		iLen = 8;
	}
	int				iLen;
};

struct SeRaceCmd_PlayerLeave : public SeRaceCmd
{
	SeRaceCmd_PlayerLeave() : SeRaceCmd(SRC_PLAYER_LEAVE)
	{
	}
};

struct SeRaceCmd_HeroMove : public SeRaceCmd
{
	SeRaceCmd_HeroMove() : SeRaceCmd(SRC_HERO_MOVE)
	{
		sTargetX = 0;
		sTargetY = 0;
	}

	short sTargetX;
	short sTargetY;
};

struct SeRaceCmd_HeroDirectMove : public SeRaceCmd
{
	SeRaceCmd_HeroDirectMove() : SeRaceCmd(SRC_HERO_DIRECT_MOVE)
	{
		fMoveDirect = 0.0f;
	}

	float	fMoveDirect;
};

struct SeRaceCmd_HeroHold : public SeRaceCmd
{
	SeRaceCmd_HeroHold() : SeRaceCmd(SRC_HERO_HOLD)
	{
	}
};

struct SeRaceCmd_HeroAttackPos : public SeRaceCmd
{
	SeRaceCmd_HeroAttackPos() : SeRaceCmd(SRC_HERO_ATTACK_POS)
	{
		sTargetX = 0;
		sTargetY = 0;
	}

	short sTargetX;
	short sTargetY;
};

struct SeRaceCmd_HeroAttackCancel : public SeRaceCmd
{
	SeRaceCmd_HeroAttackCancel() : SeRaceCmd(SRC_HERO_ATTACK_CANCEL)
	{
	}
};

struct SeRaceCmd_UnitAttackPos : public SeRaceCmd
{
	SeRaceCmd_UnitAttackPos() : SeRaceCmd(SRC_UNIT_ATTACK_POS)
	{
		sTargetX = 0;
		sTargetY = 0;
		bySize = 0;
		aiIDs[0] = 0;
	}

	short sTargetX;
	short sTargetY;
	char bySize;
	int aiIDs[1];
};

struct SeRaceCmd_HeroAttackUnit : public SeRaceCmd
{
	SeRaceCmd_HeroAttackUnit() : SeRaceCmd(SRC_HERO_ATTACK_UNIT)
	{
		iTargetID = 0;
	}
	int iTargetID;
};

struct SeRaceCmd_UnitAttackUnit : public SeRaceCmd
{
	SeRaceCmd_UnitAttackUnit() : SeRaceCmd(SRC_UNIT_ATTACK_UNIT)
	{
		iTargetID = 0;
		bySize = 0;
		aiIDs[0] = 0;
	}

	int iTargetID;
	char bySize;
	int aiIDs[1];
};

struct SeRaceCmd_UnitAttack : public SeRaceCmd
{
	SeRaceCmd_UnitAttack() : SeRaceCmd(SRC_UNIT_ATTACK)
	{
		byTargetType = 0;
		iTargetID = 0;
		bySize = 0;
		aiIDs[0] = 0;
	}

	char byTargetType;
	int iTargetID;
	char bySize;
	int aiIDs[1];
};

struct SeRaceCmd_UnitSpell : public SeRaceCmd
{
	SeRaceCmd_UnitSpell() : SeRaceCmd(SRC_UNIT_SPELL)
	{
		iUnitID = 0;
		bySpellPos = -1;
		byTargetType = 0;
		iTargetID = 0;
		byAutoSelect = 0;
	}

	int iUnitID;
	char bySpellPos;
	char byTargetType;
	char byAutoSelect;
	union
	{
		int iTargetID;
		struct
		{
			short	sTargetX;
			short	sTargetY;
		};
	};
};

struct SeRaceCmd_UnitUseItem : public SeRaceCmd
{
	SeRaceCmd_UnitUseItem() : SeRaceCmd(SRC_UNIT_USE_ITEM)
	{
		byItemPos = -1;
		byTargetType = 0;
		iTargetID = 0;
	}

	char byItemPos;
	char byTargetType;
	union
	{
		int iTargetID;
		struct
		{
			short sTargetX;
			short sTargetY;
		};
	};
};

struct SeRaceCmd_UnitDropItem : public SeRaceCmd
{
	SeRaceCmd_UnitDropItem() : SeRaceCmd(SRC_UNIT_DROP_ITEM)
	{
		iUnitID = 0;
		byItemPos = -1;
		byTargetType = 0;
		iTargetID = 0;
	}

	int iUnitID;
	char byItemPos;
	char byTargetType;
	union
	{
		int iTargetID;
		struct
		{
			short sTargetX;
			short sTargetY;
		};
	};
};

struct SeRaceCmd_UnitPickItem : public SeRaceCmd
{
	SeRaceCmd_UnitPickItem() : SeRaceCmd(SRC_UNIT_PICK_ITEM)
	{
		iUnitID = 0;
		dwMapItemID = 0;
	}

	int iUnitID;
	unsigned int dwMapItemID;
};

static	const	int		SSD_ENDSTR_LEN = 4;

struct SeAddPlayerRaceLoad : public SeRaceCmd
{

	SeAddPlayerRaceLoad() : SeRaceCmd(SRC_8V8_ADD_PLAYER)
	{
		iCmdLength = 0;
		iMapID = 0;
		dwRoomID = 0;
		iDataSize = 0;
		bIsServerMode = false;
		dwUserID = 0;
		std::memset(acData, 0, SSD_ENDSTR_LEN);
	}

	int			iCmdLength;
	int			dwRoomID;
	int			iMapID;
	bool		bIsServerMode;
	unsigned int		dwUserID;
	int			iDataSize;
	char		acData[SSD_ENDSTR_LEN];
};

enum ShowActionType {
	SAT_SHOW_PROFICIENCY_ACTION = 1,
	SAT_SHOW_EMOJI_ACTION = 2,
};

struct SeRaceCmd_ShowAction : public SeRaceCmd
{
	SeRaceCmd_ShowAction() : SeRaceCmd(SRC_SHOW_ACTION)
	{
		usOpt = 0;
	}

	unsigned short	usOpt;
};
#pragma pack(pop) 
