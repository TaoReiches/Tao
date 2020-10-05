/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Skill.h"

IMPLEMENT_POOL(BeStopCommand, 128, 16);
IMPLEMENT_POOL(BeHoldCommand, 128, 16);
IMPLEMENT_POOL(BeMoveCommand, 128, 16);
IMPLEMENT_POOL(BePatrolCommand, 8, 4);
IMPLEMENT_POOL(BeAttackCommand, 128, 16);
IMPLEMENT_POOL(BeSpellCommand, 8, 4);
IMPLEMENT_POOL(BeDropItemCommand, 8, 4);
IMPLEMENT_POOL(BePickItemCommand, 8, 4);
IMPLEMENT_POOL(BeTaskActionStand, 128, 32);
IMPLEMENT_POOL(BeTaskActionWalk, 128, 32);
IMPLEMENT_POOL(BeTaskActionAttack, 128, 32);
IMPLEMENT_POOL(BeTaskActionSpell, 8, 4);
IMPLEMENT_POOL(BeTaskActionDeath, 64, 32);
IMPLEMENT_POOL(BeTaskMoveToPos, 128, 16);
IMPLEMENT_POOL(BeTaskMoveToUnit, 128, 16);
IMPLEMENT_POOL(BeTaskAttackUnit, 128, 16);
IMPLEMENT_POOL(BeTaskAttackToPos, 128, 16);
IMPLEMENT_POOL(BeTaskFollowUnit, 128, 16);
IMPLEMENT_POOL(BeTaskAttackItem, 128, 16);
IMPLEMENT_POOL(BeUnitData, 128, 16);
IMPLEMENT_POOL(BeAttackingAttr, 128, 16);
IMPLEMENT_POOL(BeEffectData, 256, 32);

IMPLEMENT_POOL1(BeBuffer, 64, 32);
IMPLEMENT_POOL1(BeSkill, 128, 16);

IMPLEMENT_POOL1(BeEffect, 64, 32);
IMPLEMENT_POOL1(BeUnit, 128, 16);

IMPLEMENT_POOL_ARRAY(BeLearnSkillData, MAX_LEARN_SKILLS, 128, 16);
