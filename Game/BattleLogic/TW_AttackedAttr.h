#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

enum BeAttackedAttType
{
	AAT_AVOID_PHYSIC,
	AAT_RESIST_PHYSIC,
	AAT_ANTI_PHYSIC,
	AAT_ANTI_MAGIC,
	AAT_REBOUND_SKILL,
	AAT_REBOUND_PHYSIC_SHORTRANGE,
	AAT_REBOUND_PHYSIC_BOTH,
	AAT_ANTI_LEECH,
	AAT_REBOUND_SKILL_ALL,

	AAT_NUM,
};

struct BeAvoidPhysicAttr
{
	float	fRate;
};

struct BeResistPhysicAttr
{
	float	fRate;
	float	fShortRangeResistValue;
	float	fEjectResistValue;
	float	fAdditionalResistValue;
};

struct BeAntiPhysicAttr
{
	float	fAntiPhysic;
};

struct BeAntiLeechAttr
{
	float fAntiLeech;
};

struct BeAntiMagicAttr
{
	float	fAntiMagic;
};

struct BeReboundSkillAttr
{
	float	fRebound;
};

struct BeReboundSkillAllAttr
{
	float	fRebound;
};

struct BeReboundPhysicShortRangeAttr
{
	float	fPhysicValue;
};

struct BeReboundPhysicBothAttr
{
	float fShortRangeValue;
	float fEjectValue;
};

struct BeAttackedAttr
{
	BeAttackedAttr()
	{
		eType = AAT_NUM;
		iSkillTypeID = 0;
		iSkillLevel = 0;
	}
	BeAttackedAttType eType;
	int		iSkillTypeID;
	int		iSkillLevel;
	union
	{
		BeAvoidPhysicAttr	kAvoidPhysic;
		BeResistPhysicAttr	kResistPhysic;
		BeAntiPhysicAttr	kAntiPhysic;
		BeAntiMagicAttr		kAntiMagic;
		BeReboundSkillAttr	kReboundSkill;
		BeReboundPhysicShortRangeAttr	kReboundPhysicShortRange;
		BeReboundPhysicBothAttr	kReboundPhysicBoth;
		BeAntiLeechAttr kAntiLeech;
		BeReboundSkillAllAttr	kReboundSkillAll;
	};

	void InitAvoidPhysic(float rate)
	{
		eType = AAT_AVOID_PHYSIC;
		kAvoidPhysic.fRate = rate;
	}

	void InitResistPhysic(float rate, float value, float fEjectValue, float fAdd = 0.0f)
	{
		eType = AAT_RESIST_PHYSIC;
		kResistPhysic.fRate = rate;
		kResistPhysic.fShortRangeResistValue = value;
		kResistPhysic.fEjectResistValue = fEjectValue;
		kResistPhysic.fAdditionalResistValue = fAdd;
	}

	void InitAntiPhysic(float percent)
	{
		eType = AAT_ANTI_PHYSIC;
		kAntiPhysic.fAntiPhysic = percent;
	}

	void InitAntiMagic(float fValue)
	{
		eType = AAT_ANTI_MAGIC;
		kAntiMagic.fAntiMagic = fValue;
	}

	void InitAntiLeech(float percent)
	{
		eType = AAT_ANTI_LEECH;
		kAntiLeech.fAntiLeech = percent;
	}

	void InitReboundSkill(float percent)
	{
		eType = AAT_REBOUND_SKILL;
		kReboundSkill.fRebound = percent;
	}

	void InitReboundSkillAll(float percent)
	{
		eType = AAT_REBOUND_SKILL_ALL;
		kReboundSkillAll.fRebound = percent;
	}

	void InitReboundPhysicShortRange(float value)
	{
		eType = AAT_REBOUND_PHYSIC_SHORTRANGE;
		kReboundPhysicShortRange.fPhysicValue = value;
	}

	void InitReboundPhysicBoth(float fShortRangeValue, float fEjectValue)
	{
		eType = AAT_REBOUND_PHYSIC_BOTH;
		kReboundPhysicBoth.fShortRangeValue = fShortRangeValue;
		kReboundPhysicBoth.fEjectValue = fEjectValue;
	}
};
