#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_MemoryPool.h"
#include "TW_Command.h"

#define NORMAL_RAD_SPEED	0.05714f
#define SLOW_RAD_SPEED		0.02248f
#define FAST_RAD_SPEED		0.10148f

enum BeEffectFlag
{
	BEF_REMOVE = (1 << 0),
	BEF_CANTSHOW = (1 << 1),

	BEF_IGNORE_MAGIC_IMMUNE = (1 << 2),
	BEF_IGNORE_INVISIBLE = (1 << 3),
	BEF_IGNORE_INVINCIBLE = (1 << 4),
	BEF_ATTRMISSLE = (1 << 5),
	BEF_IGNORE_WITHSTAND = (1 << 6),
	BEF_REMOVE_WHENUNITDEAD = (1 << 7),
	BEF_IN_MAP_SHOW_MOVE = (1 << 8),
	BEF_IN_MAP_SHOW_CYCLE = (1 << 9),

	BEF_CANT_FORBID = (1 << 10),
	BEF_MUST_CREATE = (1 << 11),

	BEF_NODEATHACTION = (1 << 12),
	BEF_NOT_SETPOS = (1 << 13),
	BEF_VISION = (1 << 14),
	BEF_SHOW_IN_WARFOG = (1 << 15),
	BEF_FLY_FUST = (1 << 16),
	BEF_NOMODELFILE = (1 << 17),
	BEF_IGNORE_PHYSIC_IMMUNE = (1 << 18),
	BEF_CYC = (1 << 19),
};

enum BeEffectTargetType
{
	BET_POSITION = BCTT_MAP,
	BET_UNIT = BCTT_UNIT,
	BET_ITEM = BCTT_MAPITEM,
	BET_EFFECT = BCTT_EFFECT,
};

enum BeEffectColorType
{
	FONT_COLOR_GREEN = 0xFF00FF00,
	FONT_COLOR_GREEN_MID = 0xFF58D03B,
	FONT_COLOR_GREEN_LOW = 0xFF80FF80,
	FONT_COLOR_PURE_RED = 0xFFFF0000,
	FONT_COLOR_RED_LOW = 0xFF8E48E5,
	FONT_COLOR_RED_MID = 0xFFFF8080,
	FONT_COLOR_RED_HIGH = 0xFFFF8C00,
	FONT_COLOR_BLUE_LOW = 0xFF8080FF,
	FONT_COLOR_GRAY = 0xFF808080,
	FONT_COLOR_GRAY_HIGH = 0xFFC0C0C0,
};

struct BeEffectData
{
	BeEffectData(void)
	{
		iProperty = 0;
		iOwnerID = 0;
		iPlayer = 0;
		uiRemoveTime = -1;
		eTargetType = BET_POSITION;
		iSourceID = 0;
		iTargetID = 0;
		fOrgPosX = 0.0f;
		fOrgPosY = 0.0f;
		fOrgPosZ = 0.0f;
		fTarPosX = 0.0f;
		fTarPosY = 0.0f;
		fTarPosZ = 0.0f;

		fPosX = 0.0f;
		fPosY = 0.0f;
		fPosZ = 0.0f;

		fMoveSpeed = 0.0f;
		fMissileArc = 0.0f;

		fFace = 0.0f;

		iSkillTypeID = 0;
		iSkillLevel = 0;

		fDamageScope = 0.0f;
		fCollisionScope = 0.0f;
		iTargetFlag = 0;

		iBumpCount = 0;

		iData = 0;
		fData = 0.0f;

		iShowPlayer = -1;
		iShowGroup = -1;
		bMustCreate = false;

		dwCreateTime = 0;
		fVisionRadius = 0.0f;
	}

	int			iOwnerID;
	int			iPlayer;
	int			iProperty;

	unsigned int		uiRemoveTime;

	BeEffectTargetType eTargetType;
	int			iSourceID;
	int			iTargetID;

	float       fOrgPosX;
	float       fOrgPosY;
	float       fOrgPosZ;

	float		fTarPosX;
	float		fTarPosY;
	float		fTarPosZ;

	float		fPosX;
	float		fPosY;
	float		fPosZ;

	float		fMoveSpeed;
	int			iSkillTypeID;
	int			iSkillLevel;

	float		fDamageScope;
	float       fCollisionScope;
	int			iTargetFlag;

	int			iBumpCount;

	int			iData;
	float		fData;
	float       fMissileArc;
	float		fFace;
	float		fVisionRadius;

	int			iShowPlayer;
	int			iShowGroup;
	bool		bMustCreate;
	unsigned int		dwCreateTime;
};
