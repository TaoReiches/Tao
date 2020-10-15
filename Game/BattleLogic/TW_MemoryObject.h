#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_MemoryPool.h"

class BeTaskActionStand;
extern TeMemoryPool<BeTaskActionStand>          mpTaskActionStand;

class BeTaskActionWalk;
extern TeMemoryPool<BeTaskActionWalk>           mpTaskActionWalk;

class BeTaskActionAttack;
extern TeMemoryPool<BeTaskActionAttack>         mpTaskActionAttack;

class BeTaskActionSpell;
extern TeMemoryPool<BeTaskActionSpell>          mpTaskActionSpell;

class BeTaskActionDeath;
extern TeMemoryPool<BeTaskActionDeath>          mpTaskActionDeath;

class BeTaskMoveToPos;

class BeAttackingAttr;
extern TeMemoryPool<BeAttackingAttr>            mpAttackingAttr;

class BeUnit;
extern TeMemoryPool<BeUnit>                     mpUnit;

class BeSkill;
extern TeMemoryPool<BeSkill>                    mpSkill;
