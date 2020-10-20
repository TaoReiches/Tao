#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_EffectDefine.h"

class BeEffectData
{
public:
	BeEffectData(void)
	{
		iProperty = 0;
		iOwnerID = 0;
		iPlayer = 0;
		uiRemoveTime = -1;
		eTargetType = BeEffectTargetType::BET_POSITION;
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
		bMustCreate = false;
		dwCreateTime = 0;
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
	bool		bMustCreate;
	unsigned int		dwCreateTime;
};
