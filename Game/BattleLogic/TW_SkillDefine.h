#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#define MUTIL_EFFECT		1
#define MUTIL_TARGET		2
#define MUTIL_TARGET_AFTER	3
#define MUTIL_FACE_EFFECT	4
#define ATTACKE_SKILL		5
#define AUTOUSE_SKILL		6
#define BEDAMGED_SKILL		7
#define EFFECT_SKILLCD		8
#define KILL_SKILLCD		9
#define ATTACKE_SKILLCD		10
#define BEDAMGED_SKILLCD	11

enum BeSkillType
{
	OtherSkill = 0,
	UIShowSkill,
	UnShowSkill,
	GenuisSkill,
	Genus,
};

struct BeSkillData
{
	int		iCurLevel;
	bool	bIsActive;
	int		iLastTrgTime;
	int		iUIShowPos;
	BeSkillData()
	{
		iCurLevel = 0;
		bIsActive = false;
		iLastTrgTime = 0;
		iUIShowPos = -1;
	}
};
